/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPGPU_H
#define TULIPGPU_H

#include <tulip/tulipconf.h>

namespace tlp {

// create a GpuGraph from a Tulip graph
TLP_GL_SCOPE GpuGraph* genGpuGraph(const tlp::Graph& graph);

// associate a Tulip Property to a gpu property variable
// and compute the needed infos
// return NULL if there is no texture image unit available
// BooleanProperty
TLP_GL_SCOPE GpuProperty* genGpuProperty(tlp::BooleanProperty& property,
    const tlp::Graph& graph,
    GpuValueType type = NODE_VALUES,
    bool outputOnly = false);
// ColorProperty
TLP_GL_SCOPE GpuProperty* genGpuProperty(tlp::ColorProperty& property,
    const tlp::Graph& graph,
    GpuValueType type = NODE_VALUES,
    bool outputOnly = false);
// DoubleProperty
TLP_GL_SCOPE GpuProperty* genGpuProperty(tlp::DoubleProperty& property,
    const tlp::Graph& graph,
    GpuValueType type = NODE_VALUES,
    bool outputOnly = false);
// IntegerProperty
TLP_GL_SCOPE GpuProperty* genGpuProperty(tlp::IntegerProperty& property,
    const tlp::Graph& graph,
    GpuValueType type = NODE_VALUES,
    bool outputOnly = false);
// LayoutProperty (node values only)
TLP_GL_SCOPE GpuProperty* genGpuProperty(tlp::LayoutProperty& property,
    const tlp::Graph& graph,
    bool outputOnly = false);
// SizeProperty
TLP_GL_SCOPE GpuProperty* genGpuProperty(tlp::SizeProperty& property,
    const tlp::Graph& graph,
    GpuValueType type = NODE_VALUES,
    bool outputOnly = false);

// set a parameter of a GpuProgram with a Tulip specific object
// Color
TLP_GL_SCOPE bool setGpuParameter(const std::string& pName, const tlp::Color& val);
// Coord
TLP_GL_SCOPE bool setGpuParameter(const std::string& pName, const tlp::Coord& val);
// Size
TLP_GL_SCOPE bool setGpuParameter(const std::string& pName, const tlp::Size& val);

// these are for the update of a Tulip property with the out property
// BooleanProperty
TLP_GL_SCOPE bool getGpuOutPropertyValues(tlp::BooleanProperty& prop, const tlp::Graph&);
// ColorProperty
TLP_GL_SCOPE bool getGpuOutPropertyValues(tlp::ColorProperty& prop, const tlp::Graph&);
// DoubleProperty
TLP_GL_SCOPE bool getGpuOutPropertyValues(tlp::DoubleProperty& prop, const tlp::Graph&);
// IntegerProperty
TLP_GL_SCOPE bool getGpuOutPropertyValues(tlp::IntegerProperty& prop, const tlp::Graph&);
// LayoutProperty
TLP_GL_SCOPE bool getGpuOutPropertyValues(tlp::LayoutProperty& prop, const tlp::Graph&);
// SizeProperty
TLP_GL_SCOPE bool getGpuOutPropertyValues(tlp::SizeProperty& prop, const tlp::Graph&);

}

#endif
///@endcond
