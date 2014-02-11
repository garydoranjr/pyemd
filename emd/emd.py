"""
Wraper for C EMD implementation
"""
from c_emd import _emd

def emd(X, Y, X_weights=None, Y_weights=None, distance='euclidean', D=None):
    return _emd(1.0, 2.0)
