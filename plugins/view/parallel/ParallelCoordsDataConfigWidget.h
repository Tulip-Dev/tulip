/*
	  ParallelCoordsDataConfigWidget.h

   Created on: 6 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef PARALLELCOORDSDATACONFIGWIDGET_H_
#define PARALLELCOORDSDATACONFIGWIDGET_H_

#include "ParallelCoordsDataConfigWidgetData.h"
#include "ParallelCoordinatesGraphProxy.h"

namespace tlp {

class ParallelCoordsDataConfigWidget : public QWidget, public Ui::ParallelCoordsDataConfigWidgetData {

public :

	ParallelCoordsDataConfigWidget(QWidget *parent = 0);
	ParallelCoordsDataConfigWidget(ParallelCoordinatesGraphProxy *graphProxy, QWidget *parent = 0);

	void setGraphProxy(ParallelCoordinatesGraphProxy *graphProxy);

	const ElementType getDataLocation() const;
	void setDataLocation(const ElementType location);

	std::vector<std::string> getSelectedProperties() const;
	void updateSelectedProperties();

private :

	ParallelCoordinatesGraphProxy *graphProxy;
	std::vector<std::string> propertyTypesFilter;

};

}

#endif /* PARALLELCOORDSDATACONFIGWIDGET_H_ */
