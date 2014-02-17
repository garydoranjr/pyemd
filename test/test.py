import numpy as np

from emd import emd

X = np.array([[0.0, 10.0],
              [1.0, 0.0]])

Y = np.array([[ 0.0, -1.0],
              [-1.0,  0.0]])

print emd(X, Y)
