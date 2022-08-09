/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef FACEITERATOR2_H
#define FACEITERATOR2_H

#include <vector>

#include <tulip/Iterator.h>
#include <tulip/Face.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>

//============================================================
/// Face iterator for PlanarConMap
namespace tlp {

class PlanarConMap;

class TLP_SCOPE FaceIterator : public Iterator<Face> {
public:
  FaceIterator(PlanarConMap *m);
  ~FaceIterator() override {}

  Face next() override;
  bool hasNext() override;

private:
  unsigned int i;
  PlanarConMap *mgraph;
};

//============================================================
/// Adjacente Face iterator for PlanarConMap
class TLP_SCOPE FaceAdjIterator : public Iterator<Face> {
public:
  FaceAdjIterator(PlanarConMap *m, const node n);
  ~FaceAdjIterator() override {}

  Face next() override;
  bool hasNext() override;

private:
  std::vector<Face> facesAdj;
  unsigned int i;
};

//============================================================
/// Node of face iterator for PlanarConMap
class TLP_SCOPE NodeFaceIterator : public Iterator<node> {
public:
  NodeFaceIterator(PlanarConMap *m, const Face);
  ~NodeFaceIterator() override {}

  node next() override;
  bool hasNext() override;

private:
  /** contains the set of computed nodes */
  std::vector<node> nodes;
  unsigned int i;
};

//============================================================
/// Edge of face iterator for PlanarConMap
class TLP_SCOPE EdgeFaceIterator : public Iterator<edge> {
public:
  EdgeFaceIterator(PlanarConMap *m, const Face);
  ~EdgeFaceIterator() override {}

  edge next() override;
  bool hasNext() override;

private:
  /** reference on the current face */
  std::vector<edge> ve;
  unsigned int i;
};
} // namespace tlp
#endif

///@endcond
