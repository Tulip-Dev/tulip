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

import unittest
import sys

from tulip import tlp

import tulipplugins

plugin_name = 'Test Algorithm'
plugin_author = 'author'
plugin_date = 'date'
plugin_info = 'info'
plugin_version = 'version'
plugin_group = 'group'

boolean_param_name = 'bool'
boolean_param_default_value = True
boolean_param_value = False

int_param_name = 'int'
int_param_default_value = 5
int_param_value = -10

float_param_name = 'float'
float_param_default_value = 53.35
float_param_value = 156.876

string_param_name = 'string'
string_param_default_value = 'foo'
string_param_value = 'bar'

boolean_prop_param_name = 'bool_prop'
boolean_prop_param_default_value = 'viewSelection'
boolean_prop_param_value = 'prop_bool'

color_prop_param_name = 'color_prop'
color_prop_param_default_value = 'viewColor'
color_prop_param_value = 'prop_color'

double_prop_param_name = 'double_prop'
double_prop_param_default_value = 'viewMetric'
double_prop_param_value = 'color_prop'

int_prop_param_name = 'int_prop'
int_prop_param_default_value = 'viewShape'
int_prop_param_value = 'prop_int'

layout_prop_param_name = 'layout_prop'
layout_prop_param_default_value = 'viewLayout'
layout_prop_param_value = 'prop_layout'

size_prop_param_name = 'size_prop'
size_prop_param_default_value = 'viewSize'
size_prop_param_value = 'prop_size'

string_prop_param_name = 'string_prop'
string_prop_param_default_value = 'viewLabel'
string_prop_param_value = 'prop_string'

boolean_vec_prop_param_name = 'bool_vec_prop'
boolean_vec_prop_param_default_value = 'bool_vector'

color_vec_prop_param_name = 'color_vec_prop'
color_vec_prop_param_default_value = 'color_vector'

double_vec_prop_param_name = 'double_vec_prop'
double_vec_prop_param_default_value = 'double_vector'

int_vec_prop_param_name = 'int_vec_prop'
int_vec_prop_param_default_value = 'int_vector'

coord_vec_prop_param_name = 'coord_vec_prop'
coord_vec_prop_param_default_value = 'coord_vector'

size_vec_prop_param_name = 'size_vec_prop'
size_vec_prop_param_default_value = 'size_vector'

string_vec_prop_param_name = 'string_vec_prop'
string_vec_prop_param_default_value = 'string_vector'

color_param_name = 'color'
color_param_default_value = '(128,128,128,128)'
color_param_value = tlp.Color.Blue
color_param_tuple_value = (0, 0, 255)

color_scale_param_name = 'color_scale'
color_scale_param_default_value = '((255,0,0,255),(0,255,0,255),(0,0,255,255))'
color_scale_default_value_dict = {0.0: (255, 0, 0, 255),
                                  0.5: (0, 255, 0, 255),
                                  1.0: (0, 0, 255, 255)}
color_scale_param_value_dict = {0.0: (255, 0, 255, 255),
                                0.5: (128, 255, 0, 255),
                                1.0: (128, 0, 255, 255)}
color_scale_param_value_list = [(255, 0, 255, 255), (128, 255, 0, 255),
                                (128, 0, 255, 255)]
color_scale_param_value = tlp.ColorScale(color_scale_param_value_dict)

dir_param_name = 'dir'
dir_param_default_value = '/tmp'
dir_param_value = '/home/toto'

file_param_name = 'file'
file_param_default_value = '/tmp/foo.csv'
file_param_value = '/home/toto/bar.csv'

string_collection_param_name = 'string collection'
string_collection_param_default_value = 'foo;bar'
string_collection_param_value = 'bar'

out_boolean_param_name = 'bool_out'
out_boolean_param_value = True
out_int_param_name = 'int_out'
out_int_param_value = 2
out_float_param_name = 'float_out'
out_float_param_value = 0.5
out_string_param_name = 'string_out'
out_string_param_value = 'toto'

parameters_value = {}


