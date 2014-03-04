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

namespace tlp {

  tlp::Interactor *createInteractor(const std::string &interactorName);
%Docstring
tlpgui.createInteractor(interactorName)

.. versionadded:: 4.4

Creates a Tulip interactor. An interactor enables to add user
interaction (usually trough the use of mouse and keyboards) to a Tulip view.
Use function :func:`tlpgui.getInteractorsCompatibleWithView` to get the names
of the interactors compatible with a particular view.
Use method :meth:`tlpgui.View.setInteractors` to modify the default interactors
associated to a view.

:param interactorName: the name of the interactor to create
:type interactorName: string
:rtype: :class:`tlpgui.Interactor`
:throws: an execption if the interactor name does not exist
%End

%MethodCode
    if (tlp::PluginLister::instance()->pluginExists<tlp::Interactor>(*a0)) {
        sipRes = tlp::PluginLister::instance()->getPluginObject<tlp::Interactor>(*a0, NULL);
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Tulip interactor named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

  std::list<std::string> getInteractorsCompatibleWithView(const std::string &viewName);
%Docstring
tlpgui.getInteractorsCompatibleWithView(viewName)

.. versionadded:: 4.4

Returns a list of interactors names compatible with a specific view.

:param viewName: the name of the view for which to retrieve the compatible interactors
:type viewName: string
:rtype: list of string
:throws: an exception if the view name does not exist
%End

%MethodCode
    if (tlp::PluginLister::instance()->pluginExists<tlp::View>(*a0)) {
        QList<std::string> ret = tlp::InteractorLister::compatibleInteractors(*a0);
        sipRes = new std::list<std::string>(ret.begin(), ret.end());
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Tulip view named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

class Interactor : tlp::Plugin /Abstract/ {
%TypeHeaderCode
#include <tulip/Interactor.h>
%End

%Docstring
.. versionadded:: 4.4

Bases: :class:`tulip.tlp.Plugin`

An interactor enables to add user interaction (usually through the use of mouse and keyboards)
to a Tulip view.
Use function :func:`tlpgui.getInteractorsCompatibleWithView` to get the names
of the interactors compatible with a particular view.
Use function :func:`tlpgui.createInteractor` to instantiate a specific interactor.
Use method :meth:`tlpgui.View.setInteractors` to modify the default interactors
associated to a view.
%End

public:

  virtual bool isCompatible(const std::string& viewName) const=0;
%Docstring
tlpgui.Interactor.isCompatible(viewName)

Returns if the interactor is compatible with a specific view.
%End

%MethodCode
    if (tlp::PluginLister::instance()->pluginExists<tlp::View>(*a0)) {
        QList<std::string> ret = tlp::InteractorLister::compatibleInteractors(*a0);
        sipRes = sipCpp->isCompatible(*a0);
    } else {
        sipIsErr = 1;
        std::ostringstream oss;
        oss << "Error : No Tulip view named \"" << *a0 << "\".";
        PyErr_SetString(PyExc_Exception, oss.str().c_str());
    }
%End

  virtual tlp::View* view() const=0;


};

};
