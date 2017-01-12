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
