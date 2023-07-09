##############################################
# The MIT License (MIT)
# Copyright (c) 2016 Kevin Walchko
# see LICENSE for full details
##############################################
import numpy as np

def get_ideal(orient, num):
    """
    Given an orientation, calculate the ideal local gravity vector
    """
    g = 1.0
    if orient == "x-up":
        nn=np.array([g,0,0]*num).reshape(num,3)
    elif orient == "x-down":
        nn=np.array([-g,0,0]*num).reshape(num,3)
    elif orient == "y-up":
        nn=np.array([0,g,0]*num).reshape(num,3)
    elif orient == "y-down":
        nn=np.array([0,-g,0]*num).reshape(num,3)
    elif orient == "z-up":
        nn=np.array([0,0,g]*num).reshape(num,3)
    elif orient == "z-down":
        nn=np.array([0,0,-g]*num).reshape(num,3)
    else:
        raise Exception("Invalid orientation")

    return nn

def least_squares_fit(A, B):
    """B = A*x which is: ideal = noisy*x => [n,3] = [n,4][4,3]
    ideal = [n,3] = [x,y,z]
    noisy = [n,4] = [x,y,z,1]: the 1 accounts for the biases
    A = [4,3]: this is a bunch of coefficient matrices [[S],[H]]
      S = [3,3]
      H = [1,3]
    """
    # Need to extend A and B
    # A = np.hstack((A, np.ones(len(A)).reshape(-1, 1)))
    # print(A)

    X, res, rank, s = np.linalg.lstsq(A, B, rcond=None)
    print(f"Rank: {rank}")
    print(f"Singular values: {s}")
    print(f"Sum residual error: {np.sum(res)}")
    return X

def accelcal(noisey, axisOrder):
    sz = noisey.shape
    noisey = np.concatenate((noisey, np.ones((sz[0],1))), axis=1)
    ideal = None
    for axis in axisOrder:
        i = get_ideal(axis, sz[0]//6)
        if ideal is None:
            ideal = i
        else:
            ideal = np.concatenate((ideal, i), axis=0)

    xx = least_squares_fit(noisey, ideal)
    print("--------------------------------")
    print(xx)
    return xx

def correct(noisey, A):
    """
    accel = [noisey| 1].dot(A)
    [Nx3] = [Nx4]*[4x3]
    """
    sz = noisey.shape
    a = np.concatenate((noisey, np.ones((sz[0],1))), axis=1)
    return a.dot(A)
