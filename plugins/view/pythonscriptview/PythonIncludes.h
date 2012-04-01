/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#ifndef PYTHONINCLUDES_H_
#define PYTHONINCLUDES_H_

// thanks to the VTK project for this patch for Visual Studio in debug mode
#if defined(_MSC_VER) && defined(_DEBUG)
// Include these low level headers before undefing _DEBUG. Otherwise when doing
// a debug build against a release build of python the compiler will end up
// including these low level headers without DEBUG enabled, causing it to try
// and link release versions of this low level C api.
# include <basetsd.h>
# include <assert.h>
# include <ctype.h>
# include <errno.h>
# include <io.h>
# include <math.h>
# include <sal.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <time.h>
# include <wchar.h>
#  undef _DEBUG
#  if _MSC_VER >= 1400
#    define _CRT_NOFORCE_MANIFEST 1
#  endif
#  include <Python.h>
#  include <frameobject.h>
#  include <structmember.h>
#  include <import.h>
#  include <sip.h>
#  define _DEBUG
# else
#  include <Python.h>
#  include <frameobject.h>
#  include <structmember.h>
#  include <import.h>
#  include <sip.h>
# endif

const sipAPIDef *get_sip_api() {
#if defined(SIP_USE_PYCAPSULE)
  return (const sipAPIDef *)PyCapsule_Import("sip._C_API", 0);
#else
  PyObject *sip_module;
  PyObject *sip_module_dict;
  PyObject *c_api;

  /* Import the SIP module. */
  sip_module = PyImport_ImportModule("sip");

  if (sip_module == NULL)
    return NULL;

  /* Get the module's dictionary. */
  sip_module_dict = PyModule_GetDict(sip_module);

  /* Get the "_C_API" attribute. */
  c_api = PyDict_GetItemString(sip_module_dict, "_C_API");

  if (c_api == NULL)
    return NULL;

  /* Sanity check that it is the right type. */
  if (!PyCObject_Check(c_api))
    return NULL;

  /* Get the actual pointer from the object. */
  return (const sipAPIDef *)PyCObject_AsVoidPtr(c_api);
#endif
}

#endif /* PYTHONINCLUDES_H_ */
