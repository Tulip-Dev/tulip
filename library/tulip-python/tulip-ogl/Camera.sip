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
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert                              |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
// +-------------------------------------------------------------------------+

namespace tlp {

class Camera {
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

	/**
	 * TODO - Vector
	 * Vector<int, 4> getViewport();
	 */

	void initGl();

	void initLight();

	/**
	 * TODO - Vector
	 * void initProjection(const Vector<int, 4>& viewport,bool reset=true);
	 */

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

	void addObjectTransformation(const tlp::Coord &translation, const tlp::Coord &scale, const tlp::Coord &baseCoord);

	void getObjectTransformation(std::vector<tlp::Coord> &translation, std::vector<tlp::Coord> &scale, std::vector<tlp::Coord> &objectCoord);

	bool haveObjectTransformation();

	/**
	 * TODO - Matrix
	 * void getModelviewMatrix(Matrix<float, 4> &modelviewMatrix) { modelviewMatrix=this->modelviewMatrix;}
	 * void getProjectionMatrix(Matrix<float, 4> &projectionMatrix) { projectionMatrix=this->projectionMatrix;}
	 * void getTransformMatrix(Matrix<float, 4> &transformMatrix) { transformMatrix=this->transformMatrix;}
	 * void getProjAndMVMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &projectionMatrix,Matrix<float, 4> &modelviewMatrix);
	 * void getTransformMatrix(const Vector<int, 4>& viewport,Matrix<float, 4> &transformMatrix);
	 */

	tlp::Coord screenTo3DWorld(const tlp::Coord &point);

	tlp::Coord worldTo2DScreen(const tlp::Coord &obj);

	/**
	 * TODO - xmlNodePtr
	 * void getXML(xmlNodePtr rootNode);
	 * void setWithXML(xmlNodePtr rootNode);
	 */
};
};