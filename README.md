PyEMD: Earth Mover's Distance for Python (and MATLAB)
=====================================================
by Gary Doran (<gary.doran@case.edu>)

Overview
--------
An efficient, accurate, easy-to-use EMD implementation in C with Python
wrapper. New bonus MATLAB wrapper also included.

Installation
------------
*Python:* this package can be installed in two ways (the easy way):

    # If needed:
    # pip install numpy
    # pip install scipy
    pip install -e git+https://github.com/garydoranjr/pyemd.git#egg=pyemd

or by running the setup file manually

    git clone [the url for pyemd]
    cd pyemd
    python setup.py install

Note the code requires Python 3 and depends on the `numpy` and `scipy` packages.
So have those installed first. The build will likely fail if it can't find them.
For more information, see:

 + [NumPy](http://www.numpy.org/): Library for efficient matrix math in Python
 + [SciPy](http://www.scipy.org/): Library for more MATLAB-like functionality

*MATLAB:* clone the repository and `cd` to the `matlab` subdirectory. Either
set the `MATLABDIR` environment variable, or edit the first line of the Makefile
to set the path to the desired MATLAB installation, and then run `make`.

After the MEX file has compiled, add the `matlab` subdirectory to the MATLAB
path (e.g., by using the `addpath` command in MATLAB).

Why?
----
Several Python wrappers for C-based EMD implementations already exist, so why is
another one necessary? There are two popular alternative approaches, each with
their limitations:

  - *Solve a LP with GLPK:* Since the transportation problem is a special case
    of the general LP formulation, it can be solved more efficiently and with
    much less use of memory. This implementation is approximately 7-8 times
    faster than a GLPK-based solution, and uses about 500 MB of memory when the
    GLPK-based solution uses over 10 GB before it crashes my machine.  These
    figures are from problems in which samples of size ~1000 with ~100 features
    are compared using the EMD for the multiple-instance learning problems I
    study.

  - *Wrap Yossi Rubner's Implementation:* There exist several wrappers of
    [Yossi Rubner's EMD code](http://robotics.stanford.edu/~rubner/emd/default.htm),
    the most popular of which is in the OpenCV library. The
    first limitation of this code is the use of single-precison versus
    double-precision floating point numbers.  Another issue is a hard-coded
    `MAX_SIG_SIZE`, which limits the size of the samples that can be used in
    the EMD computation.

PyEMD is a more "Pythonic" EMD implementation than other wrappers. Only the
minimal amount of computation is done in C (the core transportation algorithm).
This means that the distance computation is done in Python using the efficient
SciPy library, and a custom, precomputed distance matrix can be easily provided.

Usage
-----
The EMD implementation can be used simply in Python as:

    >>> from emd import emd
    >>> emd(X, Y)

where `X` and `Y` are each n-dimensional samples of points. Each argument should
be a NumPy array with n columns, but possibly different numbers of rows.

If the sample is weighted, the weights can be specified with optional
`X_weights` and `Y_weights` arguments. By default, uniform weights are used.
Because the EMD is a distance between probability measures, the *total weights
of each of the two samples must sum to 1*.

By default, the Euclidean distance between points is used. However, an optional
argument `distance` takes a string that specifies a valid distance type accepted
by the `scipy.spatial.cdist` function. Alternatively, if
`distance='precomputed'`, then a precomputed distance matrix is expected to be
supplied to the optional argument `D`.

Finally, PyEMD can also return the flows between the two samples that are used
to compute the distance. If the `return_flows` argument is `True`, then two
arguments, the distance an array of the flows, are returned.

See the docstring for a more formal description of the functionality. In MATLAB,
the functionality is essentially the same; see the help for details.

Citing
------
If you have used PyEMD for your research and would like to cite it, you can use
the following BibTeX entry:

    @Misc{,
      author =    {Gary Doran},
      title =     {{PyEMD}: Earth Mover's Distance for {Python}},
      year =      {2014--},
      url = "https://github.com/garydoranjr/pyemd",
      note = {[Online; accessed <today>]}
    }

Questions and Issues
--------------------
If you find any bugs or have any questions about this code, please create an
issue on [GitHub](https://github.com/garydoranjr/pyemd/issues), or contact Gary
Doran at <gary.doran@case.edu>. Of course, I cannot guarantee any support for
this software.
