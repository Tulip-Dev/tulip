/**
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

#include <tulip/BoundingBox.h>

#define THIS_INITED (memcmp(this, &_invalidBB, sizeof(_invalidBB)) != 0)
#define BB_INITED(bb) (memcmp(&bb, &_invalidBB, sizeof(_invalidBB)) != 0)

static bool getIntersection(float fDst1, float fDst2, const tlp::Vec3f &p1, const tlp::Vec3f &p2,
                            tlp::Vec3f &hit) {
  if ((fDst1 * fDst2) >= 0.0f)
    return false;

  if (fDst1 == fDst2)
    return false;

  hit = p1 + (p2 - p1) * (-fDst1 / (fDst2 - fDst1));
  return true;
}

namespace tlp {

static std::array<std::array<float, 3>, 2> _invalidBB{{{{1, 1, 1}}, {{-1, -1, -1}}}};

BoundingBox::BoundingBox() {
  // set as invalid
  memcpy(static_cast<void *>(this), &_invalidBB, sizeof(_invalidBB));
  assert(!isValid());
}

void BoundingBox::clear() {
  // restore an invalid state
  memcpy(static_cast<void *>(this), &_invalidBB, sizeof(_invalidBB));
  assert(!isValid());
}

BoundingBox::BoundingBox(const tlp::Vec3f &min, const tlp::Vec3f &max, bool compute) {
  if (compute)
    tlp::minMaxVectors(min, max, (*this)[0], (*this)[1]);
  else {
    (*this)[0] = min;
    (*this)[1] = max;
  }
  assert(isValid());
}

void BoundingBox::expand(const tlp::Vec3f &coord, bool noCheck) {
  if (noCheck || THIS_INITED) {
    (*this)[0] = tlp::minVector((*this)[0], coord);
    (*this)[1] = tlp::maxVector((*this)[1], coord);
  } else {
    (*this)[0] = coord;
    (*this)[1] = coord;
  }
  assert(isValid());
}

void BoundingBox::expand(const tlp::BoundingBox &bb, bool noCheck) {
  if (noCheck || THIS_INITED) {
    (*this)[0] = tlp::minVector((*this)[0], bb[0]);
    (*this)[1] = tlp::maxVector((*this)[1], bb[1]);
  } else
    *this = bb;
  assert(isValid());
}

void BoundingBox::translate(const tlp::Vec3f &vec) {
  (*this)[0] += vec;
  (*this)[1] += vec;
  assert(isValid());
}

void BoundingBox::scale(const tlp::Vec3f &vec) {
  (*this)[0] *= vec;
  (*this)[1] *= vec;
  assert(isValid());
}

bool BoundingBox::isValid() const {
  return (*this)[0][0] <= (*this)[1][0] && (*this)[0][1] <= (*this)[1][1] &&
         (*this)[0][2] <= (*this)[1][2];
}

bool BoundingBox::contains(const tlp::Vec3f &coord, bool noCheck) const {
  if (noCheck || THIS_INITED) {
    assert(isValid());
    return (coord[0] >= (*this)[0][0] && coord[1] >= (*this)[0][1] && coord[2] >= (*this)[0][2]) &&
           (coord[0] <= (*this)[1][0] && coord[1] <= (*this)[1][1] && coord[2] <= (*this)[1][2]);
  }
  return false;
}

bool BoundingBox::contains(const tlp::BoundingBox &boundingBox) const {
  return THIS_INITED && BB_INITED(boundingBox) && contains(boundingBox[0], true) &&
         contains(boundingBox[1], true);
}

bool BoundingBox::intersect(const tlp::BoundingBox &boundingBox) const {
  if (!THIS_INITED || !BB_INITED(boundingBox))
    return false;

  assert(isValid() && boundingBox.isValid());
  if ((*this)[1][0] < boundingBox[0][0])
    return false;

  if (boundingBox[1][0] < (*this)[0][0])
    return false;

  if ((*this)[1][1] < boundingBox[0][1])
    return false;

  if (boundingBox[1][1] < (*this)[0][1])
    return false;

  if ((*this)[1][2] < boundingBox[0][2])
    return false;

  if (boundingBox[1][2] < (*this)[0][2])
    return false;

  return true;
}

bool BoundingBox::intersect(const Vec3f &segStart, const Vec3f &segEnd) const {
  if (!THIS_INITED)
    return false;

  assert(isValid());
  if (segEnd[0] < (*this)[0][0] && segStart[0] < (*this)[0][0])
    return false;

  if (segEnd[0] > (*this)[1][0] && segStart[0] > (*this)[1][0])
    return false;

  if (segEnd[1] < (*this)[0][1] && segStart[1] < (*this)[0][1])
    return false;

  if (segEnd[1] > (*this)[1][1] && segStart[1] > (*this)[1][1])
    return false;

  if (segEnd[2] < (*this)[0][2] && segStart[2] < (*this)[0][2])
    return false;

  if (segEnd[2] > (*this)[1][2] && segStart[2] > (*this)[1][2])
    return false;

  if (segStart[0] > (*this)[0][0] && segStart[0] < (*this)[1][0] && segStart[1] > (*this)[0][1] &&
      segStart[1] < (*this)[1][1] && segStart[2] > (*this)[0][2] && segStart[2] < (*this)[1][2])
    return true;

  Vec3f hit;

  if ((getIntersection(segStart[0] - (*this)[0][0], segEnd[0] - (*this)[0][0], segStart, segEnd,
                       hit) &&
       contains(hit, true)) ||
      (getIntersection(segStart[1] - (*this)[0][1], segEnd[1] - (*this)[0][1], segStart, segEnd,
                       hit) &&
       contains(hit, true)) ||
      (getIntersection(segStart[2] - (*this)[0][2], segEnd[2] - (*this)[0][2], segStart, segEnd,
                       hit) &&
       contains(hit, true)) ||
      (getIntersection(segStart[0] - (*this)[1][0], segEnd[0] - (*this)[1][0], segStart, segEnd,
                       hit) &&
       contains(hit, true)) ||
      (getIntersection(segStart[1] - (*this)[1][1], segEnd[1] - (*this)[1][1], segStart, segEnd,
                       hit) &&
       contains(hit, true)) ||
      (getIntersection(segStart[2] - (*this)[1][2], segEnd[2] - (*this)[1][2], segStart, segEnd,
                       hit) &&
       contains(hit, true)))
    return true;

  return false;
}

void BoundingBox::getCompleteBB(Vec3f bb[8]) const {
  bb[0] = (*this)[0];
  bb[1] = (*this)[0];
  bb[1][0] = (*this)[1][0];
  bb[2] = bb[1];
  bb[2][1] = (*this)[1][1];
  bb[3] = (*this)[0];
  bb[3][1] = (*this)[1][1];
  bb[4] = bb[0];
  bb[4][2] = (*this)[1][2];
  bb[5] = bb[1];
  bb[5][2] = (*this)[1][2];
  bb[6] = bb[2];
  bb[6][2] = (*this)[1][2];
  bb[7] = bb[3];
  bb[7][2] = (*this)[1][2];
}
} // namespace tlp
