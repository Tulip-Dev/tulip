// +-------------------------------------------------------------------------+
// | Tulip Software                                                          |
// | Author:  Morgan Mathiaut                                                |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip is free software; you can redistribute it and/or modify           |
// | it under the terms of the GNU General Public License as published by    |
// | the Free Software Foundation; either version 2 of the License, or       |
// | (at your option) any later version.                                     |
// |                                                                         |
// | Tulip is distributed in the hope that it will be useful,                |
// | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with QSimpleSheet; if not, write to the Free Software             |
// | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA|
// |                                                                         |
// +-------------------------------------------------------------------------+

#ifndef Tulip_SPREADLINEEDIT_H
#define Tulip_SPREADLINEEDIT_H

#include <QtGui/QLineEdit>

#include <tulipconf.h>

namespace tlp {

  class TLP_QT_SCOPE SpreadLineEdit : public QLineEdit 
  {
    Q_OBJECT

  public :
    
    SpreadLineEdit(QWidget *parent=NULL):QLineEdit(parent){}

  signals:

    void widgetActivate();

  protected :
    
    void mousePressEvent(QMouseEvent *event) {
      emit widgetActivate();
      QLineEdit::mousePressEvent(event);
    }
  };

}

#endif
