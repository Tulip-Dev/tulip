/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include "HistoStatsConfigWidget.h"

using namespace std;

namespace tlp {

HistoStatsConfigWidget::HistoStatsConfigWidget(QWidget *parent) : QWidget(parent) {
	setupUi(this);
}

double HistoStatsConfigWidget::getSampleStep() const {
	return sampleStepSB->value();
}

double HistoStatsConfigWidget::getBandwidth() const {
	return windowWidthSP->value();
}

string HistoStatsConfigWidget::getKernelFunctionName() const {
	return kernelFunctionsList->currentText().toStdString();
}

bool HistoStatsConfigWidget::densityEstimation() const {
	return densityEstimationGB->isChecked();
}

bool HistoStatsConfigWidget::displayMeanAndStandardDeviation() const {
	return displayMeanAndSdGB->isChecked();
}

bool HistoStatsConfigWidget::nodesSelection() const {
	bool ret = nodesSelectionGB->isChecked();
	if (ret) nodesSelectionGB->setChecked(false);
	return ret;
}

double HistoStatsConfigWidget::getSelectionLowerBound() const {
	return getBoundFromString(lowerBoundCB->currentText());
}

double HistoStatsConfigWidget::getSelectionUpperBound() const {
	return getBoundFromString(upperBoundCB->currentText());
}

void HistoStatsConfigWidget::setMinMaxMeanAndSd(double min, double max, double mean, double standardDeviation) {
	this->min = min;
	this->max = max;
	this->mean = mean;
	this->standardDeviation = standardDeviation;
	meanValue->setText(QString::number(mean));
	standardDeviationValue->setText(QString::number(standardDeviation));
	lowerBoundCB->clear();
	upperBoundCB->clear();
	lowerBoundCB->addItem("min");
	upperBoundCB->addItem("min");
	bool m3sdOk = false;
	bool m2sdOk = false;
	if ((mean - (3 * standardDeviation)) > min) {
		lowerBoundCB->addItem("m - 3sd");
		upperBoundCB->addItem("m - 3sd");
		m3sdOk = true;
	}
	if ((mean - (2 * standardDeviation)) > min) {
		lowerBoundCB->addItem("m - 2sd");
		upperBoundCB->addItem("m - 2sd");
		m2sdOk = true;
	}
	lowerBoundCB->addItem("m - sd");
	upperBoundCB->addItem("m - sd");
	lowerBoundCB->addItem("m");
	upperBoundCB->addItem("m");
	lowerBoundCB->addItem("m + sd");
	upperBoundCB->addItem("m + sd");

	if (m2sdOk) {
		lowerBoundCB->addItem("m + 2sd");
		upperBoundCB->addItem("m + 2sd");
	}
	if (m3sdOk) {
		lowerBoundCB->addItem("m + 3sd");
		upperBoundCB->addItem("m + 3sd");
	}
	lowerBoundCB->addItem("max");
	upperBoundCB->addItem("max");

	lowerBoundCB->setCurrentIndex(lowerBoundCB->findText("m - sd"));
	upperBoundCB->setCurrentIndex(upperBoundCB->findText("m + sd"));
}

double HistoStatsConfigWidget::getBoundFromString(const QString &bound) const {
	if (bound == "min") {
		return min;
	} else if (bound == "m - 3sd") {
		return (mean - 3 * standardDeviation);
	} else if (bound == "m - 2sd") {
		return (mean - 2 * standardDeviation);
	} else if (bound == "m - sd") {
		return (mean - standardDeviation);
	} else if (bound == "m") {
		return mean;
	} else if (bound == "m + sd") {
		return (mean + standardDeviation);
	} else if (bound == "m + 2sd") {
		return (mean + 2 * standardDeviation);
	} else if (bound == "m + 3sd") {
		return (mean + 3 * standardDeviation);
	} else {
		return max;
	}
}

}
