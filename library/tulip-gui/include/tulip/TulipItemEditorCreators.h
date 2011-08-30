#ifndef TULIPITEMEDITORCREATORS_H
#define TULIPITEMEDITORCREATORS_H

#include <QtGui/QItemEditorCreatorBase>
#include <tulip/tulipconf.h>

namespace tlp {

class TLP_QT_SCOPE TulipItemEditorCreator: public QItemEditorCreatorBase {
public:
  virtual QString displayText(const QVariant &)=0;
};

}

#endif // TULIPITEMEDITORCREATORS_H
