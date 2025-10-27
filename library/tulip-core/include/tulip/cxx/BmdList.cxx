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

//=================================================================
template <typename TYPE>
tlp::BmdList<TYPE>::BmdList() : head(nullptr), tail(nullptr), count(0) {}
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
tlp::BmdLink<TYPE> *BmdList<TYPE>::nextItem(tlp::BmdLink<TYPE> *p, tlp::BmdLink<TYPE> *predP) {
  if (p != nullptr) {
    if (p == tail)
      return nullptr;

    if (p == head)
      predP = nullptr;

    if (p->prev() != predP)
      return p->prev();
    else
      return p->succ();
  } else
    return nullptr;
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::predItem(tlp::BmdLink<TYPE> *p, tlp::BmdLink<TYPE> *succP) {
  if (p != nullptr) {
    if (p == head)
      return nullptr;

    if (p == tail)
      succP = nullptr;

    if (p->succ() != succP)
      return p->succ();
    else
      return p->prev();
  } else
    return nullptr;
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::cyclicPred(tlp::BmdLink<TYPE> *it,
                                                   tlp::BmdLink<TYPE> *succIt) {
  if (it == nullptr)
    return nullptr;

  if (it == head)
    return tail;

  if (it == tail)
    succIt = nullptr;

  return predItem(it, succIt);
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::cyclicSucc(tlp::BmdLink<TYPE> *it,
                                                   tlp::BmdLink<TYPE> *predIt) {
  if (it == nullptr)
    return nullptr;

  if (it == tail)
    return head;

  if (it == head)
    predIt = nullptr;

  return nextItem(it, predIt);
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::push(const TYPE &data) {
  count++;

  if (head != nullptr) {
    if (head->suc != nullptr)
      head = head->pre = new tlp::BmdLink<TYPE>(data, nullptr, head);
    else
      head = head->suc = new tlp::BmdLink<TYPE>(data, nullptr, head);
  } else
    head = tail = new tlp::BmdLink<TYPE>(data, nullptr, nullptr);

  return head;
}
//=================================================================
template <typename TYPE>
tlp::BmdLink<TYPE> *tlp::BmdList<TYPE>::append(const TYPE &data) {
  count++;

  if (tail != nullptr) {
    if (tail->pre != nullptr)
      tail = tail->suc = new tlp::BmdLink<TYPE>(data, tail, nullptr);
    else
      tail = tail->pre = new tlp::BmdLink<TYPE>(data, tail, nullptr);

  } else {
    tail = head = new tlp::BmdLink<TYPE>(data, nullptr, nullptr);
  }

  return tail;
}
//=================================================================
template <typename TYPE>
TYPE tlp::BmdList<TYPE>::delItem(tlp::BmdLink<TYPE> *it) {
  assert(it != nullptr);

  if (it == head)
    return pop();

  if (it == tail)
    return popBack();

  tlp::BmdLink<TYPE> *p = predItem(it, nullptr);
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
  assert(head != nullptr);
  tlp::BmdLink<TYPE> *x = head;
  head = nextItem(head, nullptr);

  if (head) {
    if (head->suc == x)
      head->suc = nullptr;
    else
      head->pre = nullptr;
  } else
    tail = nullptr;

  TYPE p = x->data;
  delete x;
  count--;
  return p;
}
//=================================================================
template <typename TYPE>
TYPE tlp::BmdList<TYPE>::popBack() {
  assert(head != nullptr);
  tlp::BmdLink<TYPE> *x = tail;
  tail = predItem(tail, nullptr);

  if (tail) {
    if (tail->pre == x)
      tail->pre = nullptr;
    else
      tail->suc = nullptr;
  } else
    head = nullptr;

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
  if (head == nullptr) {
    head = l.head;
    tail = l.tail;
  } else {
    if (tail->pre == nullptr)
      tail->pre = l.head;
    else
      tail->suc = l.head;

    if (l.head) {
      if (l.head->suc == nullptr)
        l.head->suc = tail;
      else
        l.head->pre = tail;

      tail = l.tail;
    }
  }

  count += l.count;
  l.head = l.tail = nullptr;
  l.count = 0;
}
//=================================================================
template <typename TYPE>
void tlp::BmdList<TYPE>::clear() {
  if (head == nullptr)
    return;

  tlp::BmdLink<TYPE> *it = head;
  tlp::BmdLink<TYPE> *p = head;

  for (int i = 0; i < count; i++) {
    tlp::BmdLink<TYPE> *x = it;
    it = nextItem(it, p);

    if (p != x)
      delete p;

    p = x;
  }

  delete p;
  count = 0;
  head = tail = nullptr;
}
//=================================================================
template <typename TYPE>
void tlp::BmdList<TYPE>::swap(tlp::BmdList<TYPE> &l) {
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
