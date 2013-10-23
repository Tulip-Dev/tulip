.. _installation_unix:

*******************************
Installation on a UNIX based OS
*******************************

The classic installation details are specified in the *INSTALL.[linux,mac,win]* files available in the tulip sources root directory. However, if you are reading this tutorial/documentation, you may be interested in using Tulip in a more advanced fashion whether by creating new perspectives, plug-ins or algorithms. The following instructions will help you prepare your system by installing the Tulip program ready to be recompiled and modified to your convinience.


.. _installation_how_to:

How to proceed ?
================

The installation process is not complicated, it can however take some time depending on your computer. Before starting the Tulip set-up, as a developer, you must be aware of some of the Tulip intern behaviours. 


Foreword: Debug and Release mods
--------------------------------

Tulip can be compiled and thus executed in two different mods. The first one, called *Release*, is the classic Tulip execution mod. The application should be executed this way when you are only using the originaly integrated tool of Tulip or the add-ons included with the plug-in library. Most of the user will only be interested of this mod.

Nevertheless, because Tulip is a research tool, the users testing their algorithms or plug-ins may not be sure of their creation stability. These persons should be more concerned with the second mod available, the *Debug* mod. With it, the whole program will be slightly slower as the execution is monitored with security assertions. If an unexpected behaviour happens and one of the assertion catch it, Tulip will pop an error message expliciting the problem encounter. 

These mods must be separated as their behaviour are entirely different. A program instance compiled in *Debug* mod can not be used to launch a *Release* version of Tulip and conversely. For instance, a few functionnalities will need assertions only checked in debug mod to run correctly, some of these cases are presented in the :ref:`Tulip library tutorial section <tulip_library_tutorial>`.


Setting up the system
---------------------

If you want to be able to recompile Tulip, you have to organize the different directories which will contain the sources, the libraries and the executables. The disposition we propose here and use in the following steps is just a suggestion. It allows a clear distinction between each directory and there use and gives you the possibility to have several working versions of Tulip at the same time on your station.

First, select the directory in which you want to set the application (you must have the appropriate permissions from the OS) and create there the directory *Tulip-4.X* (where *X* is adapted to the current version number).
Into that folder, create five subdirectories: 

* *tulip-src* : the source files will be kept here
* *build-debug* : where we stock the generated build file (with the mod *Debug*)
* *build-release* : where we stock the generated build file (with the mod *Release*)
* *install-debug* : the program will be installed here (with the mod *Debug*)
* *install-release* : the program will be installed here (with the mod *Release*)

You should obtain a directory hierarchy similar to this one:

.. image:: _images/install_rep.png
    :width: 150


.. _installation_download:

Download the sources
====================

You can obtain the Tulip sources in several ways. The easiest is to pick them directly from the Sourceforge website (by following this `link <http://sourceforge.net/projects/auber/files/tulip/>`_). This method, nevertheless, is not always the best choice. If you want to work actively with Tulip, you will need to keep your project up to date.

The other, slightly more complicated, solution is to use the subversion development repository. This technic, however, allows you to keep Tulip up to date really easily. The Unix/Linux based systems should be able to exploit the `svn tool <http://subversion.apache.org/>`_ with the following commands::

  cd [...]/Tulip-4.X/

  svn checkout svn://svn.code.sf.net/p/auber/code/tulip tulip-src

You can choose the access path and the target dir *tulip-src* according to your preference. Do not forget where the sources are located, you will need the path for completing the compilation and for specifying it to your API.

Once you have import the source code using *svn*, you can easily update it with the commands::

  cd [...]/Tulip-4.X/tulip-src

  svn update


.. _installation_gen_makefiles:

Generate the MakeFiles
======================

Tulip uses *cmake* to generate the compilation instructions. In the following instructions we will use *cmake-gui*, offering a compliant graphic interface, to create the appropriate package files.

You must start by indicating the source code path and the file in which the binaries will be build:

