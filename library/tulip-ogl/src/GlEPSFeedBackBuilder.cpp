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
#include <tulip/GlEPSFeedBackBuilder.h>

using namespace std;

namespace tlp {

#define EPS_GOURAUD_THRESHOLD 0.5  /* Lower for better (slower) */

//====================================================
static const char *gouraudtriangleEPS[] = {
  "/bd{bind def}bind def /triangle { aload pop   setrgbcolor  aload pop 5 3",
  "roll 4 2 roll 3 2 roll exch moveto lineto lineto closepath fill } bd",
  "/computediff1 { 2 copy sub abs threshold ge {pop pop pop true} { exch 2",
  "index sub abs threshold ge { pop pop true} { sub abs threshold ge } ifelse",
  "} ifelse } bd /computediff3 { 3 copy 0 get 3 1 roll 0 get 3 1 roll 0 get",
  "computediff1 {true} { 3 copy 1 get 3 1 roll 1 get 3 1 roll 1 get",
  "computediff1 {true} { 3 copy 2 get 3 1 roll  2 get 3 1 roll 2 get",
  "computediff1 } ifelse } ifelse } bd /middlecolor { aload pop 4 -1 roll",
  "aload pop 4 -1 roll add 2 div 5 1 roll 3 -1 roll add 2 div 3 1 roll add 2",
  "div 3 1 roll exch 3 array astore } bd /gouraudtriangle { computediff3 { 4",
  "-1 roll aload 7 1 roll 6 -1 roll pop 3 -1 roll pop add 2 div 3 1 roll add",
  "2 div exch 3 -1 roll aload 7 1 roll exch pop 4 -1 roll pop add 2 div 3 1",
  "roll add 2 div exch 3 -1 roll aload 7 1 roll pop 3 -1 roll pop add 2 div 3",
  "1 roll add 2 div exch 7 3 roll 10 -3 roll dup 3 index middlecolor 4 1 roll",
  "2 copy middlecolor 4 1 roll 3 copy pop middlecolor 4 1 roll 13 -1 roll",
  "aload pop 17 index 6 index 15 index 19 index 6 index 17 index 6 array",
  "astore 10 index 10 index 14 index gouraudtriangle 17 index 5 index 17",
  "index 19 index 5 index 19 index 6 array astore 10 index 9 index 13 index",
  "gouraudtriangle 13 index 16 index 5 index 15 index 18 index 5 index 6",
  "array astore 12 index 12 index 9 index gouraudtriangle 17 index 16 index",
  "15 index 19 index 18 index 17 index 6 array astore 10 index 12 index 14",
  "index gouraudtriangle 18 {pop} repeat } { aload pop 5 3 roll aload pop 7 3",
  "roll aload pop 9 3 roll 4 index 6 index 4 index add add 3 div 10 1 roll 7",
  "index 5 index 3 index add add 3 div 10 1 roll 6 index 4 index 2 index add",
  "add 3 div 10 1 roll 9 {pop} repeat 3 array astore triangle } ifelse } bd",
  NULL
};

struct Feedback3DColor {
  float x, y, z, r, g, b, a;
};

void GlEPSFeedBackBuilder::begin(const Vector<int, 4> &viewport,GLfloat*,GLfloat,GLfloat lineWidth) {
  /* Emit EPS header. */
  stream_out << "%%!PS-Adobe-2.0 EPSF-2.0" << endl;
  /* Notice %% for a single % in the fprintf calls. */
  stream_out << "%%%%Creator: " << "rendereps" << " (using OpenGL feedback) " << endl ;
  stream_out << "%%%%BoundingBox: " << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << endl;
  stream_out << "%%EndComments" << endl << endl;
  stream_out << "gsave" << endl << endl;

  /* Output Frederic Delhoume's "gouraudtriangle" PostScript
     fragment. */
  stream_out << "%% the gouraudtriangle PostScript fragement below is free" << endl;
  stream_out << "%% written by Frederic Delhoume (delhoume@ilog.fr)" << endl ;
  stream_out << "/threshold " << EPS_GOURAUD_THRESHOLD << " def" << endl;

  for (int i = 0; gouraudtriangleEPS[i]; i++) {
    stream_out << gouraudtriangleEPS[i] << endl;
  }

  stream_out << endl << lineWidth << " setlinewidth" << endl;

  /* Clear the background like OpenGL had it. */
  stream_out << "1.0 1.0 1.0 setrgbcolor" << endl;
  stream_out << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << " rectfill" << endl << endl;
}
void GlEPSFeedBackBuilder::colorInfo(GLfloat* data) {
  fillColor[0]=(unsigned char)data[0];
  fillColor[1]=(unsigned char)data[1];
  fillColor[2]=(unsigned char)data[2];
  fillColor[3]=(unsigned char)data[3];
  strokeColor[0]=(unsigned char)data[4];
  strokeColor[1]=(unsigned char)data[5];
  strokeColor[2]=(unsigned char)data[6];
  strokeColor[3]=(unsigned char)data[7];
  textColor[0]=(unsigned char)data[8];
  textColor[1]=(unsigned char)data[9];
  textColor[2]=(unsigned char)data[10];
  textColor[3]=(unsigned char)data[11];
}
void GlEPSFeedBackBuilder::beginGlGraph(GLfloat) {
}
void GlEPSFeedBackBuilder::endGlGraph() {
}
void GlEPSFeedBackBuilder::beginGlEntity(GLfloat) {
}
void GlEPSFeedBackBuilder::endGlEntity() {
}
void GlEPSFeedBackBuilder::beginNode(GLfloat) {
}
void GlEPSFeedBackBuilder::endNode() {
}
void GlEPSFeedBackBuilder::beginEdge(GLfloat) {
}
void GlEPSFeedBackBuilder::endEdge() {
}
void GlEPSFeedBackBuilder::pointToken(GLfloat *data) {
  Feedback3Dcolor *vertex = (Feedback3Dcolor *) data;
  stream_out << vertex[0].red << " " << vertex[0].green << " " << vertex[0].blue << " setrgbcolor" << endl;
  stream_out << vertex[0].x << " " <<  vertex[0].y << " " <<  pointSize / 2.0 << " 0 360 arc fill" << endl << endl;
}
void GlEPSFeedBackBuilder::lineToken(GLfloat *data) {
  Feedback3Dcolor *vertex = (Feedback3Dcolor *) data;
  GLfloat dx, dy, dr, dg, db, absR, absG, absB, colormax;
  int steps;
  GLfloat xstep, ystep, rstep, gstep, bstep;
  xstep=ystep=rstep=gstep=bstep=0;
  GLfloat xnext, ynext, rnext, gnext, bnext, distance;
  xnext=ynext=rnext=gnext=bnext=distance=0;

  dr = vertex[1].red - vertex[0].red;
  dg = vertex[1].green - vertex[0].green;
  db = vertex[1].blue - vertex[0].blue;

  if (dr != 0 || dg != 0 || db != 0) {
    /* Smooth shaded line. */
    dx = vertex[1].x - vertex[0].x;
    dy = vertex[1].y - vertex[0].y;
    distance = sqrt(dx * dx + dy * dy);

    absR = fabs(dr);
    absG = fabs(dg);
    absB = fabs(db);

#define Max(a,b) (((a)>(b))?(a):(b))

#define EPS_SMOOTH_LINE_FACTOR 1  /* Upper for better smooth
    lines. */
    colormax = Max(absR, Max(absG, absB));
    steps =(int) rint(Max(1.0, colormax * distance * EPS_SMOOTH_LINE_FACTOR));

    xstep = dx / steps;
    ystep = dy / steps;

    rstep = dr / steps;
    gstep = dg / steps;
    bstep = db / steps;

    xnext = vertex[0].x;
    ynext = vertex[0].y;
    rnext = vertex[0].red;
    gnext = vertex[0].green;
    bnext = vertex[0].blue;

    /* Back up half a step; we want the end points to be
       exactly the their endpoint colors. */
    xnext -= xstep / 2.0;
    ynext -= ystep / 2.0;
    rnext -= rstep / 2.0;
    gnext -= gstep / 2.0;
    bnext -= bstep / 2.0;
  }
  else {
    /* Single color line. */
    steps = 0;
  }

  stream_out << vertex[0].red << " " << vertex[0].green << " " << vertex[0].blue << " setrgbcolor" << endl;
  stream_out << vertex[0].x << " " << vertex[0].y << " moveto" << endl;

  for (int i = 0; i < steps; i++) {
    xnext += xstep;
    ynext += ystep;
    rnext += rstep;
    gnext += gstep;
    bnext += bstep;
    stream_out << xnext << " " << ynext << " lineto stroke" << endl;
    stream_out << rnext << " " << gnext << " " << bnext << " setrgbcolor" << endl;
    stream_out << xnext << " " << ynext << " moveto" << endl;
  }

  stream_out << vertex[1].x << " " << vertex[1].y << " lineto stroke" << endl;
}

void GlEPSFeedBackBuilder::lineResetToken(GLfloat *data) {
  lineToken(data);
}

void GlEPSFeedBackBuilder::polygonToken(GLfloat *data) {
  int nvertices =(int) *data;
  GLfloat red, green, blue;
  Feedback3Dcolor *vertex = (Feedback3Dcolor *)(data+1);

  if (nvertices > 0) {
    red = vertex[0].red;
    green = vertex[0].green;
    blue = vertex[0].blue;
    bool smooth = false;

    for(int i = 1; i < nvertices; i++) {
      if (red != vertex[i].red || green != vertex[i].green || blue != vertex[i].blue) {
        smooth = true;
        break;
      }
    }

    if (smooth) {
      /* Smooth shaded polygon; varying colors at vetices. */

      /* Break polygon into "nvertices-2" triangle fans. */
      for (int i = 0; i < nvertices - 2; i++) {
        stream_out << "[" << vertex[0].x << " " << vertex[i + 1].x << " " << vertex[i + 2].x << " " << vertex[0].y << " " <<  vertex[i + 1].y << " " << vertex[i + 2].y << "]";
        stream_out << " [" << vertex[0].red << " " << vertex[0].green << " " << vertex[0].blue << "] ["
                   << vertex[i + 1].red << " " << vertex[i + 1].green << " " << vertex[i + 1].blue << "] ["
                   << vertex[i + 2].red << " " << vertex[i + 2].green << " " << vertex[i + 2].blue << "] gouraudtriangle" << endl;
      }
    }
    else {
      /* Flat shaded polygon; all vertex colors the same. */
      stream_out << "newpath" << endl;
      stream_out << red << " " << green << " " <<  blue <<" setrgbcolor" << endl;

      /* Draw a filled triangle. */
      stream_out << vertex[0].x << " " << vertex[0].y << " moveto" << endl;

      for (int i = 1; i < nvertices; i++) {
        stream_out << vertex[i].x << " " << vertex[i].y << " lineto" << endl;
      }

      stream_out << "closepath fill" << endl << endl;
    }
  }
}
void GlEPSFeedBackBuilder::end() {
  stream_out << "grestore "<< endl << endl ;
  stream_out << "%%Add `showpage' to the end of this file to be able to print to a printer." << endl;
}

void GlEPSFeedBackBuilder::getResult(string* str) {
  *str=stream_out.str();
}

}
