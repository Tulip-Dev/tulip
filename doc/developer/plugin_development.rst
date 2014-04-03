
.. _plugins:

********************
Plug-ins development
********************

Tulip has been built to be easily extensible. Therefore a mechanism of plug-ins has been set up. It enables to directly add new functionalities into the Tulip kernel. One must keeps in mind that a plug-in have access to all the parts of Tulip. Thus, plug-ins have to be written very carefully to prevent memory leak, errors and "core dump". 

To enable the use of plug-ins, a program must call the initialization functions of the plug-ins. This function loads dynamically all the plug-ins and register them into a factory that will enable to direct access to it. 

To develop a plug-in, you need to create a new class that will inherits from a specific algorithm class. They are separated in 4 different types:

* *Property algorithms:* This kind of plug-ins will only affect a specific property of any type. See the section called :ref:`The PropertyAlgorithm class <plugins_property>`.

* *Generic algorithms:* This kind of plug-ins can be used to modify the entire graph. In other case  it is preferable to use a specific class of algorithms to implement your plug-in. See the section called :ref:`The Algorithm class <plugins_algorithm>`.

* *Import algorithms:* Plug-ins to import a specific graph. For example a plug-in that will create a graph from a file directory system. See the section called :ref:`Import plug-ins <plugins_import>`.

* *Export algorithms:* Plug-ins to save a graph to a specific type of file. See the section called :ref:`Export plug-ins <plugins_export>`.


.. _plugins_property:

The PropertyAlgorithm class
===========================

The PropertyAlgorithm class (which inherits of the *Algorithm* class), is the ârent class of different types of algorithms such as the *BooleanAlgorithm* class or the *LayoutAlgorithm* class. This class is important as most of the specific algorithms developed will have to inherit from it. For example, if you write a plug-in to update the graph layout, your class will have to inherit from the *LayoutAlgorithm* class which, itself, inherits from the *PropertyAlgorithm* class.

 Following is a table showing the subclasses of PropertyAlgorithm, with the corresponding, class of result property, the default graph property and the 'Algorithms' group in the GUI :

.. csv-table:: 
   :header: "Class name", "Result class name", "Graph property replaced (by default)", "Algorithms group"

   "BooleanAlgorithm",		"BooleanProperty", 	"viewSelection", 	"Selection"
   "ColorAlgorithm", 		"ColorProperty",	"viewColor", 		"Coloring"
   "DoubleAlgorithm", 		"DoubleProperty",	"viewMetric", 		"Measure"
   "IntegerAlgorithm", 		"IntegerProperty",	"NA",	 		"Measure"
   "LayoutAlgorithm", 		"LayoutProperty", 	"viewLayout", 		"Layout"
   "SizeAlgorithm", 		"SizeProperty",		"viewSize", 		"Resizing"
   "StringAlgorithm", 		"StringProperty", 	"viewLabel", 		"Labeling"

Each one of classes presented above has a public member::

  *TypeName*Property result;

which is the data member that have to be computed by your plug-in. After a successful run Tulip will automatically copy this data member into the corresponding property of the graph.


Note that at anytime, if the user clicks on the "cancel" button (see :ref:`the Plugin Progress class <plugins_pluginprogress>` for more details ), none of your algorithm's actions will changes the graph since the *result* data member is not copied in the corresponding property.

A quick overview of the functions and data members of the class PropertyAlgorithm is needed in order to have a generic understanding of its derived classes.


.. _plugins_property_public:

Public members
--------------

Following is a list of all public members:

* *PropertyAlgorithm (const tlp::PluginContext& context)*: 
  Even if the constructor is used to initialize the inner variables, it is also the right place to declare the parameters needed by the algorithm and to specify how we are going to use them (consult the :ref:`Parameter section <plugins_parameters_add>`).

  If needed, some dependencies can also be specified between two algorithms using the method::

    void addDependency(const char *name, const char *release);

  which allows to declare that the current *PropertyAlgorithm* depends (in terms of programming call) from the *release* version of the *Algorithm*  *release* named *name*. .
* *~PropertyAlgorithm ()*: 
  Destructor of the class.

* *bool run ()*: 
  This is the main method:

    * It will be called out if the pre-condition method (bool check (..)) returned true.
    * It is the starting point of your algorithm. The returned value must be true if your algorithm succeeded.

