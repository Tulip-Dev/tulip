/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <GL/glew.h>

#include "FTVectoriser.h"

#include <tesselator.h>

#include <vector>
#include <iostream>


FTMesh::FTMesh()
  : currentTesselation(0),
    err(0)
{
  tesselationList.reserve(16);
}


FTMesh::~FTMesh()
{
  for(size_t t = 0; t < tesselationList.size(); ++t)
    {
      delete tesselationList[t];
    }

  tesselationList.clear();
}


void FTMesh::AddPoint(const FTGL_DOUBLE x, const FTGL_DOUBLE y, const FTGL_DOUBLE z)
{
  currentTesselation->AddPoint(x, y, z);
}


const FTGL_DOUBLE* FTMesh::Combine(const FTGL_DOUBLE x, const FTGL_DOUBLE y, const FTGL_DOUBLE z)
{
  tempPointList.push_back(FTPoint(x, y,z));
  return static_cast<const FTGL_DOUBLE*>(tempPointList.back());
}


void FTMesh::Begin(GLenum meshType)
{
  currentTesselation = new FTTesselation(meshType);
}


void FTMesh::End()
{
  tesselationList.push_back(currentTesselation);
}


const FTTesselation* FTMesh::Tesselation(size_t index) const
{
  return (index < tesselationList.size()) ? tesselationList[index] : NULL;
}


FTVectoriser::FTVectoriser(const FT_GlyphSlot glyph)
  :   contourList(0),
    mesh(0),
    ftContourCount(0),
    contourFlag(0)
{
  if(glyph)
    {
      outline = glyph->outline;

      ftContourCount = outline.n_contours;
      contourList = 0;
      contourFlag = outline.flags;

      ProcessContours();
    }
}


FTVectoriser::~FTVectoriser()
{
  for(size_t c = 0; c < ContourCount(); ++c)
    {
      delete contourList[c];
    }

  delete [] contourList;
  delete mesh;
}


void FTVectoriser::ProcessContours()
{
  short contourLength = 0;
  short startIndex = 0;
  short endIndex = 0;

  contourList = new FTContour*[ftContourCount];

  for(int i = 0; i < ftContourCount; ++i)
    {
      FT_Vector* pointList = &outline.points[startIndex];
      char* tagList = &outline.tags[startIndex];

      endIndex = outline.contours[i];
      contourLength =  (endIndex - startIndex) + 1;

      FTContour* contour = new FTContour(pointList, tagList, contourLength);

      contourList[i] = contour;

      startIndex = endIndex + 1;
    }

  // Compute each contour's parity. FIXME: see if FT_Outline_Get_Orientation
  // can do it for us.
  for(int i = 0; i < ftContourCount; i++)
    {
      FTContour *c1 = contourList[i];

      // 1. Find the leftmost point.
      FTPoint leftmost(65536.0, 0.0);

      for(size_t n = 0; n < c1->PointCount(); n++)
        {
          FTPoint p = c1->Point(n);
          if(p.X() < leftmost.X())
            {
              leftmost = p;
            }
        }

      // 2. Count how many other contours we cross when going further to
      // the left.
      int parity = 0;

      for(int j = 0; j < ftContourCount; j++)
        {
          if(j == i)
            {
              continue;
            }

          FTContour *c2 = contourList[j];

          for(size_t n = 0; n < c2->PointCount(); n++)
            {
              FTPoint p1 = c2->Point(n);
              FTPoint p2 = c2->Point((n + 1) % c2->PointCount());

              /* FIXME: combinations of >= > <= and < do not seem stable */
              if((p1.Y() < leftmost.Y() && p2.Y() < leftmost.Y())
                 || (p1.Y() >= leftmost.Y() && p2.Y() >= leftmost.Y())
                 || (p1.X() > leftmost.X() && p2.X() > leftmost.X()))
                {
                  continue;
                }
              else if(p1.X() < leftmost.X() && p2.X() < leftmost.X())
                {
                  parity++;
                }
              else
                {
                  FTPoint a = p1 - leftmost;
                  FTPoint b = p2 - leftmost;
                  if(b.X() * a.Y() > b.Y() * a.X())
                    {
                      parity++;
                    }
                }
            }
        }

      // 3. Make sure the glyph has the proper parity.
      c1->SetParity(parity);
    }
}


size_t FTVectoriser::PointCount()
{
  size_t s = 0;
  for(size_t c = 0; c < ContourCount(); ++c)
    {
      s += contourList[c]->PointCount();
    }

  return s;
}


const FTContour* FTVectoriser::Contour(size_t index) const
{
  return (index < ContourCount()) ? contourList[index] : NULL;
}


void FTVectoriser::MakeMesh(FTGL_DOUBLE zNormal, int outsetType, float outsetSize)
{
  if(mesh)
    {
      delete mesh;
    }

  const int nvp = 6;

  mesh = new FTMesh;

  TESStesselator* tess = tessNewTess(NULL);
  int windingRule = 0;

  if(contourFlag & ft_outline_even_odd_fill) // ft_outline_reverse_fill
    {
      windingRule = TESS_WINDING_POSITIVE;
    }
  else
    {
      windingRule = TESS_WINDING_NONZERO;
    }


  for(size_t c = 0; c < ContourCount(); ++c)
    {
      /* Build the */
      switch(outsetType)
        {
        case 1 : contourList[c]->buildFrontOutset(outsetSize); break;
        case 2 : contourList[c]->buildBackOutset(outsetSize); break;
        }

      const FTContour* contour = contourList[c];

      std::vector<float> contourPoints;
      for(size_t p = 0; p < contour->PointCount(); ++p)
        {
          const FTGL_DOUBLE* d;
          switch(outsetType)
            {
            case 1: d = contour->FrontPoint(p); break;
            case 2: d = contour->BackPoint(p); break;
            case 0: default: d = contour->Point(p); break;
            }

          for (int i = 0 ; i < 3 ; ++i) {
              contourPoints.push_back(d[i]);
            }
        }
      tessAddContour(tess, 3, &contourPoints[0], sizeof(float)*3, contourPoints.size() / 3);
    }

  TESSreal normal[3] = {0.0f, 0.0f, static_cast<TESSreal>(zNormal)};

  if (tessTesselate(tess, windingRule, TESS_POLYGONS, nvp, 3, normal)) {
      mesh->Begin(GL_TRIANGLES);
      const float* verts = tessGetVertices(tess);
      const int* elems = tessGetElements(tess);
      const int nelems = tessGetElementCount(tess);
      // iterate over polygons computed by tesselation
      for (int i = 0; i < nelems; ++i) {
          std::vector<FTGL_DOUBLE> vertices;
          const int* p = &elems[i*nvp];
          for (int j = 0; j < nvp && p[j] != TESS_UNDEF; ++j) {
              int idxx = p[j]*3;
              int idxy = p[j]*3+1;
              int idxz = p[j]*3+2;

              vertices.push_back(verts[idxx]);
              vertices.push_back(verts[idxy]);
              vertices.push_back(verts[idxz]);
            }
          // transform the polygon to a list of triangles
          for (size_t j = 3 ; j < vertices.size() - 3 ; j+=3) {
              mesh->AddPoint(vertices[0], vertices[1], vertices[2]);
              mesh->AddPoint(vertices[j], vertices[j+1], vertices[j+2]);
              mesh->AddPoint(vertices[j+3], vertices[j+4], vertices[j+5]);
            }
        }
      mesh->End();
    }
}

