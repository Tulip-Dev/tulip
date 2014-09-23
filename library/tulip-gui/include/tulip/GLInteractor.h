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
#ifndef GLINTERACTOR_H
#define GLINTERACTOR_H

#include <tulip/InteractorComposite.h>

#include <QIcon>

namespace tlp {

class GlMainWidget;

/**
 * @brief The GLInteractorComponent class is an InteractorComponent that can do OpenGL rendering on a GlMainWidget.
 *
 * @warning Only GLInteractorComponent that are stored into a GLInteractorComposite will be able to do OpenGL rendering. Storing them into an InteractorComposite will have no effect.
 *
 * GLInteractorComponent are meant to be stored into a GLInteractorComposite
 * When installed on a GlMainWidget, this interactor will have two new methods getting called:
 * @list
 * @li draw: Draws a visual feedback into the OpenGL scene. This is called after the scene was rendered
 * @li compute: Allows the interactor to do some pre-initialisation steps before the OpenGL scene is rendered.
 * @endlist
 */
class TLP_QT_SCOPE GLInteractorComponent: public tlp::InteractorComponent {
  Q_OBJECT
public slots:

  /**
   * @brief Draws an OpenGL visual feedback for the interactor into a given GlMainWidget.
   * This method is called after the scene was rendered.
   * @return true if the rendering completed sucessfully
   */
  virtual bool draw(tlp::GlMainWidget*) {
    return false;
  }

  /**
   * @brief Initializes the interactor before the scene in the given GlMainWidget is rendered
   */
  virtual bool compute(tlp::GlMainWidget*) {
    return false;
  }
};

/**
 * @brief The GLInteractorComposite class behaves like a InteractorComposite but is meant to store GLInteractorComponent.
 * @warning Only GLInteractorComponent that are stored into a GLInteractorComposite will be able to do OpenGL rendering. Storing them into an InteractorComposite will have no effect.
 */
class TLP_QT_SCOPE GLInteractorComposite: public tlp::InteractorComposite {
  Q_OBJECT

public:
  GLInteractorComposite(const QIcon& icon, const QString& text="");

public slots:
  /**
   * @brief Calls the compute method on every sub-components
   * @note You can subclass this method to add custom behavior before or after components are called.
   */
  virtual void compute(tlp::GlMainWidget*);

  /**
   * @brief Calls the draw method on every sub-components
   * @note You can subclass this method to add custom behavior before or after components are called.
   */
  virtual void draw(tlp::GlMainWidget*);
};

}

#endif // GLINTERACTOR_H
