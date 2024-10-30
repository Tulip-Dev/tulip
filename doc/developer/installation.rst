.. _installation:

****************
Installing Tulip
****************

Tulip is a cross-platform framework and can be installed or compiled on many **Linux** distributions (Arch Linux, Debian, Fedora, Gentoo, OpenSUSE, Ubuntu...), on **MacOS** and **Windows**.


.. _precompiled_binaries:

Precompiled binaries
--------------------

For each release, Tulip offers precompiled binaries for **Linux** (using `AppImage <https://github.com/AppImage/AppImageKit>`_), **MacOS** (dmg bundles) or **Windows** (`NSIS <http://nsis.sourceforge.net/Main_Page>`_ based installers).
You can download those binaries from `SourceForge <https://sourceforge.net/projects/auber/files/tulip/>`_.

Compiling from scratch
----------------------

Tulip can be easily compiled on every supported platforms. However, that process can take some times
depending on your system configuration.

The following dependencies are required to build Tulip:

* `CMake <https://cmake.org>`_ >= 3.22
* A `C++17 <https://en.wikipedia.org/wiki/C%2B%2B17>`_ compiler : `GCC <https://gcc.gnu.org/>`_ > 8.0,
  `Clang <https://clang.llvm.org/>`_ > 5.0 or
  `Microsoft Visual Studio <https://www.visualstudio.com/>`_ >= 2017
* `FreeType <https://www.freetype.org>`_ 
* `zlib <http://zlib.net>`_
* `libzip <https://libzip.org/>`_
* `Qt <https://www.qt.io>`_ >= 5.15
* `OpenGL <https://www.opengl.org>`_ >= 2.0
* `GLEW <http://glew.sourceforge.net/>`_ >= 1.4
* `Python <https://www.python.org>`_ >= 3.9
* `SIP Python Module <https://www.riverbankcomputing.com/software/sip/intro>`_ >= 6.8.5
* `Pip Python module <https://pypi.org/project/pip/>`_
* `Qhull <http://www.qhull.org/>`_
* `yajl <https://lloyd.github.io/yajl/>`_ >= 2.0

In order to generate the documentation, the following tools must be installed:

* `Sphinx <http://www.sphinx-doc.org/en/stable/>`_ to build the `User Manual <https://tulip.labri.fr/Documentation/current/tulip-user/html/index.html>`_, `Developer Handbook <https://tulip.labri.fr/Documentation/current/tulip-dev/html/index.html>`_ and `Python bindings documentation <https://tulip.labri.fr/Documentation/current/tulip-python/html/index.html>`_
* `Doxygen <http://www.stack.nl/~dimitri/doxygen/>`_ to build the `C++ API documentation <https://tulip.labri.fr/Documentation/current/doxygen/html/index.html>`_

If you are a **Linux** developer, all these dependencies can be installed with the package manager of your distribution. The old continuous integration setup file `.travis.yml <https://github.com/Tulip-Dev/tulip/blob/master/travis.yml>`_ can give you some hints for Tulip build on **Linux** platforms.

If you are a **MacOS** developer, we recommend to use `MacPorts <https://www.macports.org/>`_ or `Homebrew <https://brew.sh/>`_ in order to easily install all these dependencies. Hints on how to build Tulip for these two environments can be found in the old continuous integration setup file `.travis.yml <https://github.com/Tulip-Dev/tulip/blob/master/travis.yml>`_. A dedicated script **macports_build.sh** in the **bundlers/macos** directory of Tulip source code is also available for **MacPorts** build.

If you are a **Windows** developer, we recommend to use `MSYS2 <http://www.msys2.org/>`_ as it greatly facilitates the build of Tulip on that platform (notably by providing up to date compilers and precompiled dependencies). A dedicated batch file **msys2_build.bat** in the **bundlers/win** directory of Tulip source code is available for **MSYS2** build.

Hints on how to build Tulip for **MSVC** can be found in `appveyor_msvc.bat <https://github.com/Tulip-Dev/tulip/blob/master/appveyor_msvc.bat>`_ used for continuous integration on `AppVeyor <https://www.appveyor.com/>`_ until 5.6.3 version.

