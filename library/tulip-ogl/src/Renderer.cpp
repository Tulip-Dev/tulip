#include "tulip/Renderer.h"
#include "tulip/Fonts.h"

using namespace tlp;

void Renderer::setMode(FontMode  m){  mode = m; }

FontMode Renderer::getMode(){  return mode; }

void Renderer::setDepth(float p){  depth = p; }

float Renderer::getDepth(){  return depth; }

