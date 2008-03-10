//-*-c++-*-
/**
 Author: Morgan Mathiaut and Samuel Carruesco, Mickael Melas, Laurent Peyronnet, Michael Roche, Sabrina Wons
 Email : mathiaut@labri.fr
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

#ifndef _Tulip_RESPONSETREATMENT_H
#define _Tulip_RESPONSETREATMENT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

namespace tlp {

  class TLP_PLUGINSMANAGER_SCOPE  ResponseTreatment {  

  public:
    virtual ~ResponseTreatment(){};
    virtual void operator()(char **data)= 0;
  };

  class TLP_PLUGINSMANAGER_SCOPE  RequestFinish {
   
  public :
    virtual ~RequestFinish(){};
    virtual void operator()()= 0;
  };

}
  
#endif //_RESPONSE_TREATMENT_H
