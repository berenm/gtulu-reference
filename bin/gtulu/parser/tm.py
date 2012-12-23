import re
import logging
from gtulu import logger

log = logging.getLogger(__name__)

class TMParser(object):
  MapEntryRE = re.compile(r'^(?P<type_spec>\w+),\*,\*,\s*(?P<type_c>(?:[\w\*]+ ?)+),\*,\*,?$')
  CommentRE = re.compile(r'^#.*$')

  def __init__(self, filename):
    super(TMParser, self).__init__()
    self.filename = filename

  def parse(self):
    with open(self.filename) as tm:
      for line in tm:
        line = line.strip('\n')
        match = TMParser.MapEntryRE.match(line)
        if match is None and TMParser.CommentRE.match(line) is None:
          log.error('"%s" did not match %s' % (line, TMParser.MapEntryRE))
          raise SyntaxError('"%s" did not match %s' % (line, TMParser.MapEntryRE))
        else:
          pass
          # log.debug('Match: type_spec: %s, type_c: %s' % (match.group('type_spec'), match.group('type_c')))
    pass
