/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

// +-------------------------------------------------------------------------+
// | Tulip Python View                                                       |
// | Author:  Antoine Lambert                                                |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python View is free software; you can redistribute it             |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python View is distributed in the hope that it will be            |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

#include <iostream>
#include <sstream>

#include <tulip/TulipRelease.h>
#include <tulip/TlpTools.h>

#include <QtCore/QDir>
#include <QtGui/QVBoxLayout>

#include "PythonInterpreter.h"
#include "ConsoleOutputModule.h"
#include "TulipUtilsModule.h"

#include <sip.h>

extern "C" {

extern void initsip();
extern void initstl();
extern void inittulip();
extern void inittulipogl();

}

#include <cstdio>
#ifndef WIN32 
#include <dlfcn.h>
#endif
using namespace std;

const string pluginUtils =

		"def registerAlgorithmPlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.AlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.AlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerAlgorithmPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.AlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.AlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code) in globals(), locals()\n"

		"def registerLayoutPlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.LayoutAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.LayoutAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerLayoutPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.LayoutAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.LayoutAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerDoublePlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.DoubleAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.DoubleAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerDoublePluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.DoubleAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.DoubleAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerIntegerPlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.IntegerAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.IntegerAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerIntegerPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.IntegerAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.IntegerAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerBooleanPlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.BooleanAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.BooleanAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerBooleanPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.BooleanAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.BooleanAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerSizePlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.SizeAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.SizeAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerSizePluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.SizeAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.SizeAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerColorPlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.ColorAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.ColorAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerColorPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.ColorAlgorithmFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.ColorAlgorithmFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerImportPlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.ImportModuleFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.ImportModuleFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerImportPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.ImportModuleFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.ImportModuleFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerExportPlugin(pluginClassName, algoName, author, date, info, release):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.ExportModuleFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.ExportModuleFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

		"def registerExportPluginOfGroup(pluginClassName, algoName, author, date, info, release, group):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.ExportModuleFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.ExportModuleFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + group + \"\\\"\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

//		"def registerInteractorPlugin(pluginClassName, algoName, author, date, info, release):\n"
//		"	code = \"class \" + pluginClassName + \"Factory(tlp.InteractorFactory):\\n\"\n"
//		"	code += \"\\tdef __init__(self):\\n\"\n"
//		"	code += \"\\t\\ttlp.InteractorFactory.__init__(self)\\n\"\n"
//		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
//		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
//		"	code += \"\\t\\treturn \" + pluginClassName + \"()\\n\"\n"
//		"	code += \"\\tdef getName(self):\\n\"\n"
//		"	code += \"\\t\\t return \\\"\" + algoName + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getAuthor(self):\\n\"\n"
//		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getGroup(self):\\n\"\n"
//		"	code += \"\\t\\t return str(\"\")\\n\"\n"
//		"	code += \"\\tdef getDate(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getInfo(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getRelease(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
//		"	code += \"pluginFactory[algoName] = \" + pluginClassName + \"Factory()\\n\"\n"
//		"	exec(code)\n"

		"def registerGlyphPlugin(pluginClassName, glyphName, author, date, info, release, glyphId):\n"
		"	code = \"class \" + pluginClassName + \"Factory(tlp.GlyphFactory):\\n\"\n"
		"	code += \"\\tdef __init__(self):\\n\"\n"
		"	code += \"\\t\\ttlp.GlyphFactory.__init__(self)\\n\"\n"
		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
		"	code += \"\\t\\treturn \" + pluginClassName + \"(context)\\n\"\n"
		"	code += \"\\tdef getName(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + glyphName + \"\\\"\\n\"\n"
		"	code += \"\\tdef getAuthor(self):\\n\"\n"
		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
		"	code += \"\\tdef getGroup(self):\\n\"\n"
		"	code += \"\\t\\t return str(\"\")\\n\"\n"
		"	code += \"\\tdef getDate(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
		"	code += \"\\tdef getInfo(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
		"	code += \"\\tdef getRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
		"	code += \"\\tdef getId(self):\\n\"\n"
		"	code += \"\\t\\treturn \" + str(glyphId) + \"\\n\"\n"
		"	code += \"pluginFactory[glyphName] = \" + pluginClassName + \"Factory()\\n\"\n"
		"	exec(code)\n"

