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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+


namespace tlp {
class AbstractView : tlp::View {
%TypeHeaderCode
#include <tulip/AbstractView.h>
%End

public:

	AbstractView();

	virtual QWidget *construct(QWidget *parent);

	QWidget *getWidget();

	virtual void setInteractors(const std::list<tlp::Interactor *> &interactorsList);

	virtual std::list<tlp::Interactor *> getInteractors();

	virtual void setActiveInteractor(tlp::Interactor *interactor);

	tlp::Interactor *getActiveInteractor();

protected:

	void setCentralWidget(QWidget *widget);	
	
public slots:
	bool eventFilter(QObject *object, QEvent *event);
};
};