#include <tulip/GlSphere.h>
#include <tulip/TulipViewSettings.h>

using namespace tlp;

GlSphere::GlSphere(const Coord &position, float radius, const Color &color, float rotX, float rotY, float rotZ) : GlGlyphEntity(NodeShape::Sphere) {
  setCenter(position);
  setSize(Size(radius * 2, radius * 2, radius * 2));
  setColor(color);
  setRotationData(Vec4f(rotX, rotY, rotZ));
  setRotationAxisAndAngle(false);
}

GlSphere::GlSphere(const Coord &position, float radius, const std::string &textureFile, int alpha, float rotX, float rotY, float rotZ)
    : GlGlyphEntity(NodeShape::Sphere) {
  setCenter(position);
  setSize(Size(radius * 2, radius * 2, radius * 2));
  setColor(Color(255, 255, 255, alpha));
  setTexture(textureFile);
  setRotationData(Vec4f(rotX, rotY, rotZ));
  setRotationAxisAndAngle(false);
}
