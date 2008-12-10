#include "AxisConfigDialogs.h"

#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>

#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtCore/QString>

using namespace std;

namespace tlp {

QuantitativeAxisConfigDialog::QuantitativeAxisConfigDialog(QuantitativeParallelAxis *axis) : QDialog(), axis(axis) {
	setWindowTitle("Quantitative Axis Configuration");

	QVBoxLayout *dialogLayout = new QVBoxLayout();
	QHBoxLayout *nbGradsLayout = new QHBoxLayout();
	QHBoxLayout *axisOrderLayout = new QHBoxLayout();
	QHBoxLayout *okButtonLayout = new QHBoxLayout();

	nbGrads = new QSpinBox();
	okButton = new QPushButton("OK");
	axisOrder = new QComboBox();
	log10Scale = new QCheckBox("Logarithmic scale (base 10)");
	log10Scale->setChecked(axis->hasLog10Scale());

	nbGrads->setRange(10, axis->getAxisHeight());
	nbGrads->setValue(axis->getNbAxisGrad());
	nbGradsLayout->addWidget(new QLabel("Number of graduations : "));
	nbGradsLayout->addWidget(nbGrads);

	axisOrder->addItem("ascending");
	axisOrder->addItem("descending");

	if (axis->hasAscendindOrder()) {
		axisOrder->setCurrentIndex(0);
	} else {
		axisOrder->setCurrentIndex(1);
	}

	axisOrderLayout->addWidget(new QLabel("Axis order : "));
	axisOrderLayout->addWidget(axisOrder);

	dialogLayout->addLayout(nbGradsLayout);
	dialogLayout->addLayout(axisOrderLayout);
	dialogLayout->addWidget(log10Scale);
	okButtonLayout->addStretch(1);
	okButtonLayout->addWidget(okButton);
	okButtonLayout->addStretch(1);
	dialogLayout->addLayout(okButtonLayout);

	setLayout(dialogLayout);
	connect(okButton,SIGNAL(clicked()),this,SLOT(close()));

	resize(300, 100);

}

void QuantitativeAxisConfigDialog::closeEvent (QCloseEvent * event) {
	axis->setNbAxisGrad(nbGrads->value());
	axis->setAscendindOrder(axisOrder->currentText() == "ascending");
	axis->setLog10Scale(log10Scale->isChecked());
	axis->redraw();
}

NominalAxisConfigDialog::NominalAxisConfigDialog(NominalParallelAxis *axis) : QDialog(), axis(axis) {

	setWindowTitle("Nominal Axis Configuration");

	QVBoxLayout *dialogLayout = new QVBoxLayout();
	QVBoxLayout *buttonsUpDownLayout = new QVBoxLayout();
	QHBoxLayout *labelsOrderLayout = new QHBoxLayout();
	QHBoxLayout *okButtonLayout = new QHBoxLayout();

	okButton = new QPushButton("OK");
	axisLabelsOrder = new ItemsDialogList(this);
	QPushButton* up = new QPushButton(QIcon(":/up.png"),"");
	QPushButton *down = new QPushButton(QIcon(":/down.png"),"");

	buttonsUpDownLayout->addWidget(up);
	buttonsUpDownLayout->addWidget(down);

	labelsOrderLayout->addWidget(axisLabelsOrder);
	labelsOrderLayout->addLayout(buttonsUpDownLayout);

	dialogLayout->addWidget(new QLabel(tr("Axis Labels Order :")));
	dialogLayout->addLayout(labelsOrderLayout);
	okButtonLayout->addStretch(1);
	okButtonLayout->addWidget(okButton);
	okButtonLayout->addStretch(1);
	dialogLayout->addLayout(okButtonLayout);

	vector<string> labelsOrder = axis->getLabelsOrder();
	vector<string>::iterator it;
	for (it = labelsOrder.begin() ; it != labelsOrder.end() ; ++it) {
		axisLabelsOrder->addItem(QString((*it).c_str()));
	}

	setLayout(dialogLayout);

	connect(okButton,SIGNAL(clicked()),this,SLOT(close()));
	connect(up,SIGNAL(clicked()),this,SLOT(pressButtonUp()));
	connect(down,SIGNAL(clicked()),this,SLOT(pressButtonDown()));
}

void NominalAxisConfigDialog::closeEvent (QCloseEvent * event) {
	vector<string> labelsOrder;
	for	(int i = 0; i < axisLabelsOrder->count(); ++i) {
		labelsOrder.push_back(axisLabelsOrder->item(i)->text().toStdString());
	}
	axis->setLabelsOrder(labelsOrder);
	axis->redraw();
}

void NominalAxisConfigDialog::pressButtonUp() {
	if (axisLabelsOrder->count() > 0) {
		int row = axisLabelsOrder->currentRow();
		if (row > 0) {
			QString s = axisLabelsOrder->currentItem()->text();
			QString s2 = axisLabelsOrder->item(row - 1)->text();
			axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row - 1));
			axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row - 1));
			axisLabelsOrder->insertItem(row - 1, s2);
			axisLabelsOrder->insertItem(row - 1, s);
			axisLabelsOrder->setCurrentRow(row - 1);
		}
	}
}

void NominalAxisConfigDialog::pressButtonDown() {
	if (axisLabelsOrder->count() > 0) {
		int row = axisLabelsOrder->currentRow();
		if (row < (axisLabelsOrder->count() - 1)) {
			QString s = axisLabelsOrder->currentItem()->text();
			QString s2 = axisLabelsOrder->item(row + 1)->text();
			axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row));
			axisLabelsOrder->deleteItemList(axisLabelsOrder->item(row));
			axisLabelsOrder->insertItem(row, s);
			axisLabelsOrder->insertItem(row, s2);
			axisLabelsOrder->setCurrentRow(row + 1);
		}
	}
}

}
