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
#include <tulip/GlFeedBackRecorder.h>
#include <tulip/GlFeedBackBuilder.h>

#include <cstdlib>

using namespace std;

namespace tlp {

static int compare(const void *a, const void *b) {
  DepthIndex *p1 = (DepthIndex *) a;
  DepthIndex *p2 = (DepthIndex *) b;
  GLfloat diff = p2->depth - p1->depth;

  if (diff > 0.0) {
    return 1;
  }
  else if (diff < 0.0) {
    return -1;
  }
  else {
    return 0;
  }
}

GlFeedBackRecorder::GlFeedBackRecorder(GlFeedBackBuilder *builder,unsigned int pointSize):feedBackBuilder(builder), pointSize(pointSize) {}

void GlFeedBackRecorder::record(bool doSort,GLint size, GLfloat *feedBackBuffer,const Vector<int,4>& viewport) {
  feedBackBuilder->begin(viewport);

  if(doSort)
    sortAndRecord(size,feedBackBuffer);
  else
    record(size,feedBackBuffer);

  feedBackBuilder->end();
}

void GlFeedBackRecorder::sortAndRecord(GLint size, GLfloat *feedBackBuffer) {
  int token;
  GLfloat *loc, *end;
  Feedback3Dcolor *vertex;
  GLfloat depthSum;
  int nprimitives, item;
  DepthIndex *prims;
  int nvertices, i;

  end = feedBackBuffer + size;

  /* Count how many primitives there are. */
  nprimitives = 0;
  loc = feedBackBuffer;

  while (loc < end) {
    token = (int)*loc;
    loc++;

    switch (token) {
    case GL_LINE_TOKEN:
    case GL_LINE_RESET_TOKEN:
      loc += pointSize*2;
      nprimitives++;
      break;

    case GL_POLYGON_TOKEN:
      nvertices = (int)*loc;
      loc++;
      loc += (pointSize * nvertices);
      nprimitives++;
      break;

    case GL_POINT_TOKEN:
      loc += pointSize;
      nprimitives++;
      break;

    case GL_PASS_THROUGH_TOKEN:
      loc++;
      break;

    default:
      /* XXX Left as an excersie to the reader. */
      //printf("Incomplete implementation.  Unexpected token (%d).\n",token);
      assert(false);
    }
  }

  /* Allocate an array of pointers that will point back at
     primitives in the feedback buffer.  There will be one
     entry per primitive.  This array is also where we keep the
     primitive's average depth.  There is one entry per
     primitive  in the feedback buffer. */
  prims = (DepthIndex *) malloc(sizeof(DepthIndex) * nprimitives);

  item = 0;
  loc = feedBackBuffer;

  while (loc < end) {
    prims[item].ptr = loc;  /* Save this primitive's location. */
    token = (int)*loc;
    loc++;

    switch (token) {
    case GL_LINE_TOKEN:
    case GL_LINE_RESET_TOKEN:
      vertex = (Feedback3Dcolor *) loc;
      depthSum = vertex[0].z + vertex[1].z;
      prims[item].depth = depthSum / 2.0;
      loc += pointSize*2;
      item++;
      break;

    case GL_POLYGON_TOKEN:
      nvertices = (int)*loc;
      loc++;
      vertex = (Feedback3Dcolor *) loc;
      depthSum = vertex[0].z;

      for (i = 1; i < nvertices; i++) {
        depthSum += vertex[i].z;
      }

      prims[item].depth = depthSum / nvertices;
      loc += (pointSize * nvertices);
      item++;
      break;

    case GL_POINT_TOKEN:
      vertex = (Feedback3Dcolor *) loc;
      prims[item].depth = vertex[0].z;
      loc += pointSize;
      item++;
      break;

    case GL_PASS_THROUGH_TOKEN:
      loc++;
      break;

    default:
      /* XXX Left as an excersie to the reader. */
      free(prims);
      return;
    }
  }

  assert(item == nprimitives);

  /* Sort the primitives back to front. */
  qsort(prims, nprimitives, sizeof(DepthIndex), compare);

  /* Understand that sorting by a primitives average depth
     doesn't allow us to disambiguate some cases like self
     intersecting polygons.  Handling these cases would require
     breaking up the primitives.  That's too involved for this
     example.  Sorting by depth is good enough for lots of
     applications. */

  /* Emit the Encapsulated PostScript for the primitives in
     back to front order. */
  for (item = 0; item < nprimitives; item++) {
    (void) recordPrimitive(prims[item].ptr);
  }

  free(prims);
}

void GlFeedBackRecorder::record(GLint size, GLfloat *feedBackBuffer) {
  GLfloat *loc, *end;
  loc = feedBackBuffer;
  end = feedBackBuffer + size;

  while (loc < end) {
    loc = recordPrimitive(loc);
  }
}

GLfloat* GlFeedBackRecorder::recordPrimitive(GLfloat *loc) {
  int token = (int)*loc;
  loc++;

  switch (token) {
  case GL_LINE_RESET_TOKEN:
    feedBackBuilder->lineResetToken(loc);
    return loc+pointSize*2;

  case GL_LINE_TOKEN:
    feedBackBuilder->lineToken(loc);
    return loc+pointSize*2;

  case GL_POLYGON_TOKEN:
    int nvertices;
    nvertices = (int)*loc;
    feedBackBuilder->polygonToken(loc);
    return loc+(pointSize * nvertices)+1;

  case GL_POINT_TOKEN:
    feedBackBuilder->pointToken(loc);
    return loc+pointSize;

  case GL_PASS_THROUGH_TOKEN:
    feedBackBuilder->passThroughToken(loc);
    return loc+1;

  case GL_BITMAP_TOKEN:
    feedBackBuilder->bitmapToken(loc);
    return loc+pointSize;

  case GL_DRAW_PIXEL_TOKEN:
    feedBackBuilder->drawPixelToken(loc);
    return loc+pointSize;

  case GL_COPY_PIXEL_TOKEN:
    feedBackBuilder->copyPixelToken(loc);
    return loc+pointSize;

  default:
    /* XXX Left as an excersie to the reader. */
    //printf("Incomplete implementation.  Unexpected token (%d).\n", token);
    //exit(1);
    assert(false);
  }

  return loc;
}

}
