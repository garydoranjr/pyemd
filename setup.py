#!/usr/bin/env python

from distutils.core import setup, Extension
setup(name='emd', version='1.0',  \
      ext_modules=[Extension('emd', ['pyemd/pyemd.c', 'pyemd/emd.c'], include_dirs=['pyemd'])])