* *bool check (std::string& errMsg) {return true;}*: 
  This method can be used to check the graph about its topological properties, metric properties on graph elements or any other requirement for the algorithm to run flawlessly. The default implementation inherited from the class *PropertyAlgorithm* returns *true*;


.. _plugins_property_protected:

Protected members
-----------------

Following is a list of all protected members. Each of those are transmitted to the plugin through the *PluginContext* and then restored into the following temporary variables:

* *Graph * graph*:
  This graph is the one on which the algorithm will be applied.

* *PluginProgress * pluginProgress*:
  This instance of the class PluginProgress can be used to have an interaction between the user and our algorithm. See the next section for more details.

* *DataSet * dataSet*:
  This member contains all the parameters needed to run the algorithm. The class DataSet is a container which allows insertion of values of different types. The inserted data must have a copy-constructor well done. See the section called DataSet for more details.

The methods of the *TypeName* Algorithm class, will be redefined in your plug-in as shown in :ref:`the plugin skeleton <plugins_skeleton>`.


.. _plugins_parameters_add:

Adding parameters to an algorithm
---------------------------------

Your algorithm may need some parameters, for example a boolean or a property name, that must be filled in by the user just before being launched. In this section, we will look at the methods and techniques to do so.

The class PropertyAlgorithm provides the following methods to declare a parameter::

  template<typename T>
  void addInParameter(const std::string &name,
                      const std::string &help,
                      const std::string &defaultValue,
                      bool isMandatory = true);

  template<typename T>
  void addOutParameter(const std::string &name,
                       const std::string &help=std::string(),
                       const std::string &defaultValue = std::string(),
                       bool isMandatory = true);

  template<typename T>
  void addInOutParameter(const std::string &name,
                         const std::string &help=std::string(),
                         const std::string &defaultValue = std::string(),
                         bool isMandatory = true);

The new parameters added to the plug-in are, by default, IN parameters. The OUT parameters are defined depending of the *PropertyAlgorithm* used. INOUT parameters gives the developer the abilities to extract additionnal information.

This methods have to be called in the constructor of your class. Following is a description of its fields :

* *name*: Name of the new parameter.
* *help*: This parameter will be used to add a documentation to the parameter (See example below).
* *defaultValue*: Default value.
* *isMandatory*: If false, the user have to set this parameter.

On the following example, we declare a character buffer that will contain the parameter descriptions of the *ColorMapping* algorithm::

  static const char * paramHelp[] = {
    // type
    HTML_HELP_OPEN()         \
    HTML_HELP_DEF("type", "String Collection") \
    HTML_HELP_DEF("values", "linear <BR> uniform <BR> enumerated") \
    HTML_HELP_DEF("default", "linear")   \
    HTML_HELP_BODY() \
    "If linear, the input property must be a Double or Integer property." \
    "The minimum value is mapped to one end of the color scale," \
    "the maximum value is mapped to the other end, and a linear interpolation " \
    "is used between both.<BR>" \
    "If uniform, this is the same except for the interpolation: the value are sorted, " \
    "numbered, and a linear interpolation is used on those numbers" \
    "(in other words, only the order is taken into account, not the actual values).<BR>" \
    "Finally, if enumerated, the input property can be of any type. Each possible " \
    "value is mapped to a distinct color in no specific order." \
    HTML_HELP_CLOSE(),
    // property
    HTML_HELP_OPEN() \
    HTML_HELP_DEF("type", "PropertyInterface") \
    HTML_HELP_BODY() \
    "This property is used to get the values affected to graph items." \
    HTML_HELP_CLOSE(),
    // target
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()         \
    HTML_HELP_DEF("type", "String Collection") \
    HTML_HELP_DEF("values", "nodes <BR> edges") \
    HTML_HELP_DEF("default", "nodes")  \
    HTML_HELP_BODY() \
    "Whether colors are computed for nodes or for edges."  \
    HTML_HELP_CLOSE(),
    // color
    HTML_HELP_OPEN() \
    HTML_HELP_DEF("type", "ColorScale") \
    HTML_HELP_BODY() \
    "Color scale used to transform a scalar into a color." \
    HTML_HELP_CLOSE(),
  };

