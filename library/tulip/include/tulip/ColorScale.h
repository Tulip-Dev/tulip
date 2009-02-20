/*
	  ColorScale.h

   Created on: 18 févr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef COLORSCALE_H_
#define COLORSCALE_H_

#include <tulip/Observable.h>
#include <tulip/Color.h>

#include <vector>
#include <map>

namespace tlp {

class TLP_SCOPE ColorScale : public Observable {

public:

  ColorScale();
  ColorScale(const ColorScale& scale);
  virtual ~ColorScale();

  virtual void setColorScale(const std::vector<Color> colors, const bool gradient = true);
  virtual Color getColorAtPos(const float pos) const;

  bool colorScaleInitialized() const {return colorScaleSet;}
  std::map<float, Color> getColorMap() const {return colorMap;}

protected:

  std::map<float, Color> colorMap;
  bool gradient;
  bool colorScaleSet;
};

}

#endif /* COLORSCALE_H_ */
