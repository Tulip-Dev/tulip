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
#include <tulip/GlyphManager.h>
%End


namespace tlp {

	class GlyphContext {
%TypeHeaderCode
#include <tulip/Glyph.h>
%End
  		public:
    		tlp::GlGraphInputData *glGraphInputData;
    		~GlyphContext();
  	};
  	
  	class Glyph {
%TypeHeaderCode
#include <tulip/Glyph.h>
%End
		public :
			Glyph(tlp::GlyphContext *);
    		virtual ~Glyph();
    		virtual void draw(tlp::node n,float lod)=0;
   			
   			virtual void getIncludeBoundingBox(tlp::BoundingBox &boundingBox,tlp::node);

    		virtual void getTextBoundingBox(tlp::BoundingBox &boundingBox,tlp::node n);

    		virtual tlp::Coord getAnchor(const tlp::Coord &nodeCenter, const tlp::Coord &from,
			    const tlp::Size &scale, const double zRotation) const;

    		virtual bool renderLabel();
   			
   			tlp::GlGraphInputData *glGraphInputData;  	
 	};
  	
  	
  	
  	
  	class GlyphFactory : tlp::PluginInfoInterface {
%TypeHeaderCode
#include <tulip/Glyph.h>
%End  	
  	public:
    	virtual ~GlyphFactory();
    
    	virtual tlp::Glyph *createPluginObject(tlp::GlyphContext *gc) = 0 /Factory/;
    
    	virtual int getId() const = 0;
    
    	virtual  std::string getMajor();
    
    	virtual  std::string getMinor();
    
    	virtual  std::string getTulipMajor();
    
    	virtual  std::string getTulipMinor();

    	void registerPlugin();
%MethodCode
	sipCpp->initFactory();
	sipCpp->factory->registerPlugin(sipCpp);
	tlp::GlyphManager::getInst().loadPlugins();
%End
  };

};