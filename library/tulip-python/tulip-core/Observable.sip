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

class Event {
%TypeHeaderCode
#include <tulip/Observable.h>
%End

%ConvertToSubClassCode
   sipType = NULL;
%End
      
	public:
	
        enum EventType {TLP_DELETE = 0, TLP_MODIFICATION, TLP_INFORMATION, TLP_INVALID};
        
        virtual ~Event();
  
        tlp::Observable* sender() const;
       
        Event(const tlp::Observable &sender, tlp::Event::EventType type);
        
        EventType type() const;
    
    };
    
    
class  OLOObject {
	
public:
	
	tlp::node   getNode() const;

	static tlp::OLOObject* getObject(tlp::node n);
	static bool  isAlive(tlp::node n);

protected:

	OLOObject();
	OLOObject(const tlp::OLOObject &);
	virtual ~OLOObject();   
};
    
class  Observable : tlp::OLOObject {
%TypeHeaderCode
#include <tulip/Observable.h>
%End
    public:
    
        Observable();

        virtual ~Observable();

        virtual void treatEvents(const std::vector<tlp::Event> &events );
        
        virtual void treatEvent(const tlp::Event & /NoCopy/);

  		void addObserver(tlp::Observable *obs) const;

  		void addListener(tlp::Observable *obs) const;

        void  removeObserver(tlp::Observable  *obs) const;
        
        void  removeListener(tlp::Observable  *obs) const;

		unsigned int countObservers() const ;

		unsigned int countListeners() const;

        static void holdObservers();

        static void unholdObservers();

        static unsigned int observersHoldCounter();

    protected:

        void sendEvent(const tlp::Event &);

        void observableDeleted();

};

};













