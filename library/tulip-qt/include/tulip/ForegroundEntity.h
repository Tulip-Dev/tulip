//-*-c++-*-

/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 24/10/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef Tulip_FOREGROUNDENTITY_H
#define Tulip_FOREGROUNDENTITY_H

namespace tlp {

  /** \brief Abstract class use by entities who want to be displayed in foreground of GlMainWidget
   *
   * This class is use by entities who want to be displayed in foreground of GlMainWidget
   * \warning This entities are very special, this is reserved to entities who can't be in scene (for example rectangle on overview widget)
   */
  class TLP_QT_SCOPE ForegroundEntity {
  public :
    virtual ~ForegroundEntity() {}

    /**
     * function to draw the entity
     */
    virtual void draw(GlMainWidget *glMainWidget) = 0;
  };

}

#endif
