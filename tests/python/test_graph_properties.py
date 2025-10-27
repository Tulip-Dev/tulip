# This file is part of Tulip (https://tulip.labri.fr)
#
# Authors: David Auber and the Tulip development Team
# from LaBRI, University of Bordeaux
#
# Tulip is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3
# of the License, or (at your option) any later version.
#
# Tulip is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.

import random
import os
import string
import sys
import unittest

from tulip import tlp

int_max = 32767
flt_max = 3.40282347e+38


def rand_int():
    return random.randint(-int_max, int_max)


def rand_float():
    return random.uniform(0, sys.float_info.max)


def rand_c_float():
    return random.uniform(0, flt_max)


def random_color():
    r = random.randint(0, 255)
    g = random.randint(0, 255)
    b = random.randint(0, 255)
    a = random.randint(0, 255)
    return tlp.Color(r, g, b, a)


def random_color_tuple():
    r = random.randint(0, 255)
    g = random.randint(0, 255)
    b = random.randint(0, 255)
    a = random.randint(0, 255)
    return (r, g, b, a)


def random_coord():
    return tlp.Coord(rand_c_float(), rand_c_float(), rand_c_float())


def random_coord_tuple():
    return (rand_c_float(), rand_c_float(), rand_c_float())


def random_coord_tuple_int():
    return (rand_int(), rand_int(), rand_int())


def random_size():
    return tlp.Size(rand_c_float(), rand_c_float(), rand_c_float())


def random_string():
    return ''.join([random.choice(string.ascii_letters + string.digits)
                    for _ in range(32)])


max_list_size = 30


def random_boolean_list(val=False):
    size = random.randint(max_list_size, max_list_size*2)
    return [val for i in range(size)]


def random_color_list():
    size = random.randint(1, max_list_size)
    return [random_color() for i in range(size)]


def random_color_tuple_list():
    size = random.randint(1, max_list_size)
    return [random_color_tuple() for i in range(size)]


def random_coord_list():
    size = random.randint(1, max_list_size)
    return [random_coord() for i in range(size)]


def random_coord_tuple_list():
    size = random.randint(1, max_list_size)
    return [random_coord_tuple() for i in range(size)]


def random_coord_tuple_int_list():
    size = random.randint(1, max_list_size)
    return [random_coord_tuple_int() for i in range(size)]


def random_double_list():
    size = random.randint(1, max_list_size)
    return [rand_float() for i in range(size)]


def random_int_list():
    size = random.randint(1, max_list_size)
    return [rand_int() for i in range(size)]


def random_size_list():
    size = random.randint(1, max_list_size)
    return [random_size() for i in range(size)]


def random_string_list():
    size = random.randint(1, max_list_size)
    return [random_string() for i in range(size)]


