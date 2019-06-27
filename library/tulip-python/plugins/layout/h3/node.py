# original module can be found on https://github.com/buzzfeed/pyh3

from .h3math import Point4d


"""
The node structure storing all attributes of a tree node.
"""


class Node(object):

    """
    The node constructor.

    :param int node_id: the id of node in the node lookup table in tree object
    :param int parent_id: the id of parent node in the node lookup table in
        tree object, default None
    :param int depth: the depth of this node in the tree, deault 0
    :param int tree_size: the subtree size of the node, ie. the number of
        nodes blow this node, default 1
    :param float radius: the node's hemisphere radius, ie. the distance to its
        children, default 0
    :param float area: the node's hemisphere area, default 0
    :param int band: the band which the node's hemisphere is placed in its
        parent's hemisphere, default -1
    :param float theta: the angle of the node's hemisphere rotating around Z
        axis in spherical space, default 0
    :param float phi: the angle between the node and Z axis in spherical space,
        default 0
    :param Point4d coord: the node's 3D coordinate in cartesisan space,
        default to Point4d(0,0,0,0)
    """

    def __init__(self, node_id, parent_id=None, depth=0, tree_size=1,
                 radius=0, area=0):
        self.node_id = node_id
        self.children = set()
        self.parent = parent_id
        self.depth = depth
        self.tree_size = tree_size
        self.radius = radius
        self.area = area
        self.band = -1
        self.theta = 0
        self.phi = 0
        self.coord = Point4d()

    def __repr__(self):
        return "<{0}>".format(self.node_id)
