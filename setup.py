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
    version='2.0',
    description=(
      "Accurate, efficient Earth Mover's Distance in Python."
    ),
    author='Gary Doran',
    author_email='gary.doran@case.edu',
    url='https://github.com/garydoranjr/pyemd.git',
    license="BSD compatable (see the LICENSE file)",
    platforms=['unix'],
    packages=['emd'],
    ext_modules=[c_emd]
)
