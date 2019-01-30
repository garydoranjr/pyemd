"""
Wraper for C EMD implementation
"""
import numpy as np
from scipy.spatial.distance import cdist

from c_emd import _emd

def emd(X, Y, X_weights=None, Y_weights=None, distance='euclidean',
        D=None, return_flows=False):
    """
    Computes the EMD between two weighted samples
    @param X : First sample
    @param Y : Second sample
    @param X_weights : weights of elements in X (must sum to 1);
        if None, 1/|X| is used as the weight of each element
    @param Y_weights : weights of elements in Y (must sum to 1);
        if None, 1/|Y| is used as the weight of each element
    @param distance : valid distance type used by scipy.spatial.distance.cdist,
        or "precomputed" if the pairwise distances D is supplied
    @param D : precomputed distance matrix; ignored unless distance="precomputed";
        must be array of size |X|-by-|Y|
    @param return_flows : whether to return the flows between cells in addition
        to the distance (default False)
    """

    if X_weights is not None:
        if not isinstance(X_weights, np.ndarray):
            raise ValueError("X_weights has to be a Numpy ndarray, not '{}'.".format(type(X_weights)))
        if not X_weights.flags['C_CONTIGUOUS']:
            raise ValueError("X_weights has to be a C-contiguous array.")
        if not X_weights.dtype == np.float64:
            raise ValueError("X_weights has to be of dtype double (float64).")
        if np.any(X_weights < 0):
            raise ValueError("X_weights has to be non-negative.")
        if not np.isclose(np.sum(X_weights), 1):
            raise ValueError("X_weights has to sum to 1.")

    if Y_weights is not None:
        if not isinstance(Y_weights, np.ndarray):
            raise ValueError("Y_weights has to be a Numpy ndarray, not '{}'.".format(type(Y_weights)))
        if not Y_weights.flags['C_CONTIGUOUS']:
            raise ValueError("Y_weights has to be a C-contiguous array.")
        if not Y_weights.dtype == np.float64:
            raise ValueError("Y_weights has to be of dtype double (float64).")
        if np.any(Y_weights < 0):
            raise ValueError("Y_weights has to be non-negative.")
        if not np.isclose(np.sum(Y_weights), 1):
            raise ValueError("Y_weights has to sum to 1.")

    if distance != 'precomputed':
        n = len(X)
        m = len(Y)
        D = cdist(X, Y, distance)
        if X_weights is None:
            X_weights = np.ones(n)/n
        elif n != len(X_weights):
            raise ValueError('Size mismatch of X and X_weights')
        if Y_weights is None:
            Y_weights = np.ones(m)/m
        elif m != len(Y_weights):
            raise ValueError('Size mismatch of Y and Y_weights')

    else:
        if D is None:
            raise ValueError("D must be supplied when distance='precomputed'")

        if not isinstance(D, np.ndarray):
            raise ValueError("D has to be a Numpy ndarray, not '{}'.".format(type(D)))
        if not D.flags['C_CONTIGUOUS']:
            raise ValueError("D has to be a C-contiguous array.")
        if not D.dtype == np.float64:
            raise ValueError("D has to be of dtype double (float64).")
        if np.any(D < 0):
            raise ValueError("D has to be non-negative.")

        n, m = D.shape
        if X_weights is None:
            X_weights = np.ones(n)/n
        elif n != len(X_weights):
            raise ValueError('Size mismatch of D and X_weights')
        if Y_weights is None:
            Y_weights = np.ones(m)/m
        elif m != len(Y_weights):
            raise ValueError('Size mismatch of D and Y_weights')

    return _emd(X_weights, Y_weights, D, return_flows)