class TestAlgorithm(tlp.Algorithm):

    def __init__(self, context):
        tlp.Algorithm.__init__(self, context)

        self.addBooleanParameter(
            boolean_param_name,
            defaultValue=str(boolean_param_default_value))

        self.addColorParameter(
            color_param_name,
            defaultValue=color_param_default_value)

        self.addColorScaleParameter(
            color_scale_param_name,
            defaultValue=color_scale_param_default_value)

        self.addIntegerParameter(
            int_param_name,
            defaultValue=str(int_param_default_value))

        self.addFloatParameter(
            float_param_name,
            defaultValue=str(float_param_default_value))

        self.addStringParameter(
            string_param_name,
            defaultValue=str(string_param_default_value))

        self.addDirectoryParameter(
            dir_param_name,
            defaultValue=dir_param_default_value)

        self.addFileParameter(
            file_param_name,
            defaultValue=file_param_default_value)

        self.addStringCollectionParameter(
            string_collection_param_name,
            defaultValue=string_collection_param_default_value)

        self.addBooleanPropertyParameter(
            boolean_prop_param_name,
            defaultValue=boolean_prop_param_default_value)

        self.addColorPropertyParameter(
            color_prop_param_name,
            defaultValue=color_prop_param_default_value)

        self.addDoublePropertyParameter(
            double_prop_param_name,
            defaultValue=double_prop_param_default_value)

        self.addIntegerPropertyParameter(
            int_prop_param_name,
            defaultValue=int_prop_param_default_value)

        self.addLayoutPropertyParameter(
            layout_prop_param_name,
            defaultValue=layout_prop_param_default_value)

        self.addSizePropertyParameter(
            size_prop_param_name,
            defaultValue=size_prop_param_default_value)

        self.addStringPropertyParameter(
            string_prop_param_name,
            defaultValue=string_prop_param_default_value)

        self.addBooleanVectorPropertyParameter(
            boolean_vec_prop_param_name,
            defaultValue=boolean_vec_prop_param_default_value)

        self.addColorVectorPropertyParameter(
            color_vec_prop_param_name,
            defaultValue=color_vec_prop_param_default_value)

        self.addDoubleVectorPropertyParameter(
            double_vec_prop_param_name,
            defaultValue=double_vec_prop_param_default_value)

        self.addIntegerVectorPropertyParameter(
            int_vec_prop_param_name,
            defaultValue=int_vec_prop_param_default_value)

        self.addCoordVectorPropertyParameter(
            coord_vec_prop_param_name,
            defaultValue=coord_vec_prop_param_default_value)

        self.addSizeVectorPropertyParameter(
            size_vec_prop_param_name,
            defaultValue=size_vec_prop_param_default_value)

        self.addStringVectorPropertyParameter(
            string_vec_prop_param_name,
            defaultValue=string_vec_prop_param_default_value)

        self.addBooleanParameter(out_boolean_param_name, outParam=True)
        self.addIntegerParameter(out_int_param_name, outParam=True)
        self.addFloatParameter(out_float_param_name, outParam=True)
        self.addStringParameter(out_string_param_name, outParam=True)

    def check(self):
        return (True, 'Ok')

    def run(self):
        p = parameters_value
        d = self.dataSet
        p[boolean_param_name] = d[boolean_param_name]
        p[int_param_name] = d[int_param_name]
        p[float_param_name] = d[float_param_name]
        p[string_param_name] = d[string_param_name]
        p[string_collection_param_name] = d[string_collection_param_name]

        p[color_param_name] = tlp.Color(d[color_param_name])
        p[color_scale_param_name] = tlp.ColorScale(d[color_scale_param_name])

        p[boolean_prop_param_name] = d[boolean_prop_param_name]
        p[color_prop_param_name] = d[color_prop_param_name]
        p[double_prop_param_name] = d[double_prop_param_name]
        p[int_prop_param_name] = d[int_prop_param_name]
        p[layout_prop_param_name] = d[layout_prop_param_name]
        p[size_prop_param_name] = d[size_prop_param_name]
        p[string_prop_param_name] = d[string_prop_param_name]
        p[boolean_vec_prop_param_name] = d[boolean_vec_prop_param_name]
        p[color_vec_prop_param_name] = d[color_vec_prop_param_name]
        p[double_vec_prop_param_name] = d[double_vec_prop_param_name]
        p[int_vec_prop_param_name] = d[int_vec_prop_param_name]
        p[coord_vec_prop_param_name] = d[coord_vec_prop_param_name]
        p[size_vec_prop_param_name] = d[size_vec_prop_param_name]
        p[string_vec_prop_param_name] = d[string_vec_prop_param_name]

        d[out_boolean_param_name] = out_boolean_param_value
        d[out_int_param_name] = out_int_param_value
        d[out_float_param_name] = out_float_param_value
        d[out_string_param_name] = out_string_param_value

        return True