//		"def registerViewPlugin(pluginClassName, viewName, author, date, info, release):\n"
//		"	code = \"class \" + pluginClassName + \"Factory(tlp.ViewFactory):\\n\"\n"
//		"	code += \"\\tdef __init__(self):\\n\"\n"
//		"	code += \"\\t\\ttlp.ViewFactory.__init__(self)\\n\"\n"
//		"	code += \"\\t\\tself.registerPlugin()\\n\"\n"
//		"	code += \"\\tdef createPluginObject(self, context):\\n\"\n"
//		"	code += \"\\t\\treturn \" + pluginClassName + \"()\\n\"\n"
//		"	code += \"\\tdef getName(self):\\n\"\n"
//		"	code += \"\\t\\t return \\\"\" + viewName + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getAuthor(self):\\n\"\n"
//		"	code += \"\\t\\t return \\\"\" + author + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getGroup(self):\\n\"\n"
//		"	code += \"\\t\\t return str(\"\")\\n\"\n"
//		"	code += \"\\tdef getDate(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\"\" + date + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getInfo(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\"\" + info + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getRelease(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\"\" + release + \"\\\"\\n\"\n"
//		"	code += \"\\tdef getTulipRelease(self):\\n\"\n"
//		"	code += \"\\t\\treturn \\\""+ string(TULIP_RELEASE) + "\\\"\\n\"\n"
//		"	code += \"pluginFactory[viewName] = \" + pluginClassName + \"Factory()\\n\"\n"
//		"	exec(code)\n"
		;

ConsoleOutputDialog::ConsoleOutputDialog(QWidget *parent) : QDialog(parent) {
	setWindowTitle("Python Interpreter Output");
	consoleWidget = new QPlainTextEdit(this);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(consoleWidget);
	setLayout(layout);
	connect(consoleWidget, SIGNAL(textChanged()), this, SLOT(showOnOutputWrite()));
	resize(400,200);
}


void ConsoleOutputDialog::showOnOutputWrite() {
	if (!isVisible())
		show();
}

PythonInterpreter PythonInterpreter::instance;

const sipAPIDef *get_sip_api(){
#if defined(SIP_USE_PYCAPSULE)
	return (const sipAPIDef *)PyCapsule_Import("sip._C_API", 0);
#else
	PyObject *sip_module;
	PyObject *sip_module_dict;
	PyObject *c_api;

	/* Import the SIP module. */
	sip_module = PyImport_ImportModule("sip");

	if (sip_module == NULL)
		return NULL;

	/* Get the module's dictionary. */
	sip_module_dict = PyModule_GetDict(sip_module);

	/* Get the "_C_API" attribute. */
	c_api = PyDict_GetItemString(sip_module_dict, "_C_API");

	if (c_api == NULL)
		return NULL;

	/* Sanity check that it is the right type. */
	if (!PyCObject_Check(c_api))
		return NULL;

	/* Get the actual pointer from the object. */
	return (const sipAPIDef *)PyCObject_AsVoidPtr(c_api);
#endif
}




PythonInterpreter::PythonInterpreter() : runningScript(false) {
	int argc=1;
	char *argv[1];
	argv[0] = const_cast<char *>("");
	Py_OptimizeFlag = 1;
	Py_InitializeEx(0);
	PySys_SetArgv(argc, argv);


	runString("import sys");
	PyObject *pName = PyString_FromString("__main__");
	PyObject *pMainModule = PyImport_Import(pName);
	Py_DECREF(pName);
	PyObject *pMainDict = PyModule_GetDict(pMainModule);
	PyObject *pVersion = PyRun_String("str(sys.version_info[0])+\".\"+str(sys.version_info[1])", Py_eval_input, pMainDict, pMainDict);
	pythonVersion = string(PyString_AsString(pVersion));
	// hack for linux in order to be able to load dynamic python modules installed on the system (like numpy, matplotlib and other cool stuffs)		
#ifndef WIN32	
	string libPythonName = string("libpython") + pythonVersion + string(".so");
	dlopen(libPythonName.c_str(), RTLD_LAZY | RTLD_GLOBAL);
#endif	

	consoleDialog = new ConsoleOutputDialog();
	if (interpreterInit()) {

		initscriptengine();
		_PyImport_FixupExtension(const_cast<char *>("scriptengine"), const_cast<char *>("scriptengine"));

		inittuliputils();
		_PyImport_FixupExtension(const_cast<char *>("tuliputils"), const_cast<char *>("tuliputils"));

		initsip();
		_PyImport_FixupExtension(const_cast<char *>("sip"), const_cast<char *>("sip"));

		initstl();
		_PyImport_FixupExtension(const_cast<char *>("stl"), const_cast<char *>("stl"));

		inittulip();
		_PyImport_FixupExtension(const_cast<char *>("tulip"), const_cast<char *>("tulip"));

		inittulipogl();
		_PyImport_FixupExtension(const_cast<char *>("tulipogl"), const_cast<char *>("tulipogl"));

		runString("import sys; import scriptengine ; sys.stdout = scriptengine.ConsoleOutput(False); sys.stderr = scriptengine.ConsoleOutput(True);\n");
		setDefaultConsoleWidget();
		runString("from tulip import *;from tulipogl import *");
		runString("pluginFactory = {}");


		runString(pluginUtils);
		string pythonPluginsPath = tlp::TulipLibDir + "tlp/python/";
		addModuleSearchPath(pythonPluginsPath);

		QDir pythonPluginsDir(pythonPluginsPath.c_str());
		QStringList nameFilter;
		nameFilter << "*.py";
		QFileInfoList fileList = pythonPluginsDir.entryInfoList(nameFilter);
		for (int i = 0 ; i < fileList.size() ; ++i) {
			QFileInfo fileInfo = fileList.at(i);
			QFile file(fileInfo.absoluteFilePath());
			if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QString pluginCode;
				while (!file.atEnd()) {
					pluginCode += file.readLine();
				}
				file.close();
				runString(pluginCode.toStdString());
			}
		}
	}
}

