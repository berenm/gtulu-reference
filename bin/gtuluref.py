import os
import argparse

binary_dir = os.getcwd()
source_dir = os.path.join(os.path.dirname(__file__), '..')

def arguments():
	parser = argparse.ArgumentParser()
	parser.add_argument('--version', '-v', type=str, help='version of OpenGL to process')
	parser.add_argument('--compat', action='store_true', help='user OpenGL compat version instead of core')
	return parser.parse_args()


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