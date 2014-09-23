/*
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


#ifndef GLAXIS_H_
#define GLAXIS_H_

#include <tulip/GlComposite.h>
#include <tulip/Color.h>

const float DEFAULT_GRAD_WIDTH = 6.;

const float MAGIG_FACTOR = (1.f / (1.3f));

namespace tlp {

class GlLabel;

/**
 * @ingroup OpenGL
 * @brief A base class to draw an axis with graduations
 *
 * This class allow to render an axis with graduations. This class is there for code factorisation
 * and should not be used directly. Use derivated classes instead : GlQuantitativeAxis for a numerical
 * graduated axis and GlNominativeAxis for a string graduated axis
 */
class TLP_GL_SCOPE GlAxis : public GlComposite {

public :

  enum AxisOrientation {HORIZONTAL_AXIS, VERTICAL_AXIS};

  enum LabelPosition {LEFT_OR_BELOW, RIGHT_OR_ABOVE};
  enum CaptionLabelPosition {LEFT, RIGHT, BELOW, ABOVE};

  /**
   * @brief GlAxis constructor
   *
   * @param axisName the name of the axis
   * @param axisBaseCoord the base coord of the axis (if the axis is horizontal, it is the the left end, if vertical it is the down end)
   * @param axisLength the length of the axis
   * @param axisOrientation the orientation of the axis, 2 possible values (HORIZONTAL_AXIS or VERTICAL_AXIS)
   * @param axisColor the color of the axis
   */
  GlAxis(const std::string &axisName, const Coord &axisBaseCoord, const float axisLength,
         const AxisOrientation &axisOrientation, const Color &axisColor);

  /**
   * @brief GlAxis destructor
   */
  virtual ~GlAxis();

  /**
   * @brief Method which returns the base coordinates of the axis
   */
  Coord getAxisBaseCoord() const {
    return axisBaseCoord;
  }
  /**
   * @brief Method which returns the length of the axis
   */
  float getAxisLength() const {
    return axisLength;
  }
  /**
   * @brief Method which returns the name of the axis
   */
  std::string getAxisName() const {
    return axisName;
  }
  /**
   * @brief Method which returns the orientation of the axis
   */
  AxisOrientation getAxisOrientation() const {
    return axisOrientation;
  }
  /**
   * @brief Method which returns the width of the axis graduations
   */
  float getAxisGradsWidth() const {
    return axisGradsWidth;
  }
  /**
   * @brief Method which returns the distance between the axis graduations
   */
  float getSpaceBetweenAxisGrads() const {
    return spaceBetweenAxisGrads;
  }
  /**
   * @brief Method which returns the axis graduations labels height
   */
  float getLabelHeight() const {
    return labelHeight;
  }

  /**
   * @brief Method which returns the max axis graduations labels width
   */
  float getMaxLabelWidth() const {
    return maxGraduationLabelWidth;
  }

  /**
   * @brief Method which returns the color of the axis
   */
  Color getAxisColor() const {
    return axisColor;
  }

  /**
   * @brief Method to set the axis name
   */
  void setAxisName(const std::string &axisName) {
    this->axisName = axisName;
  }
  /**
   * @brief Method to set the axis length
   */
  void setAxisLength(const float axisLength) {
    this->axisLength = axisLength;
  }
  /**
   * @brief Method to set the axis color
   */
  void setAxisColor(const Color &axisColor) {
    this->axisColor = axisColor;
  }
  /**
   * @brief Methods to set the axis graduations Width
   */
  void setAxisGradsWidth(const float axisGradsWidth) {
    this->axisGradsWidth = axisGradsWidth;
  }

  /**
   * @brief Methods to set the max caption width
   */
  void setMaxCaptionWidth(const float maxCaptionWidth) {
    this->maxCaptionWidth = maxCaptionWidth;
  }

  /**
   * @brief Method to update the axis drawing.
   *
   * It has to be called when one (ore more) of the setters methods above has been used
   * This method erase the whole axis drawing and redraw the axis line and the caption (if any)
   * The axis graduations have to be reset by calling setAxisGraduations
   */
  virtual void updateAxis();

  /**
   * @brief Method to set the axis graduations. No need to call updateAxis after calling this method.
   *
   * @param axisGradsLabels the labels of the graduations, they will be equally spaced on the axis
   * @param axisGradsLabelsPosition the relative position of the axis graduations label. Two possible values : LEFT_OR_BELOW (if the axis is horizontal, labels will be on the left of the axis, otherwise below) or RIGHT_OR_ABOVE
   *
   */
  void setAxisGraduations(const std::vector<std::string> &axisGradsLabels,
                          const LabelPosition &axisGradsLabelsPosition = LEFT_OR_BELOW);


  void setAxisGraduationsMaxLabelWidth(const float maxWidth) {
    maxGraduationLabelWidth = maxWidth;
  }

  /**
   * @brief Method which adds a caption to the axis. No need to call updateAxis after calling this method.
   *
   * @param captionPos the relative position of the caption. Two possible values : LEFT_OR_BELOW (if the axis is vertical, caption will be below of the axis, otherwise on the left) or RIGHT_OR_ABOVE
   * @param captionHeight the caption text height
   * @param captionFrame if true the caption will be framed
   * @param maxCaptionWidth fill this parameter if you want to restrain the caption width
   * @param captionOffset fill this parameter if you want to fix the offset between the axis and the caption
   * @param caption if this parameter is filled, use this value as caption text, otherwise the caption text will be the axis name
   */
  void addCaption(const CaptionLabelPosition &captionPos, const float captionHeight, const bool captionFrame = false,
                  const float maxCaptionWidth = 0, const float captionOffset = 0, const std::string caption = "");

  /**
   * @brief Method to set the axis graduations labels size.
   *
   * This method can be used if you want axis with same labels size
   *
   * @param height the height for labels
   *
   */
  void setGradsLabelsHeight(float height);

  void translate(const Coord &c);

  float getCaptionHeight() const;

  void setCaptionHeight(float height, bool frame);

private :

  void buildAxisLine();


protected :

  void computeBoundingBox();
  virtual Coord computeCaptionCenter(const bool captionFrame);
  virtual void computeCaptionSize(float height);
  void addAxisCaption(const Coord &captionLabelCenter, const bool captionFrame);

  std::string axisName;
  Coord axisBaseCoord;
  float axisLength;
  AxisOrientation axisOrientation;
  LabelPosition axisGradsPosition;
  Color axisColor;
  float axisGradsWidth;
  float spaceBetweenAxisGrads;
  float captionWidth;
  float captionHeight;
  float baseCaptionHeight;
  bool captionFrame;
  std::string captionText;
  GlLabel *captionLabel;
  float labelHeight;
  float captionOffset;
  GlComposite *axisLinesComposite;
  GlComposite *captionComposite;
  GlComposite *gradsComposite;
  std::vector<GlLabel*> gradsLabelsVector;
  bool captionSet;
  CaptionLabelPosition captionPosition;
  float maxCaptionWidth;
  float maxGraduationLabelWidth;
};

}

#endif /* GLAXIS_H_ */
