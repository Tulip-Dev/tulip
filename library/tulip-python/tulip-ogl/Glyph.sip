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
  	
  	
  	
  	
  	class GlyphFactory : tlp::Plugin {
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