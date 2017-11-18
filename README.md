# Tulip [![license](https://img.shields.io/github/license/Tulip-dev/tulip.svg)](https://www.gnu.org/licenses/lgpl-3.0.html) [![GitHub last commit](https://img.shields.io/github/last-commit/Tulip-Dev/tulip.svg)]() [![Build Status](https://travis-ci.org/Tulip-Dev/tulip.svg?branch=master)](https://travis-ci.org/Tulip-Dev/tulip) [![Build Status](https://ci.appveyor.com/api/projects/status/github/tulip-dev/tulip?branch=master&svg=true)](https://ci.appveyor.com/project/tulip-dev/tulip) [![SourceForge](https://img.shields.io/sourceforge/dt/auber/tulip.svg)](https://sourceforge.net/projects/auber/files/tulip/) [![Download](https://api.bintray.com/packages/tulip-dev/tulip_binaries/Tulip/images/download.svg)](https://bintray.com/tulip-dev/tulip_binaries/Tulip/_latestVersion) [![PyPI version](https://badge.fury.io/py/tulip-python.svg)](https://badge.fury.io/py/tulip-python) [![Download](https://api.bintray.com/packages/tulip-dev/tulip_binaries/Tulip/images/download.svg?version=5.2.0-dev)](https://bintray.com/tulip-dev/tulip_binaries/Tulip/5.2.0-dev/link)

