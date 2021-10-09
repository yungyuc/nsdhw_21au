import pytest
import _vector
import numpy as np
import math
# check invalid input
def test_invalid_input():

    tolerant=0.0001

    v1=np.array([0,0])
    v2=np.random.randint(low=-10,high=10,size=2)

    ans= _vector.Angle(v1,v2)

    assert ans == -1

# check zero angle
def test_zero_angle():

    tolerant=0.001

    
    v1=np.random.randint(low=-10,high=10,size=2)
    #avoid zero vector
    if v1[0]==0 and v1[1]==0:
        v1[0]=1
    v2=v1

    ans= _vector.Angle(v1,v2)

    assert  abs(ans-0) <= tolerant

#check 90
def test_right_angle():

    tolerant=0.001


    v1=np.array([0,1])
    v2=np.array([1,0])

    ans=_vector.Angle(v1,v2)
    assert abs(ans - math.pi/2) < tolerant

#check 180
def test_180_angle():
    tolerant=0.001

    
    v1=np.random.randint(low=-10,high=10,size=2)

    #avoid zero vector
    if v1[0]==0 and v1[1]==0:
        v1[0]=1

    v2=np.array([-1 * v1[0] , -1 * v1[1]])

    ans= _vector.Angle(v1,v2)

    assert abs(ans - math.pi) < tolerant

