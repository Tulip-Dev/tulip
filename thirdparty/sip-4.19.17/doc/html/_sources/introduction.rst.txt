Introduction
============

This is the reference guide for SIP 4.19.17.  SIP is a tool for
automatically generating `Python <https://www.python.org>`__ bindings for C and
C++ libraries.  SIP was originally developed in 1998 for
`PyQt <https://www.riverbankcomputing.com/software/pyqt>`__ - the Python
bindings for the Qt GUI toolkit - but is suitable for generating bindings for
any C or C++ library.

This version of SIP generates bindings for Python v2.3 or later, including
Python v3.

There are many other similar tools available.  One of the original such tools
is `SWIG <http://www.swig.org>`__ and, in fact, SIP is so called because it
started out as a small SWIG.  Unlike SWIG, SIP is specifically designed for
bringing together Python and C/C++ and goes to great lengths to make the
integration as tight as possible.

The homepage for SIP is https://www.riverbankcomputing.com/software/sip.  Here
you will always find the latest stable version and the latest version of this
documentation.

SIP can also be downloaded from the
`Mercurial <https://www.mercurial-scm.org>`__ repository at
https://www.riverbankcomputing.com/hg/sip.


License
-------

SIP is licensed under similar terms as Python itself.  SIP is also licensed
under the GPL (both v2 and v3).  It is your choice as to which license you
use.  If you choose the GPL then any bindings you create must be distributed
under the terms of the GPL.


Features
--------

SIP, and the bindings it produces, have the following features:

- bindings are fast to load and minimise memory consumption especially when
  only a small sub-set of a large library is being used

- automatic conversion between standard Python and C/C++ data types

- overloading of functions and methods with different argument signatures

- support for Python's keyword argument syntax

- support for both explicitly specified and automatically generated docstrings

- access to a C++ class's protected methods

- the ability to define a Python class that is a sub-class of a C++ class,
  including abstract C++ classes

- Python sub-classes can implement the :meth:`__dtor__` method which will be
  called from the C++ class's virtual destructor

- support for ordinary C++ functions, class methods, static class methods,
  virtual class methods and abstract class methods

- the ability to re-implement C++ virtual and abstract methods in Python

- support for global and class variables

- support for global and class operators

- support for C++ namespaces

- support for C++ templates

- support for C++ exceptions and wrapping them as Python exceptions

- the automatic generation of complementary rich comparison slots

- support for deprecation warnings

- the ability to define mappings between C++ classes and similar Python data
  types that are automatically invoked

- the ability to automatically exploit any available run time type information
  to ensure that the class of a Python instance object matches the class of the
  corresponding C++ instance

- the ability to change the type and meta-type of the Python object used to
  wrap a C/C++ data type

- full support of the Python global interpreter lock, including the ability to
  specify that a C++ function of method may block, therefore allowing the lock
  to be released and other Python threads to run

- support for consolidated modules where the generated wrapper code for a
  number of related modules may be included in a single, possibly private,
  module

- support for the concept of ownership of a C++ instance (i.e. what part of the
  code is responsible for calling the instance's destructor) and how the
  ownership may change during the execution of an application

- the ability to generate bindings for a C++ class library that itself is built
  on another C++ class library which also has had bindings generated so that
  the different bindings integrate and share code properly

- a sophisticated versioning system that allows the full lifetime of a C++
  class library, including any platform specific or optional features, to be
  described in a single set of specification files 

- support for the automatic generation of PEP 484 type hint stub files

- the ability to include documentation in the specification files which can be
  extracted and subsequently processed by external tools

- the ability to include copyright notices and licensing information in the
  specification files that is automatically included in all generated source
  code

- a build system, written in Python, that you can extend to configure, compile
  and install your own bindings without worrying about platform specific issues

- support for building your extensions using distutils

- SIP, and the bindings it produces, runs under UNIX, Linux, Windows, MacOS/X,
  Android and iOS.


SIP Components
--------------

SIP comprises a number of different components.

- The SIP code generator (:program:`sip`).  This processes :file:`.sip`
  specification files and generates C or C++ bindings.  It is covered in detail
  in :ref:`ref-using`.

- The SIP header file (:file:`sip.h`).  This contains definitions and data
  structures needed by the generated C and C++ code.

- The SIP module (:file:`sip.so` or :file:`sip.pyd`).  This is a Python
  extension module that is imported automatically by SIP generated bindings and
  provides them with some common utility functions.  Historically the module
  was installed in the Python installation's ``site-packages`` directory where
  it was imported by any extension module that needed it, for example
  :mod:`PyQt4` and :mod:`PyQt5`.  However this approach introduces dependencies
  between otherwise unrelated packages.  The preferred approach is for each
  package to include it's own private copy of the module that is installed in
  the root directory of the package as described in :ref:`ref-private-sip`.
  See also :ref:`ref-python-api`.

- The SIP build system (:file:`sipconfig.py`).  This is a pure Python module
  that is created when SIP is configured and encapsulates all the necessary
  information about your system including relevant directory names, compiler
  and linker flags, and version numbers.  It also includes several Python
  classes and functions which help you write configuration scripts for your own
  bindings.  It is covered in detail in :ref:`ref-build-system`.

- The SIP distutils extension (:file:`sipdistutils.py`).  This is a distutils
  extension that can be used to build your extension modules using distutils
  and is an alternative to writing configuration scripts with the SIP build
  system.  This can be as simple as adding your .sip files to the list of files
  needed to build the extension module.  It is covered in detail in
  :ref:`ref-distutils`.


Preparing for SIP v5
--------------------

SIP v4.19 will be the final series of SIP v4 releases.  The next major release
of SIP will be v5.  SIP v5.0 and SIP v5.1 will have different goals.

The goals of SIP v5.0 will be to:

- remove support for Python v2

- remove support for other Python versions that have reached their end-of-life

- remove support for all features marked as deprecated in SIP v4

- play nicely in a modern Python packaging context (distutils, setuptools,
  wheels, PyPI) both itself and the packages being generated.

The goals of SIP v5.1 will be to:

- eliminate any remaining inconsistencies in the syntax of specification files

- fill in some gaps in the C/C++ support

- restructure, refactor and rewrite the code as appropriate to ensure that it
  is easy to test, maintain and enhance over the long term.

All features that will be removed in SIP v5.0 will trigger a deprecation
warning.  Any new language features added in SIP v5.0 will also be added to
SIP v4.19.  A set of specification files that does not trigger any deprecation
warnings with SIP v4.19 will work unchanged with SIP v5.0.

It will be possible to install SIP v4 and SIP v5 for the same Python
installation without conflict.

New releases of SIP v4.19 and SIP v5.0 will be made in parallel.  During this
period projects should update their build systems to use SIP v5.  When SIP v5.1
is released no more SIP v4.19 releases will be made.  It is guaranteed that
there will be a minimum of six months between the releases of SIP v5.0 and SIP
v5.1.

SIP v5 will introduce a formal end-of-life policy for Python versions.  When a
Python version reaches it's end-of-life, support for it will be removed in the
next minor release of SIP v5.  For example, if the current version of SIP is
v5.x.y then the support will be removed in v5.x+1.0.  Specifically, SIP v5.0
will only support Python v3.5 and later.


Qt Support
----------

SIP has specific support for the creation of bindings for the Qt application
toolkit from The Qt Company.

The SIP code generator understands the signal/slot type safe callback mechanism
that Qt uses to connect objects together.  This allows applications to define
new Python signals, and allows any Python callable object to be used as a slot.

SIP itself does not require Qt to be installed.
