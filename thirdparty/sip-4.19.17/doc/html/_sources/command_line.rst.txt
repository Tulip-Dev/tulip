.. _ref-command-line:

The SIP Command Line
====================

The syntax of the SIP command line is::

    sip [options] [specification]

``specification`` is the name of the specification file for the module.  If it
is omitted then ``stdin`` is used.

The full set of command line options is:

.. program:: sip

.. cmdoption:: -h

    Display a help message.

.. cmdoption:: -V

    Display the SIP version number.

.. cmdoption:: -a <FILE>

    .. deprecated:: 4.18

    The name of the QScintilla API file to generate.  This file contains a
    description of the module API in a form that the QScintilla editor
    component can use for auto-completion and call tips.  (The file may also be
    used by the SciTE editor but must be sorted first.)  By default the file is
    not generated.

.. cmdoption:: -b <FILE>

    The name of the build file to generate.  This file contains the information
    about the module needed by the :ref:`SIP build system <ref-build-system>`
    to generate a platform and compiler specific Makefile for the module.  By
    default the file is not generated.

.. cmdoption:: -B <TAG>

    .. versionadded:: 4.16

    The tag is added to the list of *backstops*.  The option may be given more
    than once if multiple timelines have been defined.  See the
    :directive:`%Timeline` directive for more details.

.. cmdoption:: -c <DIR>

    The name of the directory (which must exist) into which all of the
    generated C or C++ code is placed.  By default no code is generated.

.. cmdoption:: -d <FILE>

    .. deprecated:: 4.12
        Use the :option:`-X <sip -X>` option instead.

    The name of the documentation file to generate.  Documentation is included
    in specification files using the :directive:`%Doc` and
    :directive:`%ExportedDoc` directives.  By default the file is not
    generated.

.. cmdoption:: -D

    .. versionadded:: 4.19.1

    Code is generated for a debug build of Python.

.. cmdoption:: -e

    Support for C++ exceptions is enabled.  This causes all calls to C++ code
    to be enclosed in ``try``/``catch`` blocks and C++ exceptions to be
    converted to Python exceptions.  By default exception support is disabled.

.. cmdoption:: -f

    .. versionadded:: 4.18

    Warnings are handled as if they were errors and the program terminates.

.. cmdoption:: -g

    The Python GIL is released before making any calls to the C/C++ library
    being wrapped and reacquired afterwards.  See :ref:`ref-gil` and the
    :fanno:`ReleaseGIL` and :fanno:`HoldGIL` annotations.

.. cmdoption:: -I <DIR>

    The directory is added to the list of directories searched when looking for
    a specification file given in an :directive:`%Include` or
    :directive:`%Import` directive.  Directory separators must always be ``/``.
    This option may be given any number of times.

.. cmdoption:: -j <NUMBER>

    The generated code is split into the given number of files.  This makes it
    easier to use the parallel build facility of most modern implementations of
    ``make``.  By default 1 file is generated for each C structure or C++
    class.

.. cmdoption:: -k

    .. versionadded:: 4.10

    .. deprecated:: 4.12
        Use the ``keyword_arguments="All"`` :directive:`%Module` directive
        argument instead.

    All functions and methods will, by default, support passing parameters
    using the Python keyword argument syntax.

.. cmdoption:: -n <NAME>

    .. versionadded:: 4.19.9

    The fully qualified name of the private copy of the :mod:`sip` module.
    See also the :option:`--sip-module <configure.py --sip-module>` option of
    the installation script.

.. cmdoption:: -o

    .. versionadded:: 4.10

    Docstrings will be automatically generated that describe the signature of
    all functions, methods and constructors.

.. cmdoption:: -p <MODULE>

    The name of the :directive:`%ConsolidatedModule` which will contain the
    wrapper code for this component module.

.. cmdoption:: -P

    .. versionadded:: 4.10

    By default SIP generates code to provide access to protected C++ functions
    from Python.  On some platforms (notably Linux, but not Windows) this code
    can be avoided if the ``protected`` keyword is redefined as ``public``
    during compilation.  This can result in a significant reduction in the size
    of a generated Python module.  This option disables the generation of the
    extra code.

.. cmdoption:: -r

    Debugging statements that trace the execution of the bindings are
    automatically generated.  By default the statements are not generated.

.. cmdoption:: -s <SUFFIX>

    The suffix to use for generated C or C++ source files.  By default ``.c``
    is used for C and ``.cpp`` for C++.

.. cmdoption:: -t <TAG>

    The SIP version tag (declared using a :directive:`%Timeline` directive) or
    the SIP platform tag (declared using the :directive:`%Platforms` directive)
    to generate code for.  This option may be given any number of times so long
    as the tags do not conflict.

.. cmdoption:: -T

    .. deprecated:: 4.16.6
        This option is now ignored and timestamps are always disabled.

    By default the generated C and C++ source and header files include a
    timestamp specifying when they were generated.  This option disables the
    timestamp so that the contents of the generated files remain constant for a
    particular version of SIP.

.. cmdoption:: -w

    The display of warning messages is enabled.  By default warning messages
    are disabled.

.. cmdoption:: -x <FEATURE>

    The feature (declared using the :directive:`%Feature` directive) is
    disabled.

.. cmdoption:: -X <ID:FILE>

    .. versionadded:: 4.12

    The extract (defined with the :directive:`%Extract` directive) with the
    identifier ``ID`` is written to the file ``FILE``.

.. cmdoption:: -y <FILE>

    .. versionadded:: 4.18

    The name of the Python type hints stub file to generate.  This file
    contains a description of the module API that is compliant with PEP 484.
    By default the file is not generated.

.. cmdoption:: -z <FILE>

    .. deprecated:: 4.16.6
        Use the ``@<FILE>`` style instead.

    The name of a file containing more command line options.

Command line options can also be placed in a file and passed on the command
line using the ``@`` prefix.