Then, we can add the parameters in the constructor by writing the following lines::
 
  addInParameter<StringCollection>("type",
                                   paramHelp[0], 
                                   "linear;uniform;enumerated");
  addInParameter<PropertyInterface*>("input property", 
                                     paramHelp[1], 
                                     "viewMetric");
  addInParameter<StringCollection>("target", 
                                   paramHelp[2], 
                                   "nodes;edges");
  addInParameter<ColorScale>("colorScale", 
                             paramHelp[3], 
                             "((75, 75, 255, 200), 
                             (156, 161, 255, 200),
                             (255, 255, 127, 200),
                             (255, 170, 0, 200),
                             (229, 40, 0, 200))");


.. _plugins_parameters_access:

Accessing a parameter
---------------------

The class *PropertyAlgorithm* has a protected member called *dataSet* that contains all the parameters. The *DataSet* class implements a container which allows insertion of values of different types and implements the following methods:

* *Get*::

    template<typename T> bool get(const std::string &key, T& value) const

  Returns a copy of the value of the variable with name. If the variable name doesn't exist return false else true.

* *GetAndFree*::

    template<typename T> bool getAndFree(const std::string &key, T& value)

  Returns a copy of the value of the variable with name name. If the variable name doesn't exist return false else true. The data is removed after the call.

* *Set*::

    template<typename T> void set(const std::string &key, const T& value)

  Set the value of the variable name.

* *Exist*::

    bool exist(const std::string &str) const

  Returns true if name exists else false.

* *GetValues*::

    tlp::Iterator< std::pair<std::string, DataType*> > *getValues() const

  Returns an iterator on all values.

The methods described above are just the core of the functionalities. Consult the Doxygen documentation or the sources for an exhaustive listing.


.. _plugins_pluginprogress:

The PluginProgress class
========================

The class PluginProgress can be used to interact with the user. Following is a list of its members.

.. _plugins_pluginprogress_public:

Public members
--------------

Following is a list of some Public members:

* *ProgressState progress (int step, int max_step)*: 
  This method can be used to know the global progress of an algorithm (the number of steps accomplished).

* *void showPreview (bool showPreview)*: 
  Enables to specify if the preview check box has to be visible or not. 

* *bool isPreviewMode () const*: 
  Enables to know if the user has checked the preview box. 

* *ProgressState state () const*:
  Indicates the state of the 'Cancel', 'Stop' buttons of the dialog

* *void setError (const std::string& error)*:
  Shows an error message to the user

* *void setComment (const std::string& msg)*:
  Shows a comment message to the user


.. _plugins_pluginprogress_example:

Plugin Progress example
-----------------------

In the following small example, we will iterate all the nodes and notify the user of the progression. ::
 
  unsigned int i=0;
  unsigned int nbNodes = graph->numberOfNodes ();
  const unsigned int STEP = 10;
  node n; 
  forEach(n, graph->getNodes(n)) {
    ...
    ... // Do what you want
    ...
    if(i%STEP==0) {
       pluginProgress->progress(i, nbNodes); //Says to the user that the algorithm has progressed.
       //exit if the user has pressed on Cancel or Stop
       if(pluginProgress->state() != TLP_CONTINUE) {
          returnForEach pluginProgress->state()!=TLP_CANCEL;
       }
    }    
    i++;    	
  }

Before exiting, we check if the user pressed stop or cancel. If he pressed "cancel", the graph will not be modified. If he pressed "stop", all values computed till now will be saved to the graph.


.. _plugins_skeleton:

Example of a PropertyAlgorithm skeleton
=======================================

