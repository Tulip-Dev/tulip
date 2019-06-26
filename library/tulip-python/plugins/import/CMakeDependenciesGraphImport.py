# This file is part of Tulip (http://tulip.labri.fr)
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

# Tulip import plugin written in Python for importing the dependencies
# graph of a CMake project
# (target -> target dependencies + target -> external libraries dependencies)

# The plugin use the graphviz output feature of CMake enabling to
# generate dot files representing the different dependencies graphs of
# the project (global + target specific). The global dot file is then
# imported in Tulip through the graphviz import plugin included
# in the framework

from tulip import tlp
import tulipplugins

import tempfile
import subprocess
import os
import shutil

cmakeProjectSourceDirParamName = 'CMake project source dir'
cmakeExecutableParamName = 'CMake executable'
cmakeParametersParamName = 'CMake parameters'

cmakeProjectDotFileName = 'cmake_project'


def executeCommand(command, cwd, stderrfile, pluginProgress):
    # execute the process
    p = subprocess.Popen(command, cwd=cwd, shell=True, stdout=subprocess.PIPE,
                         stderr=open(stderrfile, 'wb'))
    # add some execution feedback trough the plugin progress
    while True:
        line = p.stdout.readline().decode('utf-8')
        line = line.replace('\r\n', '').replace('\n', '')
        if line == '' and p.poll() is not None:
            break
        if pluginProgress:
            pluginProgress.setComment(line)

    # wait for the process to complete
    retval = p.wait()
    return retval


class CMakeDependenciesGraphImport(tlp.ImportModule):
    def __init__(self, context):
        tlp.ImportModule.__init__(self, context)

        self.addDirectoryParameter(
            cmakeProjectSourceDirParamName,
            'The root source directory of the CMake project')

        self.addFileParameter(
            cmakeExecutableParamName, True,
            ('Optional parameter in order to provide the path to the CMake '
             'executable. By default CMake executable path is assumed to be '
             'in your PATH environment variable'), 'cmake')

        self.addStringParameter(
            cmakeParametersParamName,
            ('Optional parameter for providing some parameters to CMake in '
             'order to correctly configure the project'))

    def icon(self):
        return '%s/cmake.png' % os.path.dirname(__file__)

    def importGraph(self):

        # get the CMake project source directory path
        cmakeProjectSourceDir = self.dataSet[cmakeProjectSourceDirParamName]

        # create a temporary directory that we will use as a CMake
        # binary directory
        tmpdir = tempfile.mkdtemp()

        # build the command line to execute cmake and generate the dot files
        command = '%s %s --graphviz=%s %s' % (
            self.dataSet[cmakeExecutableParamName],
            self.dataSet[cmakeParametersParamName],
            cmakeProjectDotFileName,
            cmakeProjectSourceDir
        )

        # create a temporary file in which we will redirect stderr
        stderrfile = tmpdir + '/stderr.txt'

        # execute the CMake project configuration
        retval = executeCommand(command, tmpdir, stderrfile,
                                self.pluginProgress)

        # Special case for MinGW Makefiles as it often fails the first time it
        # is executed due to the presence of sh.exe in the PATH environment
        # variable. It fails the first time CMake is executed but not for the
        # subsequent calls, so relaunch the CMake configuration
        mingw = 'MinGW Makefiles' in self.dataSet[cmakeParametersParamName]
        if retval != 0 and mingw:
            retval = executeCommand(command, tmpdir, stderrfile,
                                    self.pluginProgress)

        # something went wrong
        if retval != 0:
            # read stderr file and transmit error string to the plugin progress
            errors = open(stderrfile, 'rb').read().decode('utf-8')
            if self.pluginProgress:
                self.pluginProgress.setError(errors)
            # remove temporary directory
            shutil.rmtree(tmpdir)
            return False

        # check if CMake has generated the expected dot file
        cmakeProjectDotFilePath = tmpdir + '/' + cmakeProjectDotFileName
        if not os.path.exists(cmakeProjectDotFilePath):
            cmakeProjectDotFilePath = '%s/%s.dot' % (
                tmpdir, cmakeProjectDotFileName)
            if not os.path.exists(cmakeProjectDotFilePath):
                if self.pluginProgress:
                    self.pluginProgress.setError(
                        ('Error : the graphviz file has not been generated '
                         'by CMake'))
                # remove temporary directory
                shutil.rmtree(tmpdir)
                return False

        # import the dot file in Tulip trough the graphviz import plugin
        dotImportParams = tlp.getDefaultPluginParameters('graphviz',
                                                         self.graph)
        dotImportParams['filename'] = cmakeProjectDotFilePath
        tlp.importGraph("graphviz", dotImportParams, self.graph)

        # set some visual attributes to the imported graph
        self.graph['viewSize'].setAllNodeValue((1, 1, 1))
        self.graph['viewShape'].setAllNodeValue(tlp.NodeShape.Circle)

        # apply a force directed algorithm to draw the graph
        self.graph.applyLayoutAlgorithm(
            'Fast Multipole Multilevel Embedder (OGDF)')

        # remove temporary directory
        shutil.rmtree(tmpdir)

        # done
        return True


# The line below does the magic to register the plugin to the plugin database
# and updates the GUI to make it accessible through the menus.
tulipplugins.registerPluginOfGroup(
    'CMakeDependenciesGraphImport', 'CMake dependencies graph',
    'Antoine Lambert', '24/11/2015',
    "Import the targets dependencies graph of a CMake project",
    '1.0', 'Software')
