/*
	  ParallelCoordsElementShowInfos.h

   Created on: 3 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef PARALLELCOORDSELEMENTSHOWINFOS_H_
#define PARALLELCOORDSELEMENTSHOWINFOS_H_

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/InteractorComponent.h>

namespace tlp {

class ParallelCoordsElementShowInfos : public InteractorComponent {
	public :
		ParallelCoordsElementShowInfos(){}
		~ParallelCoordsElementShowInfos(){}
		bool eventFilter(QObject *, QEvent *);
		InteractorComponent *clone() { return new ParallelCoordsElementShowInfos(); }
};

}
#endif /* PARALLELCOORDSELEMENTSHOWINFOS_H_ */
