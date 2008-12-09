/*
 * AxisConfigDialogs.h
 *
 *  Created on: 3 nov. 2008
 *      Author: antoine
 */

#ifndef AXISCONFIGDIALOGS_H_
#define AXISCONFIGDIALOGS_H_

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <QtGui/QDialog>

#include <QtGui/QSpinBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QCloseEvent>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>

#include "QuantitativeParallelAxis.h"
#include "NominalParallelAxis.h"
#include "ItemsDialogList.h"

namespace tlp {

// A really simple dialog which allows user to set number of graduations
// and data ordering (ascendent or not) for a particular quantitative axis
class TLP_QT_SCOPE QuantitativeAxisConfigDialog : public QDialog {

	Q_OBJECT

public :

	QuantitativeAxisConfigDialog(QuantitativeParallelAxis *axis);
	void closeEvent (QCloseEvent * event);

private :

	void initAxisPartitionsTable();

	QuantitativeParallelAxis *axis;
	QSpinBox *nbGrads;
	QComboBox *axisOrder;
	QPushButton *okButton;
	QCheckBox *log10Scale;
};


// A really simple dialog which allows user to set labels order
// on a particular nominative axis
class TLP_QT_SCOPE NominalAxisConfigDialog : public QDialog {

	Q_OBJECT

public :

	NominalAxisConfigDialog(NominalParallelAxis *axis);
	void closeEvent (QCloseEvent * event);

public slots :

	void pressButtonUp();
	void pressButtonDown();

private :

	NominalParallelAxis *axis;
	QPushButton *okButton;
	QHash<QString,itemInfo > hashDataBase;
	ItemsDialogList *axisLabelsOrder;
};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif /* AXISCONFIGDIALOGS_H_ */

