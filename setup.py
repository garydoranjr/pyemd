#!/usr/bin/env python
from distutils.core import setup, Extension
import numpy as np

c_emd = Extension(
    'c_emd',
    sources=['c_emd/pyemd.c',
             'c_emd/emd.c' ],
    include_dirs=['c_emd', np.get_include()])

setup(
    name='emd',
    version='1.0',
    packages=['emd'],
    ext_modules=[c_emd]
)
