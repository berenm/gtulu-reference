import re
import logging
from gtulu import logger

log = logging.getLogger(__name__)

class SpecParser(object):
  Keywords = ['param', 'category', 'subcategory', 'version', 'deprecated', \
              'return', 'profile', 'offset', 'extension', 'alias', \
              '(?:gl|glx)?vectorequiv', 'beginend', 'required-props', \
              'dlflags', 'glxsingle', 'glxflags', 'glxropcode', 'glxvendorpriv', \
              'glfflags', 'wglflags', 'glextmask']

  FunctionRE         = re.compile(r'^(?P<name>\w+)\([\w, ]*\)$')
  EnumerationRE      = re.compile(r'^(?P<name>\w+)(?:, \w+)* (?:define|enum):.*$')
  EnumerationValueRE = re.compile(r'^\s+(?P<name>\w+)\s+=(?P<value>[^#]*)(?P<comment>#.*)?$')
  EnumerationAliasRE = re.compile(r'^\s+use\s+(?P<name_enum>\w+)\s+(?P<name_value>[^#]*)(?P<comment>#.*)?$')
  AttributeRE        = re.compile(r'^\s+(?P<keyword>' + '|'.join(Keywords) + r')(?P<value>.*)$')
  AttributeValuesRE  = re.compile(r'^(?P<keyword>' + '|'.join(Keywords) + r').*$')
  NewCategoryRE      = re.compile(r'^newcategory:.*$')
  PassthruRE         = re.compile(r'^passthru:.*$')
  ParameterRE        = re.compile(r'^(?P<name>\w+)\s+(?P<type>\w+)\s+(?P<inout>\w+)\s+(?P<passby>\w+)(?P<size>|\s+\[[\w\(\),\*/]*\])(?P<retained>|\s+retained)$')
  CommentRE          = re.compile(r'^#.*$')

  def __init__(self, filename):
    super(SpecParser, self).__init__()
    self.filename = filename

  def parse(self):
    result = {'functions':{}, 'enumerations':{}}

    function = lambda: {'name': None, 'params':[], 'return': None, 'categories':[], 'properties': {}}
    enumeration = lambda: {'value': None, 'categories':[]}

    current_function = None
    current_enumeration = None

    with open(self.filename) as tm:
      pline = None
      for line in tm:
        line = line.strip('\n')
        if line.strip() == '':
          continue
        # line = line.strip()

        if SpecParser.FunctionRE.match(line) is not None:
          m = SpecParser.FunctionRE.match(line)
          if m.group('name') in result['functions']:
            log.error('%s already in functions' % m.group('name'))
          current_function = result['functions'][m.group('name')] = function()
          current_function['name'] = m.group('name')

        elif SpecParser.EnumerationRE.match(line) is not None:
          m = SpecParser.EnumerationRE.match(line)
          current_enumeration = m.group('name')

        elif SpecParser.AttributeRE.match(line) is not None:
          m = SpecParser.AttributeRE.match(line)
          if m.group('keyword') == 'param':
            mm = SpecParser.ParameterRE.match(m.group('value').strip())
            if mm is None:
              log.error('"%s" did not match.' % m.group('value').strip())
            else:
              current_function['params'].append({'name': mm.group('name').strip(), \
                                                 'type': mm.group('type').strip(), \
                                                 'inout': mm.group('inout').strip(), \
                                                 'passby': mm.group('passby').strip(), \
                                                 'size': mm.group('size').strip().strip('[]'), \
                                                 'retained': mm.group('retained').strip()})

          elif m.group('keyword') == 'return':
            current_function['return'] = m.group('value').strip()
          elif m.group('keyword') == 'category':
            current_function['categories'].extend([ cat.strip() for cat in m.group('value').strip().split('# old:') ])
          elif m.group('keyword')[:3] != 'glx':
            current_function['properties'][m.group('keyword')] = m.group('value').strip()

        elif SpecParser.EnumerationValueRE.match(line) is not None:
          m = SpecParser.EnumerationValueRE.match(line)
          name  = m.group('name')
          if not name in result['enumerations']:
            result['enumerations'][name] = enumeration()
          result['enumerations'][name]['categories'].append(current_enumeration)

          value = m.group('value').strip()
          try:
            if value[:2] == '0x':
              value = int(value, 16)
            else:
              value = int(value, 10)
          except ValueError:
            pass

          current_value = result['enumerations'][name]['value']
          if current_value is not None and current_value != value:
            log.error('%s value differs previous: %s, new: %s' % (name, current_value, value))
          result['enumerations'][name]['value'] = value

        elif SpecParser.EnumerationAliasRE.match(line) is not None:
          m = SpecParser.EnumerationAliasRE.match(line)
          name  = m.group('name_value')
          if not name in result['enumerations']:
            result['enumerations'][name] = enumeration()
          result['enumerations'][name]['categories'].append(current_enumeration)
          result['enumerations'][name]['categories'].append(m.group('name_enum'))

        else:
          matches = [ p.match(line) for p in [ SpecParser.NewCategoryRE, SpecParser.PassthruRE, \
                                               SpecParser.AttributeValuesRE, SpecParser.CommentRE ] ]
          if all([ m is None for m in matches ]):
            log.error('"%s" did not match.' % line)
            # raise SyntaxError('"%s" did not match %s' % (line, SpecParser.FunctionRE))
          else:
            pass
            # log.debug('Match: type_spec: %s, type_c: %s' % (match.group('type_spec'), match.group('type_c')))
        pline = line

      for k,v in result['functions'].items():
        v['categories'] = list(set(v['categories']))
      for k,v in result['enumerations'].items():
        v['categories'] = list(set(v['categories']))

    return result
