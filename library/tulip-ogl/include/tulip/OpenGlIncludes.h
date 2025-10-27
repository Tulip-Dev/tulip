/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef Tulip_OPENGL_INCLUDES_H
#define Tulip_OPENGL_INCLUDES_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

/*Taken from
 * https://gitlab.freedesktop.org/gstreamer/gst-plugins-good/commit/3d708a5bfa8961cc37671bc3226976dfc9ba50ad*/
/* The glext.h guard was renamed in 2018, but some software which
 * includes their own copy of the GL headers (such as qt (at least version 5.11.3)) might have
 * older version which use the old guard. This would result in the
 * header being included again (and symbols redefined).
 *
 * To avoid this, we define the "old" guard if the "new" guard is
 * defined.*/
#ifdef __gl_glext_h_
#ifndef __glext_h_
#define __glext_h_ 1
#endif
#endif

#endif // Tulip_OPENGL_INCLUDES_H
