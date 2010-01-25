/*
 * Authors: Jérémy Compostella, Jean Darracq, Benjamin Muller,
 *          Fabrice Rochambeau, Fabiani Simplice, Jyl Cristoff Zobeide
 * 
 * Email : jcompost@etu.u-bordeaux1.fr, jdarracq@etu.u-bordeaux1.fr,
 *         bmuller@etu.u-bordeaux1.fr, frochamb@etu.u-bordeaux1.fr,
 *         fsimplic@etu.u-bordeaux1.fr, jczobeid@etu.u-bordeaux1.fr.
 *
 * Last modification : $id $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
 */

//=================================================================
template <typename TYPE>
tlp::BmdList<TYPE>::BmdList():head(0),tail(0),count(0) {
}
//=================================================================
template <typename TYPE>
tlp::BmdList<TYPE>::~BmdList() {
  clear();
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::firstItem() {
  return head;
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::lastItem() {
  return tail;
}
//=================================================================
template <typename TYPE>
TYPE tlp::BmdList<TYPE>::entry(tlp::BmdLink<TYPE> *it) {
  return it->data;
}
//=================================================================
template <typename TYPE>
int BmdList<TYPE>::size() {
  return count;
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE>* BmdList<TYPE>::nextItem(tlp::BmdLink<TYPE> *p, tlp::BmdLink<TYPE> *predP) {
  if (p != 0) {
    if (p == tail)
      return 0;
    if (p == head)
      predP = 0;
    if (p->prev() != predP)
      return p->prev();
    else
      return p->succ();
  } else
    return 0;
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::predItem(tlp::BmdLink<TYPE> *p, tlp::BmdLink<TYPE> *succP) {
  if (p != 0) {
    if (p == head)
      return 0;
    if (p == tail)
      succP = 0;
    if (p->succ() != succP)
      return p->succ();
    else
      return p->prev();
  } else
    return 0;    
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::cyclicPred(tlp::BmdLink<TYPE> *it, tlp::BmdLink<TYPE> *succIt) {
  if (it == 0)
    return 0;
  if (it == head)
    return tail;
  if (it == tail)
    succIt = 0;
  return predItem(it, succIt);
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::cyclicSucc(tlp::BmdLink<TYPE> *it, tlp::BmdLink<TYPE> *predIt) {
  if (it == 0)
    return 0;
  if (it == tail)
    return head;
  if (it == head)
    predIt = 0;
  return nextItem(it, predIt);
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::push(const TYPE &data) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  count++;
  if (head != 0) {
    if (head->suc != 0)
      head = head->pre = new tlp::BmdLink<TYPE>(data, 0, head);
    else
      head = head->suc = new tlp::BmdLink<TYPE>(data, 0, head);
  } else
    head = tail = new tlp::BmdLink<TYPE>(data, 0, 0);
  return head;
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::append(const TYPE &data) {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  count++;
  //  std::cerr << "0 - appending : " << data << std::endl;
  if (tail != 0) {
    if (tail->pre != 0)
      tail = tail->suc = new tlp::BmdLink<TYPE>(data, tail, 0);
    else
      tail = tail->pre = new tlp::BmdLink<TYPE>(data, tail, 0);
    //    std::cerr << "1 - appending : " << tail->data << std::endl;
  } else {
    tail = head = new tlp::BmdLink<TYPE>(data, 0, 0);
    //    std::cerr << "2 - appending : " << tail->data << std::endl;
  }
  //  std::cerr << "3 - appending : " << tail->data << std::endl;
  return tail;
}
//=================================================================
template <typename TYPE>
TYPE tlp::BmdList<TYPE>::delItem(tlp::BmdLink<TYPE> *it) {
  assert(it != 0);
  if (it == head)
    return pop();
  if (it == tail)
    return popBack();
  tlp::BmdLink<TYPE> *p = predItem(it, 0);
  tlp::BmdLink<TYPE> *s = nextItem(it, p);
  TYPE x = it->data;
  if (p->pre == it)
    p->pre = s;
  else
    p->suc = s;
  if (s->suc == it)
    s->suc = p;
  else
    s->pre = p;
  count--;
  delete it;
  return x;
}
//=================================================================
template <typename TYPE>
TYPE tlp::BmdList<TYPE>::pop() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  assert(head != 0);
  tlp::BmdLink<TYPE> *x = head;
  head = nextItem(head, 0);
  if (head) {
    if (head->suc == x)
      head->suc = 0;
    else
      head->pre = 0;
  } else
    tail = 0;
  TYPE p = x->data;
  delete x;
  count--;
  return p;
}
//=================================================================
template <typename TYPE>
TYPE tlp::BmdList<TYPE>::popBack() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  assert(head != 0);
  tlp::BmdLink<TYPE> *x= tail;
  tail = predItem(tail, 0);
  if (tail) {
    if (tail->pre == x)
      tail->pre = 0;
    else
      tail->suc = 0;
  }
  else
    head = 0;
  TYPE p = x->data;
  delete x;
  count--;
  return p;
}
//=================================================================
template <typename TYPE>
void tlp::BmdList<TYPE>::reverse() {
  tlp::BmdLink<TYPE> *x = head;
  head = tail;
  tail = x;
}
//=================================================================
template <typename TYPE>
void tlp::BmdList<TYPE>::conc(tlp::BmdList<TYPE> &l) {
  if (head == 0) {
    head = l.head;
    tail = l.tail;
  } 
  else {
    if (tail->pre == 0)
      tail->pre = l.head;
    else
      tail->suc = l.head;
    if (l.head) {
      if (l.head->suc == 0)
	l.head->suc = tail;
      else
	l.head->pre = tail;
      tail = l.tail;
    }
  }
  count += l.count;
  l.head = l.tail = 0;
  l.count = 0;
}
//=================================================================
template <typename TYPE>
void tlp::BmdList<TYPE>::clear() {
  if (head == 0) return;
  tlp::BmdLink<TYPE> *it = head;
  tlp::BmdLink<TYPE> *p = head;
  for (int i = 0 ; i < count ; i++) {
    tlp::BmdLink<TYPE> *x = it;
    it = nextItem(it, p);
    if (p != x)
      delete p;
    p = x;
  }
  delete p;
  count = 0;
  head = tail = 0;
}
//=================================================================
template <typename TYPE>
void tlp::BmdList<TYPE>::swap(tlp::BmdList<TYPE>& l) {
  tlp::BmdLink<TYPE> *tmp;
  int tmp1;
  tmp = l.head;
  l.head = head;
  head = tmp;
  tmp = l.tail;
  l.tail = tail;
  tail = tmp;
  tmp1 = l.count;
  l.count = count;
  count = tmp1;
}
//=================================================================