![alt text](http://tulip.labri.fr/TulipDrupal/sites/default/files/logo_web.png "Tulip")

[Tulip](http://tulip.labri.fr) is an open source, cross-platform, data visualization framework mainly dedicated to the analysis, 
the drawing and the visualization of very large graphs (up to the million of nodes and edges).
It has been created by [David Auber](http://www.labri.fr/perso/auber/david_auber_home_page/doku.php/start), 
from [LaBRI](https://www.labri.fr/) (Laboratoire Bordelais de Recherche en Informatique) and [University of Bordeaux](https://www.u-bordeaux.fr/),
and maintained over the years by an average size development team.
Until the 5.1 release, the main Tulip code repository (subversion based) was hosted 
on [SourceForge](https://sourceforge.net/projects/auber/).
That repository is now in read-only mode and future development of Tulip will be hosted here.

![alt text](screenshots/tulip_screenshot01.png "Tulip software GUI")

## Overview

Tulip is an information visualization framework dedicated to the analysis and visualization of 
relational data. Tulip aims to provide the developer with a complete library, supporting the design 
of interactive information visualization applications for relational data that can be tailored to 
the problems he or she is addressing.

Written in C++ the framework enables the development of algorithms, visual encoding,
interaction techniques, data models, and domain-specific visualizations. One of the goal 
of Tulip is to facilitates the reuse of components and allows the developers to focus on 
programming their application. This development pipeline makes the framework efficient 
for research prototyping as well as the development of end-user applications.

## Features

The Tulip framework offers numerous features, notably:

  * An efficient graph data model in terms of memory usage for storing large networks and 
  the attributes of their elements (called properties in the Tulip semantics).
  It is also one of the few that offer the possibility to efficiently define and navigate 
  graph hierarchies or cluster trees (nested sub-graphs).
  
  * Several graph file formats for serializing such a model to disk, notably the 
  [TLP format](http://tulip.labri.fr/TulipDrupal/?q=tlp-file-format) based on a 
  Lisp syntax for easy parsing but also the TLP binary format for faster graph 
  saving and loading.
  
  * A large variety of graphs algorithms: clustering, metric, layout ... As Tulip is dedicated
  to graph visualization, it is provided with numerous state of the art graph layout algorithms
  but also a bridge to the [Open Graph Drawing Framework](http://www.ogdf.net).
  
  * A hardware accelerated graph rendering engine written in [OpenGL](https://www.opengl.org), 
  highly customizable in terms of visual encoding for graph nodes and edges, in order to 
  efficiently generate aesthetic and interactive visualizations.
  
  * Multiple visualization components (called views in the Tulip semantics) for analyzing graph data
  using other representations than the classical node link diagram one: matrix, histograms,
  scatter plots, parallel coordinates, ...
  
  * [Python](https://www.python.org) bindings for the main Tulip C++ API, giving to Tulip scripting 
  facilities for manipulating graphs loaded from its main graphical user interface.
  The bindings can also be obtained from the [Python Packaging Index](https://pypi.python.org/pypi/tulip-python).
  
  * A plugin based architecture for easily extend the capability of the framework with new
  graph import mechanisms, graph algorithms, visualization components, ...
  Tulip plugins can be written in C++ or Python.
    
  * A graphical user interface, based on the [Qt](https://www.qt.io) framework, enabling to
  easily interact and manipulate the different components of the framework.


<p align="center">
  <img src="screenshots/tulip_screenshot02.png" title="Python in action under Tulip 5"/>
  <img src="screenshots/tulip_screenshot03.png" title="Visual Analytics with Tulip 5"/>
</p>
  
## Documentation

Below are some links to relevant documentation resources about the Tulip framework:

  * [Tulip User Handbook](http://tulip.labri.fr/Documentation/current/tulip-user/html/index.html)
  * [Tulip Developer Handbook](http://tulip.labri.fr/Documentation/current/tulip-dev/html/index.html)
  * [Tulip C++ API](http://tulip.labri.fr/Documentation/current/doxygen/html/index.html)
  * [Tulip Python](http://tulip.labri.fr/Documentation/current/tulip-python/html/index.html)
  
## Installing Tulip

Tulip is a cross-platform framework and can be compiled or installed on [FreeBSD](https://www.freshports.org/graphics/tulip/),
Linux (packages available for [Debian](https://packages.debian.org/search?keywords=tulip), [OpenSUSE](https://software.opensuse.org/download.html?project=graphics&package=tulip), [Ubuntu](https://packages.ubuntu.com/fr/xenial/tulip) or [Fedora](https://fedora.pkgs.org/26/rpm-sphere/tulip-4.9.0-6.1.x86_64.rpm.html) but not necessarily
in synch with latest Tulip version), MacOs and Windows.

### Precompiled binaries

For each release, Tulip offers precompiled binaries for Linux (using [AppImage](https://github.com/AppImage/AppImageKit)), 
MacOS (dmg bundles) or Windows ([NSIS](http://nsis.sourceforge.net/Main_Page) based installers).
You can download those binaries from [Bintray](https://bintray.com/tulip-dev/tulip_binaries/Tulip/_latestVersion),
[GitHub](https://github.com/Tulip-Dev/tulip/releases/latest) or [SourceForge](https://sourceforge.net/projects/auber/files/tulip/). 

You can also test the latest development version of Tulip from the installers continuously uploaded on [Bintray](https://bintray.com/tulip-dev/tulip_binaries/Tulip/5.2.0-dev). Development versions are not considered stable so use at your own risks.

### Compiling from scratch

Tulip can be easily compiled on every supported platforms. However, that process can take some times
depending on your system configuration.

The following dependencies are required to build Tulip:

  * [CMake](https://cmake.org) >= 2.8.12
  * [GCC](https://gcc.gnu.org/) >= 4.8.1,
  [Clang](https://clang.llvm.org/) or 
  [Microsoft Visual Studio](https://www.visualstudio.com/)
  * [FreeType](https://www.freetype.org) 
  * [zlib](http://zlib.net)
  * [libjpeg](https://libjpeg-turbo.org/)
  * [libpng](http://www.libpng.org/pub/png/libpng.html)
  * [Qt](https://www.qt.io) >= 4.6.0
  * [OpenGL](https://www.opengl.org) >= 2.0
  * [GLEW](http://glew.sourceforge.net/) >= 1.4

In order to build the Python components, the following dependencies are needed:

  * [Python](https://www.python.org) >= 2.6
  * [SIP](https://www.riverbankcomputing.com/software/sip/intro) >= 4.19.3
  (if SIP can not be found or its version does not match the required one, 
  it will be compiled using a copy of its source code in the Tulip tree)
  
The following dependencies are also needed but they will be compiled from the Tulip
source tree if they can not be found on your system:

  * [Qhull](http://www.qhull.org/)
  * [QuaZIP](http://quazip.sourceforge.net/)
  * [yajl](https://lloyd.github.io/yajl/) >= 2.0
  
In order to generate the documentation, the following tools must be installed:

  * [Sphinx](http://www.sphinx-doc.org/en/stable/)
  * [Doxygen](http://www.stack.nl/~dimitri/doxygen/)

If you are a Linux user, all these dependencies can be installed with the package manager of your distribution. 

If you are a MacOS user, we recommend to use [MacPorts](https://www.macports.org/) in order to easily install all these dependencies.

If you are a Windows user, we recommend to use [MSYS2](http://www.msys2.org/) as it facilitates a lot the build of Tulip on
that platform (notably by providing up to date compilers and precompiled dependencies).

Hints on how to build Tulip for these three platforms can be found in the continuous integration setup for [TravisCI](https://travis-ci.org/) and
[AppVeyor](https://www.appveyor.com/):

  * [.travis.yml](.travis.yml)
  * [appveyor.yml](appveyor.yml)
  * [appveyor_msys2.bat](appveyor_msys2.bat)
  * [appveyor_msvc.bat](appveyor_msvc.bat)

## References

Below are some scientific publications related to Tulip:

  * David Auber, Romain Bourqui, Maylis Delest, Antoine Lambert, Patrick Mary, Guy Mélançon, Bruno Pinaud, Benjamin Renoust and Jason Vallet. TULIP 4. Research report. LaBRI - Laboratoire Bordelais de Recherche en Informatique. 2016. https://hal.archives-ouvertes.fr/hal-01359308

  * David Auber, Daniel Archambault, Romain Bourqui, Antoine Lambert, Morgan Mathiaut, Patrick Mary, Maylis Delest, Jonathan Dubois, and Guy Mélançon. The Tulip 3 Framework: A Scalable Software Library for Information Visualization Applications Based on Relational Data. Technical report RR-7860, INRIA, January 2012. https://hal.archives-ouvertes.fr/hal-00659880

  * Antoine Lambert and David Auber. Graph analysis and visualization with Tulip-Python. EuroSciPy 2012 - 5th European meeting on Python in Science, Bruxelles. https://hal.archives-ouvertes.fr/hal-00744969

  * David Auber. Tulip - A Huge Graph Visualization Framework. Graph Drawing Software, Springer, pages 105-126. 2004. http://tulip.labri.fr/publi_tulip/auberChapterTulipGDSBook.pdf

## License

Tulip is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

Tulip is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
