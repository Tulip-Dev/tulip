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

#include <tulip/ColorScale.h>

using namespace std;

namespace tlp {


ColorScale::ColorScale(const bool gradient) :
  gradient(gradient), colorScaleSet(false) {
  colorMap[0.0f] = Color(75, 75, 255, 200);
  colorMap[0.25f] = Color(156, 161, 255, 200);
  colorMap[0.5f] = Color(255, 255, 127, 200);
  colorMap[0.75f] = Color(255, 170, 0, 200);
  colorMap[1.0f] = Color(229, 40, 0, 200);
}

ColorScale::ColorScale(const std::vector<Color> &colors, const bool gradient) :
  gradient(gradient), colorScaleSet(true) {
  setColorScale(colors, gradient);
}

ColorScale::ColorScale(const ColorScale& scale) : Observable() {
  setColorMap(scale.colorMap);
  gradient = scale.gradient;
}

ColorScale& ColorScale::operator=(const ColorScale& scale) {
  setColorMap(scale.colorMap);
  gradient = scale.gradient;
  return *this;
}

ColorScale::~ColorScale() {
}

void ColorScale::setColorScale(const std::vector<Color> colors,
                               const bool gradientV) {
  gradient = gradientV;
  colorMap.clear();

  if (!colors.empty()) {
    colorScaleSet = true;

    if (colors.size() == 1) {
      colorMap[0.0f] = colors[0];
      colorMap[1.0f] = colors[0];
    }
    else {
      float shift;

      if (gradient) {
        shift = 1.0f / (colors.size() - 1);
      }
      else {
        shift = 1.0f / colors.size();
      }

      for (size_t i = 0; i < colors.size(); ++i) {
        //Ensure that the last color will be set to 1
        if (i == colors.size() - 1) {
          if (!gradient) {
            colorMap[1.0f - shift] = colors[i];
          }

          colorMap[1.0f] = colors[i];
        }
        else {
          colorMap[i * shift] = colors[i];

          if (!gradient) {
            colorMap[((i + 1) * shift) - 1E-6] = colors[i];
          }
        }
      }
    }

    sendEvent(Event(*this, Event::TLP_MODIFICATION));
  }
}

void ColorScale::setColorAtPos(const float pos, const Color &color) {
  colorMap[pos] = color;
  colorScaleSet = true;
}

Color ColorScale::getColorAtPos(const float pos) const {
  if (colorMap.size() == 0) {
    return Color(255, 255, 255, 255);
  }
  else {
    Color startColor;
    Color endColor;
    float startPos, endPos;
    map<float, Color>::const_iterator it = colorMap.begin();
    startPos = endPos = it->first;
    startColor = endColor = it->second;

    for (++it; it != colorMap.end(); ++it) {
      endColor = it->second;
      endPos = it->first;

      if (pos >= startPos && pos <= endPos) {
        break;
      }
      else {
        startColor = endColor;
        startPos = endPos;
      }
    }


    if (gradient) {
      Color ret;
      double ratio = (pos - startPos) / (endPos - startPos);

      for (unsigned int i = 0; i < 4; ++i) {
        ret[i] = static_cast<unsigned char>((double(startColor[i]) + (double(endColor[i])
                                             - double(startColor[i])) * ratio));
      }

      return ret;
    }
    else {
      return startColor;
    }

  }
}

void ColorScale::setColorMap(const map<float, Color>& newColorMap) {
  colorScaleSet = false;

  colorMap.clear();

  // insert all values in [0, 1]
  for(map<float, Color>::const_iterator it = newColorMap.begin();
      it != newColorMap.end(); ++it) {
    if ((*it).first < 0.f || (*it).first > 1.f)
      continue;
    else
      colorMap[(*it).first] = (*it).second;
  }

  if(!colorMap.empty()) {
    //Ensure color scale is valid
    if(colorMap.size()==1) {
      //If there is only one value in the map fill the interval with the whole color.
      Color c = (*colorMap.begin()).second;
      colorMap.clear();
      colorMap[0.f]=c;
      colorMap[1.f]=c;
    }
    else {
      //Ensure the first value is mapped to 0 and last is mapped to 1
      map<float,Color>::iterator begin = colorMap.begin();

      if((*begin).first != 0) {
        Color c = (*begin).second;
        colorMap.erase(begin);
        colorMap[0.f]=c;
      }

      map<float,Color>::reverse_iterator end = colorMap.rbegin();

      if((*end).first != 1) {
        Color c = (*end).second;
        colorMap.erase(end.base());
        colorMap[1.f]=c;
      }
    }

    colorScaleSet = true;
  }
  else {
    colorScaleSet = false;
  }
}

void ColorScale::setColorMapTransparency(unsigned char alpha) {
  //force the alpha value of all mapped colors
  for(map<float, Color>::iterator it = colorMap.begin();
      it != colorMap.end(); ++it) {
    Color& color = it->second;
    color.setA(alpha);
  }
}
}
