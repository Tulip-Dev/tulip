/*
 * CurveUtils.cpp
 *
 *  Created on: 16 avr. 2009
 *      Author: tontonnemesis
 */

#include <tulip/GlTextureManager.h>

#include "CurveUtils.h"
#include "FastBezier.h"
#include "ParallelTools.h"

namespace tlp {
//================================================
inline float sqrnorm(const Coord &v) {
	return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}
//================================================
float lineLength(const vector<Coord> &line) {
	float result = 0;
	for (unsigned int i = 1; i < line.size(); ++i) {
		result += sqrnorm(line[i-1] - line[i]);
	}
	return result;
}
//================================================
vector<Color> getColors(const vector<Coord> &line, const Color &c1, const Color &c2) {
	tlp::Vector<float, 4> _c1, _c2;
	for (unsigned int i=0; i<4; ++i) {
		_c1[i] = c1[i];
		_c2[i] = c2[i];
	}
	vector<Color> result(line.size());
	result[0] = c1;
	result[line.size()-1] = c2;
	_c2 -= _c1;
	_c2 /= lineLength(line);
	for (unsigned int i = 1; i < line.size() - 1; ++i) {
		float delta = sqrnorm(line[i-1] - line[i]);
		_c1 += _c2 * delta;
		result[i] = Color((unsigned char)_c1[0], (unsigned char)_c1[1], (unsigned char)_c1[2], (unsigned char)_c1[3]);
	}
	return result;
}
//================================================
vector<float> getSizes(const vector<Coord> &line, float s1, float s2) {
	vector<float> result(line.size());
	result[0] = s1;
	result[line.size()-1] = s2;
	s2 -= s1;
	s2 /= lineLength(line);
	for (unsigned int i = 1; i < line.size() - 1; ++i) {
		float delta = sqrnorm(line[i-1] - line[i]);
		s1 += s2 * delta;
		result[i] = s1;
	}
	return result;
}
//===============================================
void setColor(const Color &c) {
	glColor4ubv((unsigned char *) &c);
}
//====================================================
void setColor(GLfloat *c){
	glColor4fv(c);
}
//====================================================
void setMaterial(const Color &c) {
	float colorMat[4];
	colorMat[0] = ((float)c[0])/255.0;
	colorMat[1] = ((float)c[1])/255.0;
	colorMat[2] = ((float)c[2])/255.0;
	colorMat[3] = ((float)c[3])/255.0;
	setColor(c);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, colorMat);
}
//===============================================
struct CurvePoints {
	GLfloat *data;
	unsigned int size;
	CurvePoints(unsigned int size):size(size) {
		data = new GLfloat[2*size*3];
	}
	void addPoint(){
		GLfloat *newData=new GLfloat[2*(size+1)*3];
		memcpy(newData,data,sizeof(GLfloat)*size*3);
		memcpy(newData+((size+1)*3),data+(size*3),sizeof(GLfloat)*size*3);
		delete[] data;
		data=newData;
		++size;
	}
	Coord& operator()(unsigned int i, unsigned int j) {
		return (Coord&)data[j*size*3+i*3];
	}
};
//================================================
GLfloat* buildCurvePoints (const vector<Coord> &vertices,
		const vector<float> &sizes,
		const Coord &startN,
		const Coord &endN,
		unsigned int &resultSize,
		vector<unsigned int> *dec){
	unsigned int resultDec=0;
	bool inversion=false;
	CurvePoints result(vertices.size());
	//start point
	Coord xu = startN - vertices[0];
	xu /= xu.norm();
	Coord xv = Coord(0,0,1.);
	Coord dir = xu^xv;
	if (fabs (dir.norm()) > 1e-3) dir /= dir.norm();
	result(0,0) = vertices[0] - dir*sizes[0];
	result(0,1) = vertices[0] + dir*sizes[0];
	//============
	for(unsigned int i=1; i< vertices.size() - 1; ++i) {
		Coord xu = vertices[i-1] - vertices[i];
		Coord xv = vertices[i+1] - vertices[i];
		float n_xu = xu.norm();
		float n_xv = xv.norm();
		xu /= n_xu;
		xv /= n_xv;


		Coord bi_xu_xv = xu+xv;
		if(bi_xu_xv ==Coord(0,0,0)) {
			//two point at the same coord
			result(i+resultDec,0)=result(i+resultDec-1,0);
			result(i+resultDec,1)=result(i+resultDec-1,1);
			continue;
		}
		bi_xu_xv /= bi_xu_xv.norm();
		float newSize=sizes[i];
		Coord u=vertices[i-1]-vertices[i];
		Coord v=vertices[i+1]-vertices[i];
		float angle=M_PI-acos((u[0]*v[0]+u[1]*v[1]+u[2]*v[2])/(u.norm()*v.norm()));
		if(isnan(angle))
			angle=M_PI;
		newSize=newSize/cos(angle/2.);

		if(angle<M_PI/2+M_PI/4) {
			//normal form
			if ((xu^xv)[2] > 0) {
				result(i+resultDec,0 + inversion) = vertices[i] + bi_xu_xv*newSize;
				result(i+resultDec,1 - inversion) = vertices[i] - bi_xu_xv*newSize;
			} else {
				result(i+resultDec,0 + inversion) = vertices[i] - bi_xu_xv*newSize;
				result(i+resultDec,1 - inversion) = vertices[i] + bi_xu_xv*newSize;
			}
		}else{
			//broken form
			Coord vectUnit(-bi_xu_xv[1],bi_xu_xv[0],bi_xu_xv[2]);

			if(!(newSize>u.norm() || newSize>v.norm() || fabs(angle-M_PI)<1E-5)) {
				result.addPoint();
				if(dec)
					dec->push_back(i);
				if ((xu^xv)[2] > 0) {
					result(i+resultDec,0 + inversion) = vertices[i] + bi_xu_xv*newSize;
					result(i+resultDec,1 - inversion) = vertices[i] - vectUnit*sizes[1];
					result(i+resultDec+1,0 + inversion) = vertices[i] + bi_xu_xv*newSize;
					result(i+resultDec+1,1 - inversion) = vertices[i] + vectUnit*sizes[i];
				}else{
					result(i+resultDec,1 - inversion) = vertices[i] + bi_xu_xv*newSize;
					result(i+resultDec,0 + inversion) = vertices[i] + vectUnit*sizes[i];
					result(i+resultDec+1,1 - inversion) = vertices[i] + bi_xu_xv*newSize;
					result(i+resultDec+1,0 + inversion) = vertices[i] - vectUnit*sizes[i];
				}
				++resultDec;
			}else{
				if ((xu^xv)[2] > 0) {
					result(i+resultDec,0 + inversion) = vertices[i] + vectUnit*sizes[1];
					result(i+resultDec,1 - inversion) = vertices[i] - vectUnit*sizes[1];
					inversion=!inversion;
				}else{
					result(i+resultDec,1 - inversion) = vertices[i] - vectUnit*sizes[i];
					result(i+resultDec,0 + inversion) = vertices[i] + vectUnit*sizes[i];
					inversion=!inversion;
				}
			}
		}
	}
	//end point
	xu = endN - vertices[vertices.size()-1];
	xu /= xu.norm();
	xv = Coord(0,0,-1);
	dir = xu^xv;
	if (fabs (dir.norm()) > 1e-3) dir /= dir.norm();
	result(vertices.size()-1+resultDec,0 + inversion) = vertices[vertices.size()-1] - dir*sizes[vertices.size()-1];
	result(vertices.size()-1+resultDec,1 - inversion) = vertices[vertices.size()-1] + dir*sizes[vertices.size()-1];
	resultSize=vertices.size()+resultDec;
	return result.data;
}

void drawCurve(const std::vector<Coord> &curvePoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const Coord &startN, const Coord &endN, const std::string &texture) {
	unsigned int size;
	GLfloat *points = buildCurvePoints(curvePoints, getSizes(curvePoints, startSize, endSize), startN, endN ,size);
	vector<Color> curveColors = getColors(curvePoints, startColor, endColor);
	if(texture != "") {
		GlTextureManager::getInst().activateTexture(texture);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBegin(GL_QUAD_STRIP);
	for (unsigned int i = 0; i < size; ++i) {
		setMaterial(curveColors[i]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(&points[i*3]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(&points[i*3 + size*3]);
	}
	glEnd();

	// if no texture set, draw the curve outline for anti-aliasing
	if(texture == "") {
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0 ; i < size ; ++i) {
			setMaterial(curveColors[i]);
			glVertex3fv(&points[i*3]);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0 ; i < size ; ++i) {
			setMaterial(curveColors[i]);
			glVertex3fv(&points[i*3+size*3]);
		}
		glEnd();
	} else {
		GlTextureManager::getInst().desactivateTexture();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	delete [] points;
}
}