class TestGraphProperties(unittest.TestCase):

    def setUp(self):
        tlp.loadTulipPluginsFromDir(
            os.environ['TULIP_BUILD_DIR'] + '/plugins/import')
        self.prop_name = 'test_prop'
        self.prop = None
        self.graph = tlp.importGraph('Grid')
        self.n = self.graph.getRandomNode()
        self.e = self.graph.getRandomEdge()
        sg_nodes = [tlp.node(i) for i in range(self.graph.numberOfNodes()//2)]
        self.sub_graph = self.graph.inducedSubGraph(sg_nodes)

    def tearDown(self):
        if self.graph.existProperty(self.prop_name):
            self.graph.delLocalProperty(self.prop_name)
        self.prop = None
        self.graph = None
        self.n = None
        self.e = None

    def generic_property_test(self, prop_type, node_default_value,
                              edge_default_value, node_value, edge_value,
                              check_type_inference=True):
        # test some basic operations on property
        self.assertTrue(self.graph.existProperty(self.prop_name))
        self.assertTrue(
            isinstance(self.graph.getProperty(self.prop_name), prop_type))
        self.prop.setAllNodeValue(node_default_value)
        self.assertEqual(
            self.prop[self.graph.getRandomNode()], node_default_value)
        self.prop[self.n] = node_value
        self.prop[self.e] = edge_value
        self.assertEqual(self.prop[self.n], node_value)
        self.assertEqual(self.prop[self.e], edge_value)
        nodes_equal = list(self.prop.getNodesEqualTo(node_value))
        edges_equal = list(self.prop.getEdgesEqualTo(edge_value))
        self.assertEqual(len(nodes_equal), 1)
        self.assertEqual(len(edges_equal), 1)
        self.assertEqual(nodes_equal[0], self.n)
        self.assertEqual(edges_equal[0], self.e)

        self.prop.setAllNodeValue(node_default_value)
        self.prop.setValueToGraphNodes(node_value, self.sub_graph)
        for n in self.graph.getNodes():
            if self.sub_graph.isElement(n):
                self.assertEqual(self.prop[n], node_value)
            else:
                self.assertEqual(self.prop[n], node_default_value)

        self.prop.setAllEdgeValue(edge_default_value)
        self.prop.setValueToGraphEdges(edge_value, self.sub_graph)
        for e in self.graph.getEdges():
            if self.sub_graph.isElement(e):
                self.assertEqual(self.prop[e], edge_value)
            else:
                self.assertEqual(self.prop[e], edge_default_value)

        # ensure that an exception is correctly thrown when the property is
        # deleted from the C++ layer but there is still a reference to it
        # in the Python one
        self.graph.delLocalProperty(self.prop_name)
        self.assertFalse(self.graph.existProperty(self.prop_name))

        with self.assertRaisesRegex(
                RuntimeError, 'wrapped C/C\\+\\+ object .* has been deleted'):
            self.prop.setAllEdgeValue(edge_default_value)

        if not check_type_inference:
            return

        # test that the property can be automatically created by type inference
        self.graph[self.prop_name][self.graph.getRandomNode()] = node_value
        self.prop = self.graph[self.prop_name]
        self.assertTrue(
            isinstance(self.graph.getProperty(self.prop_name), prop_type))
        self.assertTrue(self.graph.existProperty(self.prop_name))

    def test_boolean_property(self):
        self.prop = self.graph.getBooleanProperty(self.prop_name)
        self.generic_property_test(
            tlp.BooleanProperty, True, False, False, True)

    def test_color_property(self):
        self.prop = self.graph.getColorProperty(self.prop_name)
        self.generic_property_test(
            tlp.ColorProperty, random_color(), random_color(), random_color(),
            random_color())

    def test_color_property_tuple(self):
        self.prop = self.graph.getColorProperty(self.prop_name)
        self.generic_property_test(
            tlp.ColorProperty, random_color_tuple(), random_color_tuple(),
            random_color_tuple(), random_color_tuple())

    def test_double_property(self):
        self.prop = self.graph.getDoubleProperty(self.prop_name)
        self.generic_property_test(
            tlp.DoubleProperty, rand_float(), rand_float(), rand_float(),
            rand_float())

    def test_integer_property(self):
        self.prop = self.graph.getIntegerProperty(self.prop_name)
        self.generic_property_test(
            tlp.IntegerProperty, rand_int(), rand_int(), rand_int(),
            rand_int())

    def test_layout_property(self):
        self.prop = self.graph.getLayoutProperty(self.prop_name)
        self.generic_property_test(
            tlp.LayoutProperty, random_coord(), random_coord_list(),
            random_coord(), random_coord_list())

    def test_layout_property_tuple(self):
        self.prop = self.graph.getLayoutProperty(self.prop_name)
        self.generic_property_test(
            tlp.LayoutProperty, random_coord_tuple(),
            random_coord_tuple_list(), random_coord_tuple(),
            random_coord_tuple_list())

    def test_layout_property_tuple_int(self):
        self.prop = self.graph.getLayoutProperty(self.prop_name)
        self.generic_property_test(
            tlp.LayoutProperty, random_coord_tuple_int(),
            random_coord_tuple_list(), random_coord_tuple_int(),
            random_coord_tuple_list(), check_type_inference=False)

    def test_size_property(self):
        self.prop = self.graph.getSizeProperty(self.prop_name)
        self.generic_property_test(
            tlp.SizeProperty, random_size(), random_size(), random_size(),
            random_size())

    def test_size_property_tuple_int(self):
        self.prop = self.graph.getSizeProperty(self.prop_name)
        self.generic_property_test(
            tlp.SizeProperty, random_coord_tuple_int(),
            random_coord_tuple_int(), random_coord_tuple_int(),
            random_coord_tuple_int(), check_type_inference=False)

    def test_string_property(self):
        self.prop = self.graph.getStringProperty(self.prop_name)
        self.generic_property_test(
            tlp.StringProperty, random_string(), random_string(),
            random_string(), random_string())

    def test_boolean_vector_property(self):
        self.prop = self.graph.getBooleanVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.BooleanVectorProperty, random_boolean_list(),
            random_boolean_list(), random_boolean_list(True),
            random_boolean_list(True))

    def test_color_vector_property(self):
        self.prop = self.graph.getColorVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.ColorVectorProperty, random_color_list(), random_color_list(),
            random_color_list(), random_color_list())

    def test_color_tuple_vector_property(self):
        self.prop = self.graph.getColorVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.ColorVectorProperty, random_color_tuple_list(),
            random_color_tuple_list(), random_color_tuple_list(),
            random_color_tuple_list())

    def test_coord_vector_property(self):
        self.prop = self.graph.getCoordVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.CoordVectorProperty, random_coord_list(), random_coord_list(),
            random_coord_list(), random_coord_list())

    def test_coord_tuple_vector_property(self):
        self.prop = self.graph.getCoordVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.CoordVectorProperty, random_coord_tuple_list(),
            random_coord_tuple_list(), random_coord_tuple_list(),
            random_coord_tuple_list())

    def test_coord_tuple_int_vector_property(self):
        self.prop = self.graph.getCoordVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.CoordVectorProperty, random_coord_tuple_int_list(),
            random_coord_tuple_int_list(), random_coord_tuple_int_list(),
            random_coord_tuple_int_list(), check_type_inference=False)

    def test_double_vector_property(self):
        self.prop = self.graph.getDoubleVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.DoubleVectorProperty, random_double_list(),
            random_double_list(), random_double_list(), random_double_list())

    def test_int_vector_property(self):
        self.prop = self.graph.getIntegerVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.IntegerVectorProperty, random_int_list(), random_int_list(),
            random_int_list(), random_int_list())

    def test_size_vector_property(self):
        self.prop = self.graph.getSizeVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.SizeVectorProperty, random_size_list(), random_size_list(),
            random_size_list(), random_size_list())

    def test_size_tuple_int_vector_property(self):
        self.prop = self.graph.getSizeVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.SizeVectorProperty, random_coord_tuple_int_list(),
            random_coord_tuple_int_list(), random_coord_tuple_int_list(),
            random_coord_tuple_int_list(), check_type_inference=False)

    def test_string_vector_property(self):
        self.prop = self.graph.getStringVectorProperty(self.prop_name)
        self.generic_property_test(
            tlp.StringVectorProperty, random_string_list(),
            random_string_list(), random_string_list(), random_string_list())
