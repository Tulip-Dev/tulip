/*
 * Authors: Jérémy Compostella, Jean Darracq, Benjamin Muller,
 *          Fabrice Rochambeau, Fabiani Simplice, Jyl Cristoff Zobeide
 * 
 * Email : jcompost@etu.u-bordeaux1.fr, jdarracq@etu.u-bordeaux1.fr,
 *         bmuller@etu.u-bordeaux1.fr, frochamb@etu.u-bordeaux1.fr,
 *         fsimplic@etu.u-bordeaux1.fr, jczobeid@etu.u-bordeaux1.fr.
 *
 * $Id $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

#ifndef BMDLIST_H
#define BMDLIST_H

#ifndef DOXYGEN_NOTFOR_USER

#include <tulip/BmdLink.h>

namespace tlp {
template <typename TYPE>
class BmdList {
 public:
  typedef  tlp::BmdLink<TYPE> BMDTYPE;

  BmdList();
  virtual ~BmdList();
  BMDTYPE *firstItem();
  BMDTYPE *lastItem();
  TYPE entry(BMDTYPE *it);
  int size();
  BMDTYPE *nextItem(BMDTYPE *p, BMDTYPE *predP);
  BMDTYPE *predItem(BMDTYPE *p, BMDTYPE *succP);
  BMDTYPE *cyclicPred(BMDTYPE *it, BMDTYPE *succIt);
  BMDTYPE *cyclicSucc(BMDTYPE *it, BMDTYPE *predIt);
  BMDTYPE *push(const TYPE &a);
  BMDTYPE *append(const TYPE &a);
  TYPE delItem(BMDTYPE *it);
  TYPE pop();
  TYPE popBack();
  void reverse();
  void conc(BmdList<TYPE> &l);
  void clear();
  void swap(BmdList<TYPE> &l);
 private:
  BMDTYPE *head;
  BMDTYPE *tail;
  int count;
};

#include <tulip/cxx/BmdList.cxx>


template<typename TYPE>
struct BmdListIt : public Iterator<TYPE> {

  BmdListIt(BmdList<TYPE> &bmdList):bmdList(bmdList) {
    pos = bmdList.firstItem();
    pred = 0;
  }
  bool hasNext() {
    return pos!=0;
  }
  TYPE next() {
    TYPE val = pos->getData();
    tlp::BmdLink< TYPE > *tmp = pos;
    pos = bmdList.nextItem(pos, pred);
    pred = tmp;
    return val;
  }
 private:
  tlp::BmdLink< TYPE > *pos;
  tlp::BmdLink< TYPE > *pred;
  BmdList<TYPE> &bmdList;
};

template<typename TYPE>
struct BmdListRevIt : public Iterator<TYPE> {
  BmdListRevIt(BmdList<TYPE> &bmdList):bmdList(bmdList) {
    pos = bmdList.lastItem();
    suc = 0;
  }
  bool hasNext() {
    return pos!=0;
  }
  TYPE next() {
    TYPE val = pos->getData();
    tlp::BmdLink< TYPE > *tmp = pos;
    pos = bmdList.predItem(pos, suc);
    suc = tmp;
    return val;
  }
 private:
  tlp::BmdLink< TYPE > *pos;
  tlp::BmdLink< TYPE > *suc;
  BmdList<TYPE> &bmdList;
};
}
#endif
#endif
