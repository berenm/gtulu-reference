import os

root=os.path.join(os.path.dirname(__file__), '..')

def man(v, file=None):
	if '.' in v:
		v = v[0]

	p = os.path.join(root, 'man', 'gl', '%s' % v)
	if not os.path.exists(p):
		os.makedirs(p)

	if file is not None:
		p = os.path.join(p, file)

	return p


def ref(v, c=None, file=None):
	if v == '2.1':
		p = os.path.join(root, 'gl', v)
	else:
		p = os.path.join(root, 'gl', v, c)

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
	p = os.path.join(root, 'lib', 'registry', 'share', 'gl', 'spec')

	if file is not None:
		p = os.path.join(p, file)

	return p