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
namespace tlp {

template<typename T>
void TulipItemDelegate::registerCreator(tlp::TulipItemEditorCreator *c) {
  int id = qMetaTypeId<T>();

  if (_creators.contains(id))
    return;

  _creators[id] = c;
}

template<typename T>
void TulipItemDelegate::unregisterCreator() {
  int id = qMetaTypeId<T>();

  if (_creators.contains(id))
    _creators.remove(id);
}

template<typename T>
tlp::TulipItemEditorCreator* TulipItemDelegate::creator() const {
  return creator(qMetaTypeId<T>());
}

}
