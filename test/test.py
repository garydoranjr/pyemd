import numpy as np

from emd import emd

X = np.array([[0.0, 10.0],
              [1.0, 0.0]])

Y = np.array([[ 0.0, -1.0],
              [-1.0,  0.0]])

Z = np.array([[ 0.0, -1.0],
              [-1.0, 0.0],
              [0.5, 5.0]])

print(emd(X, Y))

dist, flows = emd(X, Y, return_flows=True)
print(flows)

dist, flows = emd(X, Z, return_flows=True)
print(flows)