Following is an example of a dummy color algorithm::
 
  #include <tulip/TulipPluginHeaders.h>
  #include <string>
  
  using namespace std;
  using namespace tlp;
  
  /** Algorithm documentation */
  // MyColorAlgorithm is just an example
  
  class MyColorAlgorithm:public ColorAlgorithm { 
  public:

    // This line is used to pass information about the current plug-in.
    PLUGININFORMATION("Name of the Current Algorithm",
                       "Name of the Author",
                       "13/13/13",
                       "A few words describing what kind of action the plug-in realizes",
                       "Plug-in version",
                       "Name of the Sub-menu under which the plug-in should be classified")

    // The constructor below has to be defined,
    // it is the right place to declare the parameters
    // needed by the algorithm,
    //   addInParameter<ParameterType>("Name","Help string","Default value");
    // and declare the algorithm dependencies too.
    //   addDependency("name", "version");
    MyColorAlgorithm(const PluginContext* context):ColorAlgorithm(context) {
    }
  
    // Define the destructor only if needed 
    // ~MyColorAlgorithm() {
    // }
  
    // Define the check method only if needed.
    // It can be used to check topological properties of the graph,
    // metric properties on graph elements or anything else you need.
    // bool check(std::string & errorMsg) {
    //   errorMsg="";
    //   return true;
    // }
  
    // The run method is the main method : 
    //     - It will be called out if the pre-condition method (bool check (..)) returned true.
    //     - It is the starting point of your algorithm.
    // The returned value must be true if your algorithm succeeded.
    bool run() {
      return true;
    }
  };
  // This second line will be used to register your algorithm in tulip using the information given above.
  PLUGIN(MyColorAlgorithm)

The *wizards* directory in the sources also proposes a more dense skeleton in the *tlpalgorithm* folder without all the comments and ready to be transformed into a brand new plugin.

.. _plugins_algorithm:

The Algorithm class
===================

The class *Algorithm* is the class from which your algorithm will inherits if you want to write a more general algorithm. Instead of modifying just a specific property, it can be used to modify several of them. In this section, we will list some of its members to have a global overview on what we can use to develop such a plug-in.


.. _plugins_algorithm_public:

Public members
--------------

Following is a list of all public members :

* *Algorithm (const PluginContext * context)*:
  As previously described, the constructor is the right place to declare the parameters needed by the algorithm::

    addInParameter<DoubleProperty>("metric", paramHelp[0], 0);

  And to declare the algorithm dependencies::

    addDependency("MyGeneralAlgorithm", "Quotient Clustering");

* *~Algorithm ()*:
  Destructor of the class.

* *bool run ()*:
  This is the main method:

  * It will be called out if the pre-condition method (bool check (..)) returned true.
  * It is the starting point of your algorithm. The returned value must be true if your algorithm succeeded.

* *bool check (std::string)*: This method can be used to check what you need about topological properties of the graph, metric properties on graph elements or anything else.

The methods below, will be redefined in our plugin (See section plug-in skeleton).


.. _plugins_algorithm_protected:

Protected members
-----------------

Following is a list of all protected members, similar to the one found in the *PropertyAlgorithm* class:

* *Graph * graph*:
  The graph passed as a parameter containing the data to visualize.

* *PluginProgress * pluginProgress*:
  The class reporting the algorithm evolution (see the section on :ref:`the PluginProgress class <plugins_pluginprogress>`). 

* *DataSet * dataSet*:
  The *dataSet* gathering all the parameters needed to run the algorithm. See the section above detailling :ref:`the DataSet accession <plugins_parameters_access>`.


.. _plugins_import:

Import plug-ins
===============

In this section, we will learn how to create import plug-ins. Those plug-ins will inherit from ImportModule. 


.. _plugins_import_public:

Public members
--------------

Following is a list of all public members :

* *ImportModule (AlgorithmContext context)*:
  Again, the parameters needed by the algorithm must be declared into the constructor (example of *RandomTree.cpp*)::

    addInParameter<unsigned int>("Minimum size",paramHelp[0],"100");
    addInParameter<unsigned int>("Maximum size",paramHelp[1],"1000");
    addInParameter<bool>("tree layout",paramHelp[2],"false");

  And so does the algorithm dependencies::

    addDependency("Tree Leaf", "1.0");

* *~ImportModule ()*:
  Destructor of the class.

* *bool import (const std::string name)*: 
  This is the main method, the starting point of your algorithm. The returned value must be true if your algorithm succeeds.

The methods above must be redefined in our plugin (as shown in the :ref:`import skeleton <plugins_import_skeleton>`).


.. _plugins_import_protected:

Protected members
-----------------

Following is a list of all protected members :

* *Graph * graph*:
  Still the same.

* *PluginProgress * pluginProgress*:
  Likewise (see the section on :ref:`the PluginProgress class <plugins_pluginprogress>`). 

* *DataSet * dataSet*:
  Here too (see the section above detailling :ref:`the DataSet accession <plugins_parameters_access>`).


