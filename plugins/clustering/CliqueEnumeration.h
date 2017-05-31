/**
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
#ifndef CLIQUEENUMERATION_H
#define CLIQUEENUMERATION_H

#include <string>
#include <set>
#include <vector>

#include <tulip/Algorithm.h>


/** \addtogroup algorithm */
/*@{*/
/**
 * \file
 * \brief Compute all maximal cliques (or maximal cliques whose size is above a given threshold)
 * according to the Eppstein algorithm. See
 *
 * Eppstein, Loffler and Strash, \n
 * "Listing All Maximal Cliques in Sparse Graphs in Near-optimal Time",\n
 * "Experimental Algorithms", Springer\n
 * "2011"
 *
 *
 *  <b>HISTORY</b>
 *
 *  - 2013 Version 1.0: Initial release
 *  by François Queyroi, LaBRI, University Bordeaux I, France
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */
class CliqueEnumeration:public tlp::Algorithm {
public:

    PLUGININFORMATION("Maximal Cliques", "Francois Queyroi", "24/10/2013", "Alpha", "0.1", "Clustering")

    CliqueEnumeration(tlp::PluginContext*);
    ~CliqueEnumeration();
    bool run();
    bool check(std::string &);

private:

    void addClique(std::vector<tlp::node> &);

    void getDegenerateOrdering(std::vector<tlp::node> &);
    void getNeighborhood(tlp::node,std::set<tlp::node> &);
    tlp::node choosePivot(std::set<tlp::node> &);
    void maxCliquePivot(std::set<tlp::node>&,std::vector<tlp::node>&,std::set<tlp::node>&);

    void intersectSet(std::set<tlp::node>&,std::set<tlp::node>&,std::set<tlp::node>&);
    void unionSet(std::set<tlp::node>&,std::set<tlp::node>&,std::set<tlp::node>&);

    unsigned int minsize;
};
/*@}*/
#endif // CLIQUEENUMERATION_H