.. image:: _images/install_cmakegui_build_conf.png
    :width: 600

In the example, we are showing the steps for a *Release* type build. The given paths must be modified according to your own preferences.

Then press on the *Configure* button, select the project generator (*Unix Makefiles* by default), use the default compilers or specify new ones and validate. *cmake-gui* will create empty folders in the build directory. We can now specify the installation informations.

.. image:: _images/install_cmakegui_build_generate.png
    :width: 600

The two important fields to fill are the *CMAKE_BUILD_TYPE* and the *CMAKE_INSTALL_PREFIX*. The build type value must be set to *Release* or *Debug* (with capitals) and the install prefix indicates where the program will be installed. We do not want to use the default */user/local/* path as we may wish to exploit different versions of the application. In our case, the install directory will be in *[...]/Tulip-4.3/install-release*.

Click again on the *Configure* button to turn the red colour of the fields off. If this operation failed, ensure you have installed all the appropriate dependencies (a complete list is given in the root directory of the sources), filled appropriatly the fields, then try again.

Press the *Generate* button. If you have correctly set all the values, the two last lines of the prompt should be::

  Configuring done
  Generating done

You can now close *cmake-gui*.
 

.. _installation_use_makefiles:

Use the Makefiles
=================


Compile the packages
--------------------

With the makefiles generated, we just have to execute them. Using your terminal, go into your build directory and use the command::

  make -jY

where Y is the number of core on your processor.

This operation can take several minutes, so here comes the time for a well deserved break!


Install the packages
--------------------

Once the make instruction is finished, you can install Tulip by using the command::

  make install

This operation can take a little time too. At the end, your installation path specified to *cmake-gui* will hold the binaries and the libraries of the application. You can now launch Tulip in its *Release* version by using the commands::

  cd [...]/Tulip-4.3

  install-release/bin/tulip

You now just have to repeat the previously detailled steps while specifying to the appropriate fields the value *debug* instead of *release*. 


.. _installation_unix_qt:

Configurating the IDE QtCreator
===============================

Using a standard text editor to write code is not always the most enjoyable experience. Several integrated development environnement exist and are freely available today, offering to ease our work with useful tools.

The following steps have been done on a `Ubuntu <http://www.ubuntu.com/>`_ distribution (12.04 LTS) with the IDE `QtCreator <http://qt-project.org/>`_ (2.8.1). If you are willing to use another integrated environnement like `Code::Blocks <http://www.codeblocks.org/>`_ or `Eclipse <http://www.eclipse.org/>`_, you may still find some of the informations displayed here to be helpful to guide you through the process.

Before starting, you should have completed all the steps above. Only the **Use the Makefiles** part is optionnal, the compilation being repeated in QtCreator.


Installing QtCreator
--------------------

Obviously, this step is unavoidable. You will need to download the IDE on its official `download page <http://qt-project.org/downloads>`_. You can choose the full Qt library or just the stand-alone QtCreator version. However, picking up the second one will provide you with the tools you need and save you some downloading time.

Once you have the installer, your desktop environment should be able to run it, giving place to a setup wizard. Just follow the installation instructions.

If you can not open the package (with a double-click, for instance), try to execute it through the terminal windows or check if its execution permissions are adapted.


Importing the existing project
------------------------------

Once QtCreator is installed, launch it and create a *New File or Project...* through the *File* menu.

.. image:: _images/install_linux_qt_new.png
    :width: 600

Among the *Projects* type, choose *Import Project* and select the *Import Existing Project* template.

.. image:: _images/install_linux_qt_import.png
    :width: 600

With the click on the *Choose* button, the import window will open, asking you to indicate the project name and its location. The project name is up to you but the location field must refer to the Tulip source directory (in our case: *[...]/Tulip-4.3/tulip-src*).

.. image:: _images/install_linux_qt_location.png
    :width: 500

QtCreator will import a tree view of the source directories and files. Make sure to check the case in front of the tree root to include every file of the project.

.. image:: _images/install_linux_qt_files.png
    :width: 600

This last panel offers to manage your project with a version control system. This choice is yet again up to you. If you do not want to add a version control, proceed by clicking on the *Finish* button. 

.. image:: _images/install_linux_qt_summary.png
    :width: 600

At this point, you have successfully import the project, however, a few more actions need to be done to set the building and running options right. In the coming examples, we want to set up the project for a *release* execution.


Build the project (*Release* version)
-------------------------------------

Once you have imported the project, click on the tab *Projects* located to the left of the window, select the *Build & Run* panel and the *Build Settings*.

Fill up the *Build directory* field (in our case the corresponding path is *[...]/Tulip-4.3/build-release*).

.. image:: _images/install_linux_qt_build_release_browse.png
    :width: 600

For easely find the current build configuration, you can rename it.

.. image:: _images/install_linux_qt_build_release_rename.png
    :width: 600

Then specify the *make arguments*: first come *-jX*, where *X* is the number of core you want to compile the project on at each build; the second is *install*, enabling the information display during compilation.

.. image:: _images/install_linux_qt_build_release_make.png
    :width: 600


Compile the project (*Release* version)
---------------------------------------

With all the build settings done, you can start the project compilation. If you have not realized the step **Use the Makefiles** described above, the operation can take several minutes. However, rest assured, only the first compilation will take this long, the next ones will focus on the updated files and only compile the strict minimum.

To start the compilation, click on the brown hammer located at the bottom-left of the window.

.. image:: _images/install_linux_qt_build_release_compil_start.png
    :width: 600

In the *Edit* view, the *Compile Output* allows you to see the progress made and the file being currently build.

.. image:: _images/install_linux_qt_build_release_compil_progress.png
    :width: 600

With the compilation being successful, you should have a QtCreator window similar to this one: 

.. image:: _images/install_linux_qt_build_release_compil_done.png
    :width: 600


Run the project (*Release* version)
-----------------------------------

The project has been build and compile, we just need to run it to use it. To do so, return to the *Build & Run* panel in the *Project* tab and select the *Run Settings*. 

We will start by changing the name of the *Run Configuration* :

.. image:: _images/install_linux_qt_run_release_rename.png
    :width: 600

In the executable field, indicate the path to the Tulip program (in our case: *[...]/Tulip-4.3/install-release/bin/tulip*):

.. image:: _images/install_linux_qt_run_release_browse.png
    :width: 600

Once the field is filled, run the program by clicking on the green right arrow on the window left-bottom side.

.. image:: _images/install_linux_qt_run_release_run.png
    :width: 600

Please note that, by default, running the project will also recompile the sources. Do not hesitate to consult the QtCreator website to learn more about the IDE and its advanced possibilities.


Change the compile and run configuration
----------------------------------------

The previous steps were exclusive to the *Release* compilation and execution. Now that we have presented the fields and the values to use, you should be able to repeat the same steps for the  *Debug* mod.

In the *Build Settings*, you just have to clone the configuration, rename it and adapt the *Build Directory* value (if you have followed our model, it should be *[...]/Tulip-4.3/build-debug*).

.. image:: _images/install_linux_qt_build_debug_clone.png
    :width: 600

The full setting will look like this:

.. image:: _images/install_linux_qt_build_debug.png
    :width: 600

The *Run Settings* does not offer to clone the current configuration, you will have to fully redo it. Create a new *Custom Excecutable*:

.. image:: _images/install_linux_qt_run_debug_new.png
    :width: 600

Change the path to the appropriate directory (for us: *[...]/Tulip-4.3/install-debug/bin/tulip*) and rename the configuration:.

.. image:: _images/install_linux_qt_run_debug.png
    :width: 600

Before building and running the program, you can choose which type of deployment you want to use:

.. image:: _images/install_linux_qt_deploy_options.png
    :width: 600

Always make sure, when you switch from one to another, to use compatible build and run configurations.