tulipplugins.registerPluginOfGroup(
    'TestAlgorithm', plugin_name, plugin_author,
    plugin_date, plugin_info, plugin_version, plugin_group)


class TestPluginParameters(unittest.TestCase):

    def setUp(self):
        self.graph = tlp.newGraph()

        self.boolean_prop = self.graph.getBooleanProperty(
            boolean_prop_param_value)

        self.color_prop = self.graph.getColorProperty(
            color_prop_param_value)

        self.double_prop = self.graph.getDoubleProperty(
            double_prop_param_value)

        self.int_prop = self.graph.getIntegerProperty(
            int_prop_param_value)

        self.layout_prop = self.graph.getLayoutProperty(
            layout_prop_param_value)

        self.size_prop = self.graph.getSizeProperty(size_prop_param_value)

        self.string_prop = self.graph.getStringProperty(
            string_prop_param_value)

        self.boolean_vec_prop = self.graph.getBooleanVectorProperty(
            boolean_vec_prop_param_default_value)

        self.color_vec_prop = self.graph.getColorVectorProperty(
            color_vec_prop_param_default_value)

        self.double_vec_prop = self.graph.getDoubleVectorProperty(
            double_vec_prop_param_default_value)

        self.int_vec_prop = self.graph.getIntegerVectorProperty(
            int_vec_prop_param_default_value)

        self.coord_vec_prop = self.graph.getCoordVectorProperty(
            coord_vec_prop_param_default_value)

        self.size_vec_prop = self.graph.getSizeVectorProperty(
            size_vec_prop_param_default_value)

        self.string_vec_prop = self.graph.getStringVectorProperty(
            string_vec_prop_param_default_value)

    def tearDown(self):
        self.graph = None

    def test_plugin_is_registered(self):
        self.assertIn(plugin_name, tlp.getAlgorithmPluginsList())
        plugin = tlp.PluginLister.pluginInformation(plugin_name)
        self.assertEqual(plugin.category(), 'Algorithm')
        self.assertEqual(plugin.name(), plugin_name)
        self.assertEqual(plugin.author(), plugin_author)
        self.assertEqual(plugin.date(), plugin_date)
        self.assertEqual(plugin.info(), plugin_info)
        self.assertEqual(plugin.release(), plugin_version)
        self.assertEqual(plugin.group(), plugin_group)
        self.assertEqual(plugin.programmingLanguage(), 'Python')

    def test_plugin_default_parameters(self):
        plugin_default_params = tlp.getDefaultPluginParameters(plugin_name,
                                                               self.graph)

        self.assertIn(boolean_param_name, plugin_default_params)
        self.assertEqual(type(plugin_default_params[boolean_param_name]), bool)
        self.assertEqual(plugin_default_params[boolean_param_name],
                         boolean_param_default_value)

        self.assertIn(color_param_name, plugin_default_params)

        self.assertTrue(isinstance(plugin_default_params[color_param_name],
                        tlp.Color))

        self.assertEqual(repr(plugin_default_params[color_param_name]),
                         color_param_default_value)

        self.assertIn(color_scale_param_name,
                      plugin_default_params)

        self.assertTrue(
            isinstance(plugin_default_params[color_scale_param_name],
                       tlp.ColorScale))

        self.assertEqual(plugin_default_params[color_scale_param_name],
                         tlp.ColorScale(color_scale_default_value_dict))

        self.assertIn(int_param_name, plugin_default_params)
        self.assertEqual(type(plugin_default_params[int_param_name]), int)
        self.assertEqual(plugin_default_params[int_param_name],
                         int_param_default_value)

        self.assertIn(float_param_name, plugin_default_params)
        self.assertEqual(type(plugin_default_params[float_param_name]), float)
        self.assertEqual(plugin_default_params[float_param_name],
                         float_param_default_value)

        self.assertIn(string_param_name, plugin_default_params)
        self.assertEqual(type(plugin_default_params[string_param_name]), str)
        self.assertEqual(plugin_default_params[string_param_name],
                         string_param_default_value)

        self.assertIn(string_collection_param_name, plugin_default_params)
        self.assertEqual(
            type(plugin_default_params[string_collection_param_name]), str)
        self.assertEqual(plugin_default_params[string_collection_param_name],
                         string_collection_param_default_value.split(';')[0])

        self.assertIn(boolean_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[boolean_prop_param_name],
                       tlp.BooleanProperty))
        self.assertEqual(
            plugin_default_params[boolean_prop_param_name].getName(),
            boolean_prop_param_default_value)

        self.assertIn(color_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[color_prop_param_name],
                       tlp.ColorProperty))
        self.assertEqual(
            plugin_default_params[color_prop_param_name].getName(),
            color_prop_param_default_value)

        self.assertIn(double_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[double_prop_param_name],
                       tlp.DoubleProperty))
        self.assertEqual(
            plugin_default_params[double_prop_param_name].getName(),
            double_prop_param_default_value)

        self.assertIn(int_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[int_prop_param_name],
                       tlp.IntegerProperty))
        self.assertEqual(
            plugin_default_params[int_prop_param_name].getName(),
            int_prop_param_default_value)

        self.assertIn(layout_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[layout_prop_param_name],
                       tlp.LayoutProperty))
        self.assertEqual(
            plugin_default_params[layout_prop_param_name].getName(),
            layout_prop_param_default_value)

        self.assertIn(size_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[size_prop_param_name],
                       tlp.SizeProperty))
        self.assertEqual(
            plugin_default_params[size_prop_param_name].getName(),
            size_prop_param_default_value)

        self.assertIn(string_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[string_prop_param_name],
                       tlp.StringProperty))
        self.assertEqual(
            plugin_default_params[string_prop_param_name].getName(),
            string_prop_param_default_value)

        self.assertIn(boolean_vec_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[boolean_vec_prop_param_name],
                       tlp.BooleanVectorProperty))
        self.assertEqual(
            plugin_default_params[boolean_vec_prop_param_name].getName(),
            boolean_vec_prop_param_default_value)

        self.assertIn(color_vec_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[color_vec_prop_param_name],
                       tlp.ColorVectorProperty))
        self.assertEqual(
            plugin_default_params[color_vec_prop_param_name].getName(),
            color_vec_prop_param_default_value)

        self.assertIn(double_vec_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[double_vec_prop_param_name],
                       tlp.DoubleVectorProperty))
        self.assertEqual(
            plugin_default_params[double_vec_prop_param_name].getName(),
            double_vec_prop_param_default_value)

        self.assertIn(int_vec_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[int_vec_prop_param_name],
                       tlp.IntegerVectorProperty))
        self.assertEqual(
            plugin_default_params[int_vec_prop_param_name].getName(),
            int_vec_prop_param_default_value)

        self.assertIn(coord_vec_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[coord_vec_prop_param_name],
                       tlp.CoordVectorProperty))
        self.assertEqual(
            plugin_default_params[coord_vec_prop_param_name].getName(),
            coord_vec_prop_param_default_value)

        self.assertIn(size_vec_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[size_vec_prop_param_name],
                       tlp.SizeVectorProperty))
        self.assertEqual(
            plugin_default_params[size_vec_prop_param_name].getName(),
            size_vec_prop_param_default_value)

        self.assertIn(string_vec_prop_param_name, plugin_default_params)
        self.assertTrue(
            isinstance(plugin_default_params[string_vec_prop_param_name],
                       tlp.StringVectorProperty))
        self.assertEqual(
            plugin_default_params[string_vec_prop_param_name].getName(),
            string_vec_prop_param_default_value)

    def test_plugin_in_parameters(self):
        plugin_params = tlp.getDefaultPluginParameters(plugin_name, self.graph)

        plugin_params[boolean_param_name] = boolean_param_value
        plugin_params[int_param_name] = int_param_value
        plugin_params[float_param_name] = float_param_value
        plugin_params[string_param_name] = string_param_value
        plugin_params[
            string_collection_param_name] = string_collection_param_value
        plugin_params[color_scale_param_name] = color_scale_param_value
        plugin_params[color_param_name] = color_param_value

        plugin_params[boolean_prop_param_name] = self.boolean_prop
        plugin_params[color_prop_param_name] = self.color_prop
        plugin_params[double_prop_param_name] = self.double_prop
        plugin_params[int_prop_param_name] = self.int_prop
        plugin_params[layout_prop_param_name] = self.layout_prop
        plugin_params[size_prop_param_name] = self.size_prop
        plugin_params[string_prop_param_name] = self.string_prop
        plugin_params[boolean_vec_prop_param_name] = self.boolean_vec_prop
        plugin_params[color_vec_prop_param_name] = self.color_vec_prop
        plugin_params[double_vec_prop_param_name] = self.double_vec_prop
        plugin_params[int_vec_prop_param_name] = self.int_vec_prop
        plugin_params[coord_vec_prop_param_name] = self.coord_vec_prop
        plugin_params[size_vec_prop_param_name] = self.size_vec_prop
        plugin_params[string_vec_prop_param_name] = self.string_vec_prop

        plugin_ret = self.graph.applyAlgorithm(plugin_name, plugin_params)
        self.assertTrue(plugin_ret[0])

        self.assertEqual(parameters_value[boolean_param_name],
                         boolean_param_value)
        self.assertEqual(parameters_value[int_param_name], int_param_value)
        self.assertEqual(parameters_value[float_param_name], float_param_value)
        self.assertEqual(parameters_value[string_param_name],
                         string_param_value)
        self.assertEqual(parameters_value[string_param_name],
                         string_param_value)
        self.assertEqual(parameters_value[string_collection_param_name],
                         string_collection_param_value)
        self.assertEqual(parameters_value[color_param_name], color_param_value)
        self.assertEqual(parameters_value[color_scale_param_name],
                         color_scale_param_value)

        self.assertEqual(parameters_value[boolean_prop_param_name],
                         self.boolean_prop)
        self.assertEqual(parameters_value[color_prop_param_name],
                         self.color_prop)
        self.assertEqual(parameters_value[double_prop_param_name],
                         self.double_prop)
        self.assertEqual(parameters_value[int_prop_param_name], self.int_prop)
        self.assertEqual(parameters_value[layout_prop_param_name],
                         self.layout_prop)
        self.assertEqual(parameters_value[size_prop_param_name],
                         self.size_prop)
        self.assertEqual(parameters_value[string_prop_param_name],
                         self.string_prop)
        self.assertEqual(parameters_value[boolean_vec_prop_param_name],
                         self.boolean_vec_prop)
        self.assertEqual(parameters_value[color_vec_prop_param_name],
                         self.color_vec_prop)
        self.assertEqual(parameters_value[double_vec_prop_param_name],
                         self.double_vec_prop)
        self.assertEqual(parameters_value[int_vec_prop_param_name],
                         self.int_vec_prop)
        self.assertEqual(parameters_value[coord_vec_prop_param_name],
                         self.coord_vec_prop)
        self.assertEqual(parameters_value[size_vec_prop_param_name],
                         self.size_vec_prop)
        self.assertEqual(parameters_value[string_vec_prop_param_name],
                         self.string_vec_prop)

        plugin_params[color_param_name] = color_param_tuple_value
        del parameters_value[color_param_name]

        plugin_params[color_scale_param_name] = color_scale_param_value_dict
        del parameters_value[color_scale_param_name]

        plugin_ret = self.graph.applyAlgorithm(plugin_name, plugin_params)
        self.assertTrue(plugin_ret[0])

        self.assertEqual(parameters_value[color_param_name], color_param_value)
        self.assertEqual(parameters_value[color_scale_param_name],
                         color_scale_param_value)

        plugin_params[color_scale_param_name] = color_scale_param_value_list
        del parameters_value[color_scale_param_name]

        plugin_ret = self.graph.applyAlgorithm(plugin_name, plugin_params)
        self.assertTrue(plugin_ret[0])

        self.assertEqual(parameters_value[color_scale_param_name],
                         color_scale_param_value)

        plugin_params[string_collection_param_name] = 'toto'

        with self.assertRaises(Exception) as cm:
            plugin_ret = self.graph.applyAlgorithm(plugin_name, plugin_params)
            self.assertContains(
                cm.exception.args[0], 'Possible values are : foo, bar')

    def test_plugin_out_parameters(self):
        plugin_params = tlp.getDefaultPluginParameters(plugin_name, self.graph)

        plugin_ret = self.graph.applyAlgorithm(plugin_name, plugin_params)
        self.assertTrue(plugin_ret[0])

        self.assertIn(out_boolean_param_name, plugin_params)
        self.assertEqual(plugin_params[out_boolean_param_name],
                         out_boolean_param_value)

        self.assertIn(out_int_param_name, plugin_params)
        self.assertEqual(plugin_params[out_int_param_name],
                         out_int_param_value)

        self.assertIn(out_float_param_name, plugin_params)
        self.assertEqual(plugin_params[out_float_param_name],
                         out_float_param_value)

        self.assertIn(out_string_param_name, plugin_params)
        self.assertEqual(plugin_params[out_string_param_name],
                         out_string_param_value)
