#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/GlGraph.h>
#include <vector>


namespace tlp {
  void curveVisibility(const Coord &startPoint,const std::vector<Coord> &bends, const Coord &endPoint,
		       const Size &size, bool &polygon, bool &line, const GLdouble *transformMatrix, const GLint *viewportArray);
  //====================================================================
  void polyLine(const std::vector<Coord> &,/* polyline vertces */
		const Color &,             /* start color */
		const Color &);            /* end color */
  void polyLine(const std::vector<Coord> &, /* polyline vertces */
		const std::vector<Color> &);/* colors at polyline verts */
  void polyQuad(const std::vector<Coord> &, /* polyline vertces */
		const Color &,            /* start color */
		const Color &,            /* end color */
		const float,              /* start size */
		const float,              /* end size */
		const Coord &,            /* nomal to the begin of the curve */
		const Coord &,           /* nomal to the end curve */
		GlGraph *); /*direction of the eyes (only for 3D);*/
  void polyQuad(const std::vector<Coord> &, /* polyline vertces */
		const std::vector<Color> &, /* colors at polyline verts */
		const std::vector<float> &, /* size at polyline verts */
		const Coord &,            /* nomal to the begin of the curve */
		const Coord &,           /* nomal to the end curve */    
		GlGraph *); /*direction of the eyes (only for 3D);*/
  void polyCylinder(const std::vector<Coord> &,    /* polyline vertces */
		    const std::vector<Color> &,    /* colors at polyline verts */
		    const std::vector<float> &,    /* size at polyline verts */
		    const Coord &,            /* nomal to the begin of the curve */
		    const Coord &);           /* nomal to the end curve */
  void polyCylinder(const std::vector<Coord> &,    /* polyline vertces */
		    const Color &,            /* start color */
		    const Color &,            /* end color */
		    const float,              /* start size */
		    const float,              /* end size */
		    const Coord &,            /* nomal to the begin of the curve */
		    const Coord &);           /* nomal to the end curve */        
  //====================================================================
  void bezierCylinder(const std::vector<Coord> &,  /* polyline vertces */
		      const Color &,          /* start color */
		      const Color &,          /* end color */
		      const float,            /* start size */
		      const float,            /* end size */
		      const Coord &,          /* nomal to the begin of the curve */
		      const Coord &);         /* nomal to the end curve */    
  void bezierQuad(const std::vector<Coord> &,  /* polyline vertces */
		  const Color &,          /* start color */
		  const Color &,          /* end color */
		  const float,            /* start size */
		  const float,            /* end size */
		  const Coord &,          /* nomal to the begin of the curve */
		  const Coord &,         /* nomal to the end curve */    
		  GlGraph *); /*direction of the eyes (only for 3D);*/
  void bezierLine(const std::vector<Coord> &,  /* polyline vertces */
		  const Color &,          /* start color */
		  const Color &);          /* end color */
  //====================================================================
  void splineCylinder(const std::vector<Coord> &,  /* polyline vertces */
		      const Color &,          /* start color */
		      const Color &,          /* end color */
		      const float,            /* start size */
		      const float,            /* end size */
		      const Coord &,          /* nomal to the begin of the curve */
		      const Coord &);         /* nomal to the end curve */    
  void splineQuad(const std::vector<Coord> &,  /* polyline vertces */
		  const Color &,          /* start color */
		  const Color &,          /* end color */
		  const float,            /* start size */
		  const float,            /* end size */
		  const Coord &,          /* nomal to the begin of the curve */
		  const Coord &,         /* nomal to the end curve */    
		  GlGraph *); /*direction of the eyes (only for 3D);*/
  void splineLine(const std::vector<Coord> &,  /* polyline vertces */
		  const Color &,          /* start color */
		  const Color &);          /* end color */
  //====================================================================
}
