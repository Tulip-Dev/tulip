# original module can be found on https://github.com/buzzfeed/pyh3

import logging
import math
from collections import deque
from .node import Node
from operator import itemgetter
from .h3math import compute_radius, compute_hyperbolic_area, compute_delta_phi, compute_delta_theta

"""
Customized exception for invalid edge input. 
"""


class InvalidArgument(Exception):
    pass


"""
The tree structure storing all nodes and edges, and also provide easy node lookup. 
"""


class Tree(object):

    """
    The Tree class is the entry point for this application. It's only argument is an iterable edgelist. This can be a
        generator, list, tuple, etc. Each edge should be of the format (parent_id, child_id). They should be int types
        (long is fine) and they should be consecutive. The edgelist must form a single connected Tree. If that is not
        the case you'll need to prune your graph to make this the case. A common operation for displaying otherwise
        cyclic graphs is to simply plot the minimum spanning tree.

    :param edges: a tuple for a tree edge as (parent, child). This is required.
    :type edges: iterable(tuple(int, int))
    """
    def __init__(self, edges=None):
        self.nodes = {}
        self.height = 0
        self.root = None

        for parent_id, child_id in edges:
            self.insert_edge(child_id, parent_id)

        edges = None # Release that memory.

        for id, node in self.nodes.items():
            if node.parent is None:
                self.root = node
                break

        self.__label_node_generations()
        self.set_subtree_radius()
        self.set_subtree_size()
        self.sort_children_by_radius()
        self.set_placement()


    def insert_edge(self, parent_id, child_id):
        """
        Insert edge(parent, child) pair into the tree. No assumptions are made about the structure except that a Node
         can have exactly 1 parent, and each child can only occur once. Otherwise; cyclic graphs can be input. This will
         probably cause your layout to run forever.

        :param int parent_id: The id of the parent node.
        :param int child_id: The id of the child node.

        :returns: A tuple of the parent and child nodes as Node objects.
        """
        parent = self.nodes.get(parent_id, Node(parent_id))
        child = self.nodes.get(child_id, Node(child_id))

        child.parent = parent
        parent.children.add(child)

        self.nodes[parent_id] = parent
        self.nodes[child_id] = child

        return parent, child


    def get_leaf_nodes(self):
        """
        Gets the node ids for all leaf nodes in the Tree.

        :returns: A generator of all the leaf nodes in the Tree
        """
        for node_id, node in self.nodes.items():
            if not node.children:
                yield node.node_id


    def print_tree(self):
        """
        Print the tree sorted by depth to log, including the following parameters.
        The tree is traversed in a breath-first-search.
        """
        current_generation = deque([self.root.node_id])
        next_generation = True
        while next_generation:
            next_generation = deque()
            while current_generation:
                node_id = current_generation.popleft()
                logging.info(
                    "{0}, parent: {1}, depth: {2}, #children: {3}, size: {4}, radius: {5}, area: {6}"
                    .format(node_id,
                            self.nodes[node_id].parent,
                            self.nodes[node_id].depth,
                            len(self.nodes[node_id].children),
                            self.nodes[node_id].tree_size,
                            self.nodes[node_id].radius,
                            self.nodes[node_id].area))
                for child in self.nodes[node_id].children:
                    next_generation.append(child.node_id)
            current_generation = next_generation


    def __label_node_generations(self, depth=0):
        """
        Decorate each node with it's depth in the tree.

        :param int depth: the initial depth value for the root, default 0
        """
        current_generation = deque([self.root.node_id])
        next_generation = True
        while next_generation:
            next_generation = deque()
            while current_generation:
                node_id = current_generation.popleft()
                self.nodes[node_id].depth = depth
                for child in self.nodes[node_id].children:
                    next_generation.append(child.node_id)
            depth += 1
            current_generation = next_generation
        self.height = depth - 1


    def set_subtree_radius(self):
        """
        Set the node's hemisphere radius and also its distance from its children. The radius is calculated
        recursively from the leaf nodes, with a unit hemisphere size, tracing back to the root. The area
        calculation is an approximation from a disc of the bottom of a child hemisphere to a spherical cap.
        The recusion requires tracing the tree from the last generation to root so that all the nodes radii
        have been calculated before their parent's radius is calcualted. As the hemisphere sizes are tightly
        calculated but placing them loosely to the parent hemisphere, the space reservation is 7.2 times of
        the actual size of a child hemisphere.

        """
        leaf_nodes = self.get_leaf_nodes()
        outermost_non_leaf = set()
        for n in leaf_nodes:
            N = len(self.nodes[self.nodes[n].parent.node_id].children)
            self.nodes[n].radius = compute_radius(0.0025)
            logging.info("leaf node {0}, parent {1}, radius {2}"
                         .format(n, self.nodes[n].parent, self.nodes[n].radius))
            outermost_non_leaf.add(self.nodes[n].parent.node_id)
        depth = self.height - 1
        current_generation = deque(list(set(n.node_id for n in outermost_non_leaf
                                            if self.nodes[n].parent is not None
                                            if self.nodes[self.nodes[n].parent.node_id].depth == depth)))
        previous_generation = True
        while previous_generation:
            previous_generation = deque()
            while current_generation:
                n = current_generation.popleft()
                if self.nodes[n].area == 0:  # avoid duplicate parents
                    if self.nodes[n].parent is not None:
                        previous_generation.append(self.nodes[n].parent.node_id)
                    for child in self.nodes[n].children:
                        self.nodes[n].area += 7.2 * compute_hyperbolic_area(self.nodes[child.node_id].radius)
                        logging.info("node {0}, child {1}, child_area+ {2}, radius {3}, area {4}"
                                     .format(n, child, compute_hyperbolic_area(self.nodes[child.node_id].radius),
                                             self.nodes[child.node_id].radius, self.nodes[child.node_id].area))
                    self.nodes[n].radius = compute_radius(self.nodes[n].area)
                    logging.info("---> node {0}, radius {1}, area {2}"
                                 .format(n, self.nodes[n].radius, self.nodes[n].area))
            for n in outermost_non_leaf:
                if n is not None:
                    if self.nodes[n].depth == depth:
                        previous_generation.append(n)
            depth -= 1
            current_generation = deque(list(set(previous_generation)))


    def set_subtree_size(self):
        """
        Set the subtree size by the number of nodes in its subtree.
        """
        leaf_nodes = self.get_leaf_nodes()
        depth = self.height
        current_generation = deque(list(n for n in leaf_nodes
                                        if self.nodes[n].depth == depth))
        previous_generation = True
        while previous_generation:
            depth -= 1
            previous_generation = deque()
            while current_generation:
                n = current_generation.popleft()
                if self.nodes[n].parent is not None:
                    previous_generation.append(self.nodes[n].parent.node_id)
                    self.nodes[self.nodes[n].parent.node_id].tree_size += \
                        self.nodes[n].tree_size
            for n in leaf_nodes:
                if self.nodes[n].depth == depth:
                    previous_generation.append(n)
            current_generation = deque(list(set(previous_generation)))


    def sort_children_by_radius(self):
        """
        Sort the nodes in decreasing order in the same depth by their radii, in place sort is used.
        The tree is traversed in a breath-first-search.
        """
        depth = 0
        current_generation = deque([self.root.node_id])
        next_generation = True
        while next_generation:
            next_generation = deque()
            while current_generation:
                node_id = current_generation.popleft()
                for child in self.nodes[node_id].children:
                    next_generation.append(child.node_id)
                child_size_pair = [[child, self.nodes[child.node_id].radius]
                                   for child in self.nodes[node_id].children]
                child_size_pair.sort(key=itemgetter(1), reverse=True)
                if child_size_pair:
                    self.nodes[node_id].children = list(zip(*child_size_pair))[0]
            depth += 1
            current_generation = next_generation


    def sort_children_by_tree_size(self):
        """
        Sort the nodes in decreasing order in the same depth by their number of nodes in subtree,
        in place sort is used.  This is an alternative option to sort the tree before placing the
        nodes on the hemisphere. The original H3 algorithem set leaf node radius as math:: N / alpha
        so the nodes with many sibilings can have a larger radius and nodes with a lot of children.
        """
        depth = 0
        current_generation = deque([self.root])
        next_generation = True
        while next_generation:
            next_generation = deque()
            while current_generation:
                node_id = current_generation.popleft()
                for child in self.nodes[node_id].children:
                    next_generation.append(child)
                child_size_pair = [[child, self.nodes[child].tree_size]
                                   for child in self.nodes[node_id].children]
                child_size_pair.sort(key=itemgetter(1), reverse=True)
                if child_size_pair:
                    self.nodes[node_id].children = list(zip(*child_size_pair))[0]
            depth += 1
            current_generation = next_generation


    def set_placement(self):
        """
        Placing the hemispheres on the root hemisphere. Start from the pole, placing the largest child
        hemisphere and then placing smaller hemispheres around the pole. When the hemispheres fully filled
        one band, start placing on the next band until fully filled again.
        Node:
            - Placing the 1st hemisphere and its phi is zero, could lead to a ZeroDivisionError exception
              so we set its value as a very small number (0.000001)
            - Don't forget to reserve space for the other half of the hemisphere, both the right half and
              the lower half. Add theta by delta_theta after placing each node. Add phi by the max
              delta_phi in the last band before placing hemispheres to the next band
            - Each subtree is independent from each other, so if the new node has a different parent node
              than the previous parent node, we know this node is in a different subtree and initialize
              phi, theta, dealta_theta and band and placing nodes all over again.
        """
        depth = 0
        current_generation = deque([n.node_id for n in self.nodes[self.root.node_id].children])
        next_generation = True
        last_parent = self.root
        while next_generation:
            next_generation = deque()
            phi, theta, delta_theta, band = 0.000001, 0., 0., 1
            last_max_phi = 0  # span phi before jumping to the next band
            while current_generation:
                node = current_generation.popleft()
                if self.nodes[node].parent != last_parent:  # same gen, diff parent
                    last_parent = self.nodes[node].parent
                    phi, theta, delta_theta, band = 0.000001, 0., 0., 1
                rp = self.nodes[self.nodes[node].parent.node_id].radius
                try:
                    if phi == 0.000001:  # first child of root
                        phi += compute_delta_phi(self.nodes[node].radius, rp)
                        self.nodes[node].band = 0
                    else:
                        delta_theta = compute_delta_theta(self.nodes[node].radius, rp, phi)
                        if (theta + delta_theta) <= 2 * math.pi:
                            theta += delta_theta
                            if last_max_phi:
                                last_max_phi = compute_delta_phi(self.nodes[node].radius, rp)
                                phi += compute_delta_phi(self.nodes[node].radius, rp)
                        else:
                            band += 1
                            theta = delta_theta
                            phi += last_max_phi + compute_delta_phi(self.nodes[node].radius, rp)
                            last_max_phi = 0
                        self.nodes[node].band = band
                        self.nodes[node].theta = theta
                        self.nodes[node].phi = phi
                except ZeroDivisionError as e:
                    logging.error("{0}\n node {1}, radius={2}, rp={3}, phi={4}, parent={5}"
                                  .format(e, node, self.nodes[node].radius, rp, phi,
                                          self.nodes[node].parent))
                self.nodes[node].coord.sph_to_cart(self.nodes[node].theta,
                                                   self.nodes[node].phi,
                                                   self.nodes[self.nodes[node].parent.node_id].radius)
                if self.nodes[node].parent != self.root.node_id:
                    self.nodes[node].coord.coordinate_transformation(
                        self.nodes[self.nodes[node].parent.node_id].theta,
                        self.nodes[self.nodes[node].parent.node_id].phi)
                    self.nodes[node].coord.cart_offset(self.nodes[self.nodes[node].parent.node_id].coord)
                logging.info("node {0}, radius {1},  band {2}, theta {3}, phi {4}"
                             .format(node, self.nodes[node].radius, self.nodes[node].band,
                                     self.nodes[node].theta, self.nodes[node].phi))
                logging.info("node {0}, x {1}, y {2}, z {3}, w {4}"
                             .format(node, self.nodes[node].coord.x, self.nodes[node].coord.y,
                                     self.nodes[node].coord.z, self.nodes[node].coord.w))
                theta += delta_theta    # reserve space for the other half sphere
                for child in self.nodes[node].children:
                    next_generation.append(child.node_id)
            depth += 1
            current_generation = next_generation

    def get_coordinates(self):
        """
        Returns a generator of all the coordinates for this plot. This can be used to build SVGs or whatever.

        :return: A generator of tuples of (x, y, z) representing the position of each node in 3-d space.
        """
        current_generation = deque([self.root.node_id])
        next_generation = True
        while next_generation:
            next_generation = deque()
            while current_generation:
                node_id = current_generation.popleft()
                yield (self.nodes[node_id].coord.x,
                       self.nodes[node_id].coord.y,
                       self.nodes[node_id].coord.z)
                for child in self.nodes[node_id].children:
                    next_generation.append(child.node_id)
            current_generation = next_generation
