# original module can be found on https://github.com/buzzfeed/pyh3

import math
import numpy as np

"""
Scaling factor for compute_radius(), compute_hyperbolic_area(), 
compute_delta_theta() and compute_delta_phi().
"""
K = 2.0


"""
The 3D coordinate structure
"""


class Point4d(object):

    """
    The constructor for Point4d

    :param float x: the x coordinate for a node in cartesian space
    :param float y: the y coordinate for a node in cartesian space
    :param float z: the z coordinate for a node in cartesian space
    """

    def __init__(self, x=0.0, y=0.0, z=0.0, w=1.0):
        self.x = x
        self.y = y
        self.z = z
        self.w = w

    """
    Transform coodinate from spherical space to cartesian space

    :param float theta: the theta coordinate for a node in spherical space, polar angle
    :param float phi: the phi coordinate for a node in spherical space, elevation angle
    :param float r: the radius for a node in spherical space, radial distance
    """

    def sph_to_cart(self, theta, phi, r):
        self.x = r * math.sin(phi) * math.cos(theta)
        self.y = r * math.sin(phi) * math.sin(theta)
        self.z = r * math.cos(phi)

    """
    Add an offset to a node's coordinate in cartesian space

    :param Point4d offset: the offset to be added to current node's coordinate
    """

    def cart_offset(self, offset):
        self.x += offset.x
        self.y += offset.y
        self.z += offset.z

    """
    Translate node's coordinate in cartesian space by translation matrix

    :param Point4d offset: the offset to be added to current node's coordinate
    """

    def translate(self, offset):
        translation_matrix = np.array([[1, 0, 0, offset.x],
                                       [0, 1, 0, offset.y],
                                       [0, 0, 1, offset.z],
                                       [0, 0, 0, 1]])
        target = translation_matrix.dot(np.array([self.x, self.y, self.z, self.w]))
        self.x, self.y, self.z, self.w = target[0], target[1], target[2], target[3]

    """
    Transform node's coordinate in cartesian space by transformation matrix by 
    spherical space coordinate values

    :param float theta: the theta coordinate for a node in spherical space, polar angle
    :param float phi: the phi coordinate for a node in spherical space, elevation angle
    """

    def coordinate_transformation(self, theta, phi):
        rotation_matrix = rotation_matrix_z(theta).dot(rotation_matrix_y(phi))
        target = rotation_matrix.dot(np.array([self.x, self.y, self.z, self.w]))
        self.x, self.y, self.z, self.w = target[0], target[1], target[2], target[3]

"""
Compute the node's hemisphere radius by its hemisphere space reserved

:returns: return the node's radius as a float
"""


def compute_radius(H_p):
    return K * math.asinh(math.sqrt(H_p / (2 * math.pi * K * K)))


"""
Compute the parent node's hemisphere radius by its child's hemisphere radius
:returns: return the node's hemisphere space reservation as a float
"""


def compute_hyperbolic_area(radius):
    beta = 1.00
    return 2 * math.pi * (math.cosh(radius / K) - 1.0) * beta


"""
Compute the proper delta variant value for node's hemisphere placement, similar to 
the space reservation the size of the node's hemisphere radius

:returns: return the node's half hemisphere space reservation as angle delta_theta 
          in spherical space as a float number
"""


def compute_delta_theta(r, rp, phi):
    return math.atan(math.tanh(r / K) /
                     (math.sinh(rp / K) * math.sinh(phi)))


"""
Compute the proper phi variant value for a band of nodes' hemisphere placement, 
similar to the space reservation the size of the largest node's hemisphere radius
of the whole band

:returns: return the nodes' max half hemisphere space reservation in the same band
          as angle delta_phi in spherical space as a float number
"""


def compute_delta_phi(r, rp):
    return math.atan(math.tanh(r / K) / math.sinh(rp / K))

"""
The Klein metric for visualizing hyperbolic space: unusual uses of 4x4 matrices 
by Phillips and Gunn
"""


def minkowski(x, y):
    return (x.x * y.x + x.y * y.y + x.z * y.z - x.w * y.w)

"""
Calculate 3D hyperbolic distance as a Klein metric
"""


def hyperbolic_distance(x, y):
    t1 = minkowski(x, y)
    t2 = minkowski(x, x)
    t3 = minkowski(y, y)
    return (2 * math.acosh(((t1 * t1) / (t2 * t3))**2))

"""
Rotation matrix around X axis

:params: the angle for rotating around X axis
:returns: return the rotation matrix around X axis by the given angle
"""


def rotation_matrix_x(angle):
    return np.array([[1, 0, 0, 0],
                     [0, math.cos(angle), -1 * math.sin(angle), 0],
                     [0, math.sin(angle), math.cos(angle), 0],
                     [0, 0, 0, 1]])


"""
Rotation matrix around Y axis

:params: the angle for rotating around Y axis
:returns: return the rotation matrix around Y axis by the given angle
"""


def rotation_matrix_y(angle):
    return np.array([[math.cos(angle), 0, math.sin(angle), 0],
                     [0, 1, 0, 0],
                     [-1 * math.sin(angle), 0, math.cos(angle), 0],
                     [0, 0, 0, 1]])


"""
Rotation matrix around Z axis

:params: the angle for rotating around Z axis
:returns: return the rotation matrix around X axis by the given angle
"""


def rotation_matrix_z(angle):
    return np.array([[math.cos(angle), -1 * math.sin(angle), 0, 0],
                     [math.sin(angle), math.cos(angle), 0, 0],
                     [0, 0, 1, 0],
                     [0, 0, 0, 1]])