PythonInterpreter::~PythonInterpreter() {
	if (interpreterInit()) {
		Py_Finalize();
	}
	delete consoleDialog;
}

PythonInterpreter *PythonInterpreter::getInstance() {
	return &instance;
}

bool PythonInterpreter::interpreterInit() const {
	return Py_IsInitialized();
}

void PythonInterpreter::registerNewModule(const string &moduleName, PyMethodDef *moduleDef) {
	Py_InitModule(moduleName.c_str(), moduleDef);
}

void PythonInterpreter::registerNewModuleFromString(const std::string &moduleName, const std::string &moduleSrcCode) {
	ostringstream oss;
	oss << moduleName << ".py";
	PyObject *pycomp = Py_CompileString(moduleSrcCode.c_str(),oss.str().c_str(), Py_file_input);
	if (pycomp == NULL) {
		PyErr_Print();
		PyErr_Clear();
		return;
	}
	PyObject *pmod = PyImport_ExecCodeModule(const_cast<char *>(moduleName.c_str()),pycomp);
	if (pmod == NULL){
		PyErr_Print();
		PyErr_Clear();
	}
}

bool PythonInterpreter::functionExists(const string &moduleName, const string &functionName) {
	PyObject *pName = PyString_FromString(moduleName.c_str());
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	PyObject *pDict = PyModule_GetDict(pModule);
	PyObject *pFunc = PyDict_GetItemString(pDict, functionName.c_str());
	return (pFunc != NULL && PyCallable_Check(pFunc));
}

bool PythonInterpreter::runString(const string &pyhtonCode) {
	int ret = PyRun_SimpleString(pyhtonCode.c_str());
	if (PyErr_Occurred()) {
		PyErr_Print();
		PyErr_Clear();
	}
	return  ret != -1;
}

void PythonInterpreter::addModuleSearchPath(const std::string &path) {
	if (currentImportPaths.find(path) == currentImportPaths.end()) {
		ostringstream oss;
		oss << "import sys" << endl;
		oss << "sys.path.append(\"" << path << "\")" << endl;
		runString(oss.str());
		currentImportPaths.insert(path);
	}
}

bool PythonInterpreter::runGraphScript(const string &module, const string &function, tlp::Graph *graph) {

	// Build the name object
	PyObject *pName = PyString_FromString(module.c_str());

	// Load the module object
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	// pDict is a borrowed reference
	PyObject *pDict = PyModule_GetDict(pModule);

	// pFunc is also a borrowed reference
	PyObject *pFunc = PyDict_GetItemString(pDict, function.c_str());

	if (PyCallable_Check(pFunc)) {
		const sipAPIDef *sisApi = get_sip_api();

		// Getting proper sipWrapperType
		const sipTypeDef* kpTypeDef     = sisApi->api_find_type("tlp::Graph");

		// Wrapping up C++ instance
		PyObject* pArgs = sisApi->api_convert_from_type(graph, kpTypeDef, NULL);

		// Finally calling 'process'
		PyObject* argTup = Py_BuildValue ("(O)", pArgs);
		runningScript = true;
		try {
			PyObject_CallObject(pFunc, argTup);
		} catch(...) {
			//
		}
		runningScript = false;
		Py_DECREF(argTup);
		Py_DECREF(pArgs);
		if (PyErr_Occurred()) {
			PyErr_Print();
			return false;
		}

	} else {
		PyErr_Print();
		return false;
	}

	return true;
}

int stopScript(void *) {
	PyErr_SetString(PyExc_Exception, "Script execution terminated by user");
	return -1;
}


void PythonInterpreter::stopCurrentScript() {
	Py_AddPendingCall(&stopScript, NULL);
}

void PythonInterpreter::setTraceFunction(Py_tracefunc tracefunc) {
	PyEval_SetTrace(tracefunc, NULL);
}

void PythonInterpreter::deleteModule(const std::string &moduleName) {
	ostringstream oss;
	oss << "import sys" << endl;
	oss << "if \"" << moduleName << "\" in sys.modules:" << endl;
	oss << "\tdel sys.modules[\"" << moduleName << "\"]" << endl;
	runString(oss.str());
}

void PythonInterpreter::reloadModule(const std::string &moduleName) {
	ostringstream oss;
	oss << "import " << moduleName << endl;
	oss << "reload(" << moduleName << ")" << endl;
	runString(oss.str());
}

void PythonInterpreter::setConsoleWidget(QPlainTextEdit *console) {
	consoleWidget = console;
}

void PythonInterpreter::setDefaultConsoleWidget() {
	consoleWidget = consoleDialog->consoleWidget;
}

