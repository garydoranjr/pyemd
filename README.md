PyEMD: Earth Mover's Distance for Python
========================================
by Gary Doran (<gary.doran@case.edu>)

Overview
--------
An efficient, easy-to-use EMD implementation in C with Python wrapper.

Installation
------------
This package can be installed in two ways (the easy way):

    # If needed:
    # pip install numpy
    # pip install scipy
    pip install -e git+https://github.com/garydoranjr/pyemd.git#egg=misvm

or by running the setup file manually

    git clone [the url for pyemd]
    cd pyemd
    python setup.py install

Note the code depends on the `numpy` and `scipy` packages. So have those
installed first. The build will likely fail if it can't find them. For more information, see:

 + [NumPy](http://www.numpy.org/): Library for efficient matrix math in Python
 + [SciPy](http://www.scipy.org/): Library for more MATLAB-like functionality

Why?
----
Several Python wrappers for C-based EMD implementations already exist, so why is
another one necessary? There are two general approaches, each with their
limitations:

  - *Solve a LP with GLPK:*
  - *Wrap Yossi Rubner's Implementation:*

Usage
-----
How to use.

Questions and Issues
--------------------
If you find any bugs or have any questions about this code, please create an
issue on [GitHub](https://github.com/garydoranjr/pyemd/issues), or contact Gary
Doran at <gary.doran@case.edu>. Of course, I cannot guarantee any support for
this software.
