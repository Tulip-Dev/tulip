/**
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

#include <QApplication>

#include "GoogleMapsViewInteractors.h"

#include "../../utils/StandardInteractorPriority.h"
#include "../../utils/ViewNames.h"

using namespace std;

GoogleMapViewInteractor::GoogleMapViewInteractor(const QString &iconPath, const QString &text) : GLInteractorComposite(QIcon(iconPath), text) {}

bool GoogleMapViewInteractor::isCompatible(const std::string &viewName) const {
  return (viewName == ViewName::GoogleMapsViewName);
}

GoogleMapViewInteractorNavigation::GoogleMapViewInteractorNavigation(const PluginContext *) : GoogleMapViewInteractor(":/tulip/gui/icons/i_navigation.png", "Navigate in view") {

}

unsigned int GoogleMapViewInteractorNavigation::priority() const {
  return StandardInteractorPriority::Navigation;
}

void GoogleMapViewInteractorNavigation::construct() {
  push_back(new GoogleMapViewNavigator);
}

QWidget *GoogleMapViewInteractorNavigation::configurationWidget() const {
  return NULL;
}

GoogleMapViewNavigator::GoogleMapViewNavigator(): x(0), y(0), inRotation(false) {}

GoogleMapViewNavigator::~GoogleMapViewNavigator() {}

void GoogleMapViewNavigator::viewChanged(View *) {
}

void trans(Coord &c1,Coord &c2,float angle1, float angle2) {
  float rho1=sqrt(c1[0]*c1[0]+c1[1]*c1[1]+c1[2]*c1[2]);
  float theta1=acos(c1[2]/rho1);
  float phi1=acos(c1[0]/sqrt(c1[0]*c1[0]+c1[1]*c1[1]));

  float rho2=sqrt(c2[0]*c2[0]+c2[1]*c2[1]+c2[2]*c2[2]);
  float theta2=acos(c2[2]/rho2);
  float phi2=acos(c2[0]/sqrt(c2[0]*c2[0]+c2[1]*c2[1]));

  if(c1[1]<0)
    phi1=2*M_PI-phi1;

  if(c1[0]==0 && c1[1]==0)
    phi1=0;

  if(c2[1]<0)
    phi2=2*M_PI-phi2;

  if(c2[0]==0 && c2[1]==0)
    phi2=0;

  if(theta1+angle1>0.001 && theta1+angle1<M_PI && theta2+angle1>0.001 && theta2+angle1<M_PI) {
    theta1+=angle1;
    theta2+=angle1;

    if(theta2>theta1) {
      float tmp=theta1;
      theta1=theta2;
      theta2=tmp;
    }
  }

  phi2+=angle2;
  phi1=phi2;

  c1[0]=rho1*sin(theta1)*cos(phi1);
  c1[1]=rho1*sin(theta1)*sin(phi1);
  c1[2]=rho1*cos(theta1);

  c2[0]=rho2*sin(theta2)*cos(phi2);
  c2[1]=rho2*sin(theta2)*sin(phi2);
  c2[2]=rho2*cos(theta2);
}

bool GoogleMapViewNavigator::eventFilter(QObject *widget, QEvent *e) {
  GoogleMapsView *googleMapsView=static_cast<GoogleMapsView*>(view());

  if(googleMapsView->viewType()==GoogleMapsView::GoogleRoadMap ||
      googleMapsView->viewType()==GoogleMapsView::GoogleSatellite ||
      googleMapsView->viewType()==GoogleMapsView::GoogleTerrain ||
      googleMapsView->viewType()==GoogleMapsView::GoogleHybrid) {
    QMouseEvent *qMouseEv = dynamic_cast<QMouseEvent *>(e);
    QWheelEvent *qWheelEv = dynamic_cast<QWheelEvent *>(e);

    if(qMouseEv || qWheelEv) {
      GoogleMapsView* googleMapsView=static_cast<GoogleMapsView*>(view());
      QApplication::sendEvent(googleMapsView->getGoogleMap(), e);
    }

    return false;
  }
  else if (googleMapsView->viewType()==GoogleMapsView::Globe) {
    if (e->type() == QEvent::Wheel &&
        (((QWheelEvent *) e)->orientation() == Qt::Vertical)) {
#define WHEEL_DELTA 120
      GlMainWidget *g = (GlMainWidget *) widget;
      g->getScene()->zoomXY(((QWheelEvent *) e)->delta() / WHEEL_DELTA,
                            g->width()/2., g->height()/2.);
      view()->draw();
      return true;
    }

    if (e->type() == QEvent::MouseButtonPress && !inRotation) {
      if(((QMouseEvent*)e)->button()==Qt::LeftButton) {
        x = ((QMouseEvent *) e)->x();
        y = ((QMouseEvent *) e)->y();
        inRotation=true;
        return true;
      }
    }

    if(e->type() == QEvent::MouseButtonRelease) {
      if(((QMouseEvent*)e)->button()==Qt::LeftButton) {
        inRotation=false;
        return true;
      }
    }

    if (e->type() == QEvent::MouseMove && inRotation) {
      GlMainWidget *g = (GlMainWidget *) widget;
      Camera &camera=g->getScene()->getGraphCamera();
      Coord c1=camera.getEyes()-camera.getCenter();
      Coord c2=camera.getEyes()-camera.getCenter()+camera.getUp();
      trans(c1,c2,-0.005*(((QMouseEvent *) e)->y()-y),-0.005*(((QMouseEvent *) e)->x()-x));
      camera.setCenter(Coord(0,0,0));
      camera.setEyes(c1);
      camera.setUp(c2-camera.getEyes());

      x = ((QMouseEvent *) e)->x();
      y = ((QMouseEvent *) e)->y();

      view()->draw();
      return true;
    }

    if (e->type() == QEvent::KeyPress) {
      GlMainWidget *g = (GlMainWidget *) widget;

      float angle1=0;
      float angle2=0;

      switch(((QKeyEvent *) e)->key()) {
      case Qt::Key_Left:
        angle2=-0.05f;
        break;

      case Qt::Key_Right:
        angle2=0.05f;
        break;

      case Qt::Key_Up:
        angle1=0.05f;
        break;

      case Qt::Key_Down:
        angle1=-0.05f;
        break;
      }

      Camera &camera=g->getScene()->getGraphCamera();
      Coord c1=camera.getEyes()-camera.getCenter();
      Coord c2=camera.getEyes()-camera.getCenter()+camera.getUp();
      trans(c1,c2,angle1,angle2);
      camera.setCenter(Coord(0,0,0));
      camera.setEyes(c1);
      camera.setUp(c2-camera.getEyes());

      view()->draw();

      return true;
    }

    return false;
  }
  else {
    return MouseNKeysNavigator::eventFilter(widget, e);
  }
}

PLUGIN(GoogleMapViewInteractorNavigation)
