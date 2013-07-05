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
// | inspired for bindings by the Booggie project development team           |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

%ModuleHeaderCode
#include <tulip/Matrix.h>
namespace tlp {
typedef tlp::Matrix<float, 4> Mat4f;
};
%End

namespace tlp {

class Camera : tlp::Observable {
public:

	Camera(tlp::GlScene* scene, tlp::Coord center = tlp::Coord(0, 0, 0), tlp::Coord eyes = tlp::Coord(0, 0, 10), tlp::Coord up = tlp::Coord(0, -1, 0), double zoomFactor = 0.5, double sceneRadius = 10);

	Camera(tlp::GlScene* scene, bool d3);

	void setScene(tlp::GlScene *scene);

	tlp::GlScene *getScene();

	void move(float speed);

	void strafeLeftRight(float speed);

	void strafeUpDown(float speed);

	void rotate(float angle, float x, float y, float z);

	bool is3D();

	tlp::Vec4i getViewport();

	void initGl();

	void initLight();

	void initProjection(const tlp::Vec4i& viewport,bool reset=true);
	
	void initProjection(bool reset = true);

	void initModelView();

	void setSceneRadius(double sceneRadius);
	
	double getSceneRadius();

	void setZoomFactor(double zoomFactor);

	double getZoomFactor();

	void setEyes(const tlp::Coord& eyes);

	tlp::Coord getEyes();

	void setCenter(const tlp::Coord& center);

	tlp::Coord getCenter();

	void setUp(const tlp::Coord& up);

	tlp::Coord getUp();

	void getModelviewMatrix(tlp::Mat4f &modelviewMatrix /Out/);
	
	void getProjectionMatrix(tlp::Mat4f &projectionMatrix /Out/);
	
	void getTransformMatrix(tlp::Mat4f &transformMatrix /Out/);
	
	void getProjAndMVMatrix(const tlp::Vec4i& viewport, tlp::Mat4f &projectionMatrix /Out/, tlp::Mat4f &modelviewMatrix /Out/);
	
	void getTransformMatrix(const tlp::Vec4i& viewport, tlp::Mat4f &transformMatrix /Out/);
	
	tlp::Coord screenTo3DWorld(const tlp::Coord &point);

	tlp::Coord worldTo2DScreen(const tlp::Coord &obj);

};
};
