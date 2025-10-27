# automatically generates the file tulippluginsdocumentation.rst
# by dynamically introspecting the Tulip plugins metadata

from tulip import tlp # noqa
import tulipgui # noqa
import tabulate # noqa
import re # noqa
import os # noqa


def safeprint(s, file=None):
    try:
        print(s, file=file)
    except UnicodeEncodeError:
        print(s.encode('utf8'), file=file)

tulip_build_dir = os.environ['TULIP_BUILD_DIR']
tlp.loadTulipPluginsFromDir('%s/plugins/clustering' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/colors' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/export' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/general' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/import' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/layout' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/metric' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/selection' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/sizes' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/string' % tulip_build_dir)
tlp.loadTulipPluginsFromDir('%s/plugins/test' % tulip_build_dir)
tlp.loadTulipPluginsFromDir(os.environ['TULIP_PYTHON_PLUGINS_DIR'])
tlp.loadTulipPluginsFromDir(os.environ['TULIPGUI_PYTHON_PLUGINS_DIR'])

f = open('tulippluginsdocumentation.rst', 'w', encoding='utf-8')

safeprint("""
.. |br| raw:: html

   <br />

.. |bstart| raw:: html

   <b>

.. |bend| raw:: html

   </b>

.. |istart| raw:: html

   <i>

.. |iend| raw:: html

   </i>

.. |listart| raw:: html

   <li>

.. |liend| raw:: html

   </li>

.. |ulstart| raw:: html

   <ul>

.. |ulend| raw:: html

   </ul>

.. |pstart| raw:: html

   <p>

.. |pend| raw:: html

   </p>

""", file=f)

safeprint('.. py:currentmodule:: tulip\n', file=f)

safeprint('.. _tulippluginsdoc:\n', file=f)


def writeSection(title, sectionChar):
    safeprint(title, file=f)
    underline = ''
    for i in range(len(title)):
        underline += sectionChar
    safeprint(underline+'\n', file=f)


def replaceHtmlTags(doc, htmlTag, htmlTagStartReplacement,
                    htmlTagEndReplacement):
    tmp = re.sub('^<%s>' % htmlTag,
                 '|%s| ' % htmlTagStartReplacement, doc)
    tmp = re.sub('^</%s>' % htmlTag,
                 '|%s| ' % htmlTagEndReplacement, tmp)
    tmp = re.sub('^<%s>' % htmlTag.upper(),
                 '|%s| ' % htmlTagStartReplacement, tmp)
    tmp = re.sub('^</%s>' % htmlTag.upper(),
                 '|%s| ' % htmlTagEndReplacement, tmp)
    tmp = tmp.replace('<%s>' % htmlTag,
                      ' |%s| ' % htmlTagStartReplacement)
    tmp = tmp.replace('</%s>' % htmlTag,
                      ' |%s| ' % htmlTagEndReplacement)
    tmp = tmp.replace('<%s>' % htmlTag.upper(),
                      ' |%s| ' % htmlTagStartReplacement)
    tmp = tmp.replace('</%s>' % htmlTag.upper(),
                      ' |%s| ' % htmlTagEndReplacement)
    return tmp


def formatSphinxDoc(doc):
    doc = re.sub('^\n', '', doc)
    doc = re.sub('\n$', '', doc)
    doc = replaceHtmlTags(doc, 'br', 'br', 'br')
    doc = replaceHtmlTags(doc, 'br/', 'br', 'br')
    doc = replaceHtmlTags(doc, 'b', 'bstart', 'bend')
    doc = replaceHtmlTags(doc, 'i', 'istart', 'iend')
    doc = replaceHtmlTags(doc, 'li', 'listart', 'liend')
    doc = replaceHtmlTags(doc, 'ul', 'ulstart', 'ulend')
    doc = replaceHtmlTags(doc, 'p', 'pstart', 'pend')
    return doc


def getTulipPythonType(tulipType):
    if tulipType == 'BooleanProperty':
        return ':class:`tlp.BooleanProperty`'
    elif tulipType == 'ColorProperty':
        return ':class:`tlp.ColorProperty`'
    elif tulipType == 'DoubleProperty':
        return ':class:`tlp.DoubleProperty`'
    elif tulipType == 'IntegerProperty':
        return ':class:`tlp.IntegerProperty`'
    elif tulipType == 'LayoutProperty':
        return ':class:`tlp.LayoutProperty`'
    elif tulipType == 'SizeProperty':
        return ':class:`tlp.SizeProperty`'
    elif tulipType == 'StringProperty':
        return ':class:`tlp.StringProperty`'
    elif tulipType == 'NumericProperty':
        return ':class:`tlp.NumericProperty`'
    elif tulipType == 'PropertyInterface':
        return ':class:`tlp.PropertyInterface`'
    elif tulipType == 'StringCollection':
        return ':class:`tlp.StringCollection`'
    elif tulipType == 'ColorScale':
        return ':class:`tlp.ColorScale`'
    elif tulipType == 'Color':
        return ':class:`tlp.Color`'
    elif tulipType == 'Boolean':
        return ':const:`bool`'
    elif tulipType.startswith('float'):
        return ':const:`float`'
    elif 'integer' in tulipType:
        return ':const:`int`'
    elif tulipType == 'string':
        return ':const:`str`'
    else:
        return tulipType


writeSection('Tulip plugins documentation', '=')

safeprint("""
In this section, you can find some documentation regarding the C++ algorithm
plugins bundled in the Tulip software but also with the Tulip Python modules
installable through the pip tool. In particular, an exhaustive description of
the input and output parameters for each plugin is given.
To learn how to call all these algorithms in Python, you can refer to the
:ref:`Applying an algorithm on a graph <applyGraphAlgorithm>` section.
The plugins documentation is ordered according to their type.

.. warning:: If you use the Tulip Python bindings through the classical Python
             interpreter, some plugins (Color Mapping, Convolution Clustering,
             File System Directory, GEXF, SVG Export, Website) require the
             :mod:`tulipgui` module to be imported before they can be called
             as they use Qt under the hood.
""" + '\n', file=f)

pluginsNames = tlp.PluginLister.availablePlugins()
plugins = {}
for pluginName in pluginsNames:
    plugin = tlp.PluginLister.pluginInformation(pluginName)
    if not plugin.category() in plugins:
        plugins[plugin.category()] = []
    plugins[plugin.category()].append(plugin)
for cat in sorted(plugins.keys()):
    if cat in ('Perspective', 'Panel', 'Node shape',
               'Edge extremity', 'Interactor'):
        continue
    writeSection(cat, '-')
    if cat == 'Algorithm':
        safeprint('.. _algorithmpluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':meth:`tlp.Graph.applyAlgorithm` method. See also '
                   ':ref:`Calling a general algorithm on a graph '
                   '<callGeneralAlgorithm>` for more details.\n'), file=f)
    elif cat == 'Coloring':
        safeprint('.. _colorpluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':meth:`tlp.Graph.applyColorAlgorithm` method. See also '
                   ':ref:`Calling a property algorithm on a graph '
                   '<callPropertyAlgorithm>` for more details.\n'), file=f)
    elif cat == 'Export':
        safeprint('.. _exportpluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':func:`tlp.exportGraph` function.\n'), file=f)
    elif cat == 'Import':
        safeprint('.. _importpluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':func:`tlp.importGraph` function.\n'), file=f)
    elif cat == 'Labeling':
        safeprint('.. _stringpluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':meth:`tlp.Graph.applyStringAlgorithm` method. See also '
                   ':ref:`Calling a property algorithm on a graph '
                   '<callPropertyAlgorithm>` for more details.\n'), file=f)
    elif cat == 'Layout':
        safeprint('.. _layoutpluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':meth:`tlp.Graph.applyLayoutAlgorithm` method. See also '
                   ':ref:`Calling a property algorithm on a graph '
                   '<callPropertyAlgorithm>` for more details.\n'), file=f)
    elif cat == 'Measure':
        safeprint('.. _doublepluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':meth:`tlp.Graph.applyDoubleAlgorithm` method. See also '
                   ':ref:`Calling a property algorithm on a graph '
                   '<callPropertyAlgorithm>` for more details.\n'), file=f)
    elif cat == 'Resizing':
        safeprint('.. _sizepluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':meth:`tlp.Graph.applySizeAlgorithm` method. See also '
                   ':ref:`Calling a property algorithm on a graph '
                   '<callPropertyAlgorithm>` for more details.\n'), file=f)
    elif cat == 'Selection':
        safeprint('.. _booleanpluginsdoc:\n', file=f)
        safeprint(('To call these plugins, you must use the '
                   ':meth:`tlp.Graph.applyBooleanAlgorithm` method. See also '
                   ':ref:`Calling a property algorithm on a graph '
                   '<callPropertyAlgorithm>` for more details.\n'), file=f)

    for p in sorted(plugins[cat], key=lambda p: p.name()):
        writeSection(p.name(), '^')
        writeSection('Description', '"')
        infos = formatSphinxDoc(p.info())
        pos = infos.find('<a')
        while pos != -1:
            pos2 = infos.find('href="', pos + 2)
            pos3 = infos.find('"', pos2 + 6)
            url = infos[pos2 + 6: pos3]
            pos2 = infos.find('>', pos3)
            pos3 = infos.find('</a>', pos2)
            txt = infos[pos2 + 1: pos3]
            url = '`' + txt + ' <' + url + '>`_'
            infos = infos[0 : pos] + url + infos[pos3 + 4:]
            if pos + len(url) < len(infos):
                pos = infos.find('<a', pos)
            else:
                break

        safeprint(infos+'\n', file=f)

        params = tlp.PluginLister.getPluginParameters(p.name())
        headers = ["name", "type", "default", "direction", "description"]
        paramsTable = []
        nbInParams = 0
        for param in params.getParameters():
            paramHelpHtml = param.getHelp()
            pattern = 'class="description">'
            pos = paramHelpHtml.find(pattern)
            paramHelp = ''
            if pos != -1:
                pos2 = paramHelpHtml.find('</', pos)
                paramHelp = paramHelpHtml[pos+len(pattern):pos2]
                paramHelp = formatSphinxDoc(paramHelp)
            pattern = 'class="type">'
            pos = paramHelpHtml.find(pattern)
            paramType = ''
            if pos != -1:
                pos2 = paramHelpHtml.find('</', pos)
                paramType = paramHelpHtml[pos+len(pattern):pos2].replace(
                    ' (double precision)', '')
            if param.getName() == 'result' and 'Property' in paramType:
                continue
            paramDir = 'input / output'
            if param.getDirection() == tlp.ParameterDirection.IN_PARAM:
                paramDir = 'input'
                nbInParams = nbInParams+1
            elif param.getDirection() == tlp.ParameterDirection.OUT_PARAM:
                paramDir = 'output'
            else:
                nbInParams = nbInParams+1
            pattern = 'class="values">'
            pos = paramHelpHtml.find(pattern)
            paramValues = ''
            if pos != -1:
                pos2 = paramHelpHtml.find('</', pos)
                paramValues = paramHelpHtml[pos+len(pattern):pos2]
                paramValues = formatSphinxDoc(paramValues)
            paramDefValue = param.getDefaultValue()
            if paramType == 'StringCollection':
                paramDefValue = paramDefValue[0:paramDefValue.find(';')]
            else:
                paramDefValue = paramDefValue.replace(
                    'false', ':const:`False`')
                paramDefValue = paramDefValue.replace('true', ':const:`True`')
            nonBreakingSpace = u'\xa0'
            if len(paramValues) > 0:
                paramDefValue += (u' |br| |br| |bstart| Values: |bend| |br| %s'
                                  % paramValues)
            valuesDoc = False

            paramName = param.getName().replace(
                'file::', '').replace('dir::', '')
            paramName = paramName.replace(' ', nonBreakingSpace)
            paramType = getTulipPythonType(
                paramType.replace(' ', nonBreakingSpace))
            paramDir = paramDir.replace(' ', nonBreakingSpace)
            paramsTable.append([paramName, paramType, paramDefValue, paramDir,
                                paramHelp])
        if len(paramsTable) > 0:
            writeSection('Parameters', '"')
            safeprint(tabulate.tabulate(paramsTable, headers,
                                        tablefmt='grid') + '\n', file=f)
        writeSection('Calling the plugin from Python', '"')
        safeprint(('To call that plugin from Python, use the following '
                   'code snippet::\n'), file=f)
        safeprint(('  # get a dictionary filled with the default plugin '
                   'parameters values'), file=f)
        if p.category() != 'Import':
            safeprint(('  # graph is an instance of the tlp.Graph class\n'
                       '  params = tlp.getDefaultPluginParameters(\'%s\', '
                       'graph)\n') % p.name(), file=f)
        else:
            safeprint(('  # for a not yet created graph\n'
                       '  params = tlp.getDefaultPluginParameters(\'%s\')\n'
                       '  # Warning: when importing in an existing graph, use\n'
                       '  # params = tlp.getDefaultPluginParameters(\'%s\', graph)\n')
                      % (p.name(), p.name()), file=f)
        if nbInParams > 0 and not(len(paramsTable) == 1 and
                                  paramsTable[0][0] == 'result'):
            safeprint('  # set any input parameter value if needed', file=f)
            for paramData in paramsTable:
                safeprint('  # params[\'%s\'] = ...' % paramData[0], file=f)
            safeprint('', file=f)
        if p.category() == 'Import':
            safeprint(('  # import in a newly created graph\n'
                       '  graph = tlp.importGraph(\'%s\', params)\n'
                       '  # Warning: when importing in an existing graph, use\n'
                       '  # tlp.importGraph(\'%s\', params, graph)\n')
                       % (p.name(), p.name()), file=f)
        elif p.category() == 'Export':
            safeprint('  outputFile = \'<path to a file>\'', file=f)
            safeprint(('  success = tlp.exportGraph(\'%s\', graph, outputFile,'
                       ' params)') % p.name(), file=f)
        elif p.category() == 'Layout':
            safeprint(('  # either create or get a layout property from the'
                       ' graph to store the result of the algorithm'), file=f)
            safeprint(('  resultLayout = graph.getLayoutProperty('
                       '\'resultLayout\')'), file=f)
            safeprint(('  success = graph.applyLayoutAlgorithm(\'%s\', '
                       'resultLayout, params)\n') % p.name(), file=f)
            safeprint(('  # or store the result of the algorithm in the'
                       ' default Tulip layout property named \'viewLayout\''),
                      file=f)
            safeprint(('  success = graph.applyLayoutAlgorithm(\'%s\', '
                       'params)\n') % p.name(), file=f)
        elif p.category() == 'Measure':
            safeprint(('  # either create or get a double property from the'
                       ' graph to store the result of the algorithm'), file=f)
            safeprint(('  resultMetric = graph.getDoubleProperty('
                       '\'resultMetric\')'), file=f)
            safeprint(('  success = graph.applyDoubleAlgorithm(\'%s\''
                       ', resultMetric, params)\n') % p.name(), file=f)
            safeprint(('  # or store the result of the algorithm in the'
                       ' default Tulip metric property named \'viewMetric\''),
                      file=f)
            safeprint(('  success = graph.applyDoubleAlgorithm(\'%s\','
                       ' params)\n') % p.name(), file=f)
        elif p.category() == 'Coloring':
            safeprint(('  # either create or get a color property from the'
                       ' graph to store the result of the algorithm'), file=f)
            safeprint(('  resultColor = graph.getColorProperty('
                       '\'resultColor\')'), file=f)
            safeprint(('  success = graph.applyColorAlgorithm(\'%s\','
                       ' resultColor, params)\n') % p.name(), file=f)
            safeprint(('  # or store the result of the algorithm in the'
                       ' default Tulip color property named \'viewColor\''),
                      file=f)
            safeprint(('  success = graph.applyColorAlgorithm(\'%s\', '
                       'params)\n') % p.name(), file=f)
        elif p.category() == 'Resizing':
            safeprint(('  # either create or get a size property from the'
                       ' graph to store the result of the algorithm'), file=f)
            safeprint('  resultSize = graph.getSizeProperty(\'resultSize\')',
                      file=f)
            safeprint(('  success = graph.applySizeAlgorithm(\'%s\','
                       ' resultSize, params)\n') % p.name(), file=f)
            safeprint(('  # or store the result of the algorithm in the'
                       ' default Tulip size property named \'viewSize\''),
                      file=f)
            safeprint(('  success = graph.applySizeAlgorithm(\'%s\', '
                       'params)\n') % p.name(), file=f)
        elif p.category() == 'Selection':
            safeprint(('  # either create or get a boolean property from the'
                       ' graph to store the result of the algorithm'), file=f)
            safeprint(('  resultSelection = graph.getBooleanProperty('
                       '\'resultSelection\')'), file=f)
            safeprint(('  success = graph.applyBooleanAlgorithm(\'%s\', '
                       'resultSelection, params)\n') % p.name(), file=f)
            safeprint(('  # or store the result of the algorithm in the '
                       'default Tulip boolean property named '
                       '\'viewSelection\''), file=f)
            safeprint(('  success = graph.applyBooleanAlgorithm(\'%s\', '
                       'params)\n') % p.name(), file=f)
        elif p.category() == 'Labeling':
            safeprint(('  # either create or get a string property from the '
                       'graph to store the result of the algorithm'), file=f)
            safeprint(('  resultString = graph.getStringProperty('
                       '\'resultString\')'), file=f)
            safeprint(('  success = graph.applyStringAlgorithm(\'%s\', '
                       'resultString, params)\n') % p.name(), file=f)
            safeprint(('  # or store the result of the algorithm in the '
                       'default Tulip string property named \'viewLabel\''),
                      file=f)
            safeprint(('  success = graph.applyStringAlgorithm(\'%s\', '
                       'params)\n') % p.name(), file=f)
        elif p.category() == 'Algorithm':
            safeprint(('  success = graph.applyAlgorithm(\'%s\', '
                       'params)\n') % p.name(), file=f)
        safeprint(('  # if the plugin declare any output parameter, its value'
                   ' can now be retrieved in the \'params\' dictionary\n'),
                  file=f)
f.close()
