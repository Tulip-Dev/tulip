/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef _DRAGHANDLE_H
#define _DRAGHANDLE_H

#include <QLabel>

namespace tlp {

class WorkspacePanel;

class DragHandle : public QLabel {
  Q_OBJECT
public:
  explicit DragHandle(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  void mousePressEvent(QMouseEvent *ev) override;
  void mouseReleaseEvent(QMouseEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *ev) override;

  void setPanel(tlp::WorkspacePanel *panel);

private:
  tlp::WorkspacePanel *_panel;
  bool _pressed;
  QPoint _clickPosition;
};
} // namespace tlp
#endif //_DRAGHANDLE_H
///@endcond