.. _plugins_import_skeleton:

Skeleton an ImportModule derived class
--------------------------------------

Code example::

  #include <tulip/ImportModule.h>
  #include <string>
  
  using namespace std;
  using namespace tlp;
  
  /** Import module documentation */
  // MyImportModule is just an example
  
  class MyImportModule:public ImportModule { 
  public:

    // This line is used to pass information about the current plug-in.
    PLUGININFORMATION("Name of the Current Import Algorithm",
                       "Name of the Author",
                       "13/13/13",
                       "A few words describing what kind of import the plug-in realizes",
                       "Plug-in version",
                       "Name of the Sub-menu under which the plug-in should be classified")

    // The constructor below has to be defined,
    // it is the right place to declare the parameters
    // needed by the algorithm,
    //   addInParameter<ParameterType>("Name","Help string","Default value");
    // and declare the algorithm dependencies too.
    //   addDependency("name", "version");
    MyImportModule(tlp::PluginContext* context):ImportModule(context) {
    }
  
    // Define the destructor only if needed 
    // ~MyImportModule() {
    // }
  
    // The import method is the starting point of your import module.
    // The returned value must be true if it succeeded.
    bool importGraph() {
      return true;
    }
  };
  // This second line will be used to register your algorithm in tulip using the information given above.
  PLUGIN(MyImportModule)

Just like the *PropertyAlgorithm*, you can find a lighten skeleton in the *tlpimport* folder in the sources, under the *wizards* directory.


.. _plugins_export:

Export plug-ins
===============

In this section, we will learn how to create export plug-ins. Those plug-ins will inherit from *ExportModule*.


.. _plugins_export_public:

Public members
--------------

Following is a list of the usual public members :

* *ExportModule (tlp::PluginContext * context)*:
  Following the examples seen to this point, we still have to use the constructor to add the parameters::

    addInParameter<ParameterType>("Name","Help string","Default value");

  and to declare the algorithm dependencies::

    addDependency("name", "version");

* *~ExportModule ()*:
  Destructor of the class.

* *bool exportGraph(std::ostream &os)*: 
  This is the main method, the starting point of your algorithm. The returned value must be true if your algorithm succeeded.

The methods above will be redefined in our plugin (see the :ref:`export skeleton <plugins_export_skeleton>`).


.. _plugins_export_protected:

Protected members
-----------------

Following is a list of all protected members :

* *Graph * graph*:
  Nothing new here...

* *PluginProgress * pluginProgress*: 
  ...or here... (see the section on :ref:`the PluginProgress class <plugins_pluginprogress>`)

* *DataSet * dataSet*:
  ...or here (see the section above detailling :ref:`the DataSet accession <plugins_parameters_access>`).


.. _plugins_export_skeleton:

Skeleton of an ExportModule derived class
-----------------------------------------

Code example::
 
  #include <tulip/ExportModule.h>
  #include <string>
  #include <iostream>
  
  using namespace std;
  using namespace tlp;
  
  /** Export module documentation */
  // MyExportModule is just an example
  
  class MyExportModule:public ExportModule { 
  public:
  
  // This line is used to pass information about the current plug-in.
  PLUGININFORMATION("Name of the Current Export Algorithm",
                     "Name of the Author",
                     "13/13/13",
                     "A few words describing what kind of export the plug-in realizes",
                     "Plug-in version",
                     "Name of the Sub-menu under which the plug-in should be classified")

    // The constructor below has to be defined,
    // it is the right place to declare the parameters
    // needed by the algorithm,
    //   addInParameter<ParameterType>("Name","Help string","Default value");
    // and declare the algorithm dependencies too.
    //   addDependency("name", "version");
    MyExportModule(tlp::PluginContext* context):ExportModule(context) {
    }

    // Define the destructor only if needed 
    // ~MyExportModule() {
    // }
  
    // The exportGraph method is the starting point of your export module.
    // The returned value must be true if it succeeded.
    bool exportGraph(ostream &os) {
      return true;
    }
  };
  // This second line will be used to register your algorithm in tulip
  // using the information given above.
  PLUGIN(MyExportModule)

A smaller skeleton can be found in the *tlpexport* folder in the sources, in the *wizards* directory.

