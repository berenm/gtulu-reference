import os
import argparse

binary_dir = os.getcwd()
source_dir = os.path.join(os.path.dirname(__file__), '..')

def arguments():
  parser = argparse.ArgumentParser()
  parser.add_argument('--version', '-v', type=str, help='version of OpenGL to process')
  parser.add_argument('--compat', action='store_true', help='user OpenGL compat version instead of core')
  return parser.parse_args()


def convert_type(t, is_return=False):
  typemap = {'GLvoid':    'void',
             'GLboolean': 'bool',
             'GLbyte':    'int8_t',
             'GLshort':   'int16_t',
             'GLint':     'int32_t',
             'GLint64':   'int64_t',
             'GLubyte':   'uint8_t',
             'GLushort':  'uint16_t',
             'GLuint':    'uint32_t',
             'GLuint64':  'uint64_t',
             'GLsizei':   'gtu::size',
             'GLintptr':  'std::ptrdiff_t',
             'GLsizeiptr':'std::ptrdiff_t',
             'GLbitfield':'gtu::bitmask const&',
             'GLenum':    'gtu::constant',
             'GLhalf':    'gtu::float16',
             'GLfloat':   'gtu::float32',
             'GLdouble':  'gtu::float64',
             'GLclampf':  'gtu::float32',
             'GLclampd':  'gtu::float64',
             'GLvoid const*':  'gtu::buffer_ref const&',
             'void const*':    'gtu::buffer_ref const&',
             'GLchar const*':  'std::string const&',
             'char const*':    'std::string const&',
             'const GLubyte*':  'std::string const&',
             'GLfloat const*': 'gtu::float32 const*',
             'GLdouble const*': 'gtu::float64 const*',
             'GLbyte const*': 'int8_t const*',
             'GLshort const*': 'int16_t const*',
             'GLint const*':   'int32_t const*',
             'GLubyte const*': 'uint8_t const*',
             'GLushort const*': 'uint16_t const*',
             'GLuint const*':  'uint32_t const*',
             'GLenum const*':  'gtu::constant const*',
             'GLfloat*': 'gtu::float32*',
             'GLint*':   'int32_t*',
             'GLuint*':  'uint32_t*',
             'GLenum*':  'gtu::constant*',
             'GLsizei*': 'gtu::size*',
             'GLint64*': 'int64_t*',
             'GLboolean*': 'bool*',
             'GLchar*':  'std::string&',
             'char*':    'std::string&',
             'GLsync':   'gtu::barrier'}
  # if t not in typemap:
  #   raise ValueError(t)
  ct = typemap.get(t, t)
  if is_return:
    return ct.replace(' const&', '')
  else:
    return ct + ((not '*' in t and ' const' not in ct) and ' const' or '')


def enumerate_values(constant):
  if 'value' in constant:
    return [{'name': constant['gtu']['name'], 'value': hex(constant['value'])}]
  else:
    return [{'name': constant['gtu']['name'][:-1] + str(i), 'value': hex(v)} for i,v in enumerate(constant['values'])]


def categorize_constants(constants):
  categories = {'__all__': {}}
  for n,c in sorted(constants.items()):
    if 'categories' not in c or all([ 'gl_' == cat[:3] for cat in c['categories']]):
      for u in enumerate_values(c):
        categories['__all__'][u['name']] = u

    else:
      for cat in [cc.lower() for cc in c['categories'] if cc[:3] != 'gl_']:
        if cat not in categories:
          categories[cat] = {}
        for u in enumerate_values(c):
          categories[cat][u['name']] = u

  return categories


def man(v, file=None):
  if '.' in v:
    v = v[0]

  p = os.path.join(source_dir, 'man', 'gl', '%s' % v)
  if not os.path.exists(p):
    os.makedirs(p)

  if file is not None:
    p = os.path.join(p, file)

  return p


def ref(v, c=None, file=None):
  if v == '2.1':
    p = os.path.join(source_dir, 'gl', v)
  else:
    p = os.path.join(source_dir, 'gl', v, c)

  if file is not None:
    p = os.path.join(p, file)

  return p


def api(v, c=None, file=None):
  if v == '2.1':
    p = os.path.join('gl', v)
  else:
    p = os.path.join('gl', v, c)

  if not os.path.exists(p):
    os.makedirs(p)

  if file is not None:
    p = os.path.join(p, file)

  return p


def list(v, file=None):
  if '.' in v:
    v = v[0]

  p = os.path.join('list', 'gl%s' % v)
  if not os.path.exists(p):
    os.makedirs(p)

  if file is not None:
    p = os.path.join(p, file)

  return p


def specs(v, file=None):
  if '.' in v:
    v = v[0]

  p = os.path.join('specs', 'gl%s' % v)
  if not os.path.exists(p):
    os.makedirs(p)

  if file is not None:
    p = os.path.join(p, file)

  return p


def spec(file=None):
  p = os.path.join(source_dir, 'lib', 'registry', 'share', 'gl', 'spec')

  if file is not None:
    p = os.path.join(p, file)

  return p