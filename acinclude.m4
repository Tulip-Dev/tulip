dnl ------------------------------------------------------------------------
dnl Find a file (or one of more files in a list of dirs)
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN(AC_FIND_FILE,
[
$3=NO
for i in $2;
do
  for j in $1;
  do
    if test -r "$i/$j"; then
      $3=$i
      break 2
    fi
  done
done
])

AC_DEFUN(FIND_PATH,
[
   AC_MSG_CHECKING([for $1])
   AC_CACHE_VAL(kde_cv_path_$1,
   [
     kde_cv_path_$1="NONE"
     if test -n "$$2"; then
        kde_cv_path_$1="$$2";
     else
	dirs="$3"
	kde_save_IFS=$IFS
	IFS=':'
	for dir in $PATH; do
	  dirs="$dirs $dir"
        done
	IFS=$kde_save_IFS

        for dir in $dirs; do
	  if test -x "$dir/$1"; then
	    if test -n "$5"
	    then
              evalstr="$dir/$1 $5 2>&1 "
	      if eval $evalstr; then
                kde_cv_path_$1="$dir/$1"
                break
	      fi
            else
		kde_cv_path_$1="$dir/$1"
                break
	    fi
          fi
	done

     fi
         
   ])

   if test -z "$kde_cv_path_$1" || test "$kde_cv_path_$1" = "NONE"; then
      AC_MSG_RESULT(not found)
      $4
   else 
     AC_MSG_RESULT($kde_cv_path_$1)
     $2=$kde_cv_path_$1
   fi
])



dnl ------------------------------------------------------------------------
dnl Try to find the Open GL headers and libraries.
dnl $(GL_LDFLAGS) will be -Lglliblocation (if needed)
dnl and $(GL_INCLUDES) will be -Iglheaderlocation (if needed)
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN(AC_PATH_GL,
[
AC_REQUIRE([AC_PATH_X])
if test ${VAR_WIN32} = 1
then
LIB_GL="-lglu32 -lopengl32"
else
LIB_GL="-lGLU -lGL -lXi -lXmu"
fi
AC_MSG_CHECKING([for Open Gl])

ac_gl_includes="" ac_gl_libraries=""

AC_ARG_WITH(gl-includes,
    [  --with-gl-includes=DIR   where the OpenGL includes are. ],
    [  ac_gl_includes="$withval"
    ])

AC_ARG_WITH(gl-libraries,
    [  --with-gl-libraries=DIR where the OpenGL libraries are. ],
    [  ac_gl_libraries="$withval"
    ])

AC_CACHE_VAL(ac_cv_have_gl,
[
if test ${VAR_WIN32} = 1
then
gl_incdirs=" /mingw/include $ac_gl_includes $GLDIR/include /usr/include /usr/X11R6/include/X11 /usr/X11R6/include $x_includes "
else
gl_incdirs=" $ac_gl_includes $GLDIR/include /usr/include /usr/X11R6/include/X11 /usr/X11R6/include $x_includes "
fi
AC_FIND_FILE(GL/gl.h, $gl_incdirs, gl_incdir)
ac_gl_includes="$gl_incdir"

if test ${VAR_WIN32} = 1
then
gl_libdirs="$GLDIR $ac_gl_libraries $GLLIB /usr/X11R6/lib /usr/lib /usr/local/lib $x_libraries "
else
gl_libdirs="$ac_gl_libraries $GLLIB /usr/X11R6/lib /usr/lib /usr/local/lib $x_libraries "
fi

test -n "$GLDIR" && gl_libdirs="$GLDIR/lib $GLDIR $gl_libdirs"
test=NONE
gl_libdir=NONE
for dir in $gl_libdirs; do
if test ${VAR_WIN32} = 1
then
  try="ls -1 $dir/*opengl*"
else
  try="ls -1 $dir/libGL*"
fi
  if test=`eval $try 2> /dev/null`; then gl_libdir=$dir; break; else echo "tried $dir" >&AC_FD_CC ; fi
done
ac_gl_libraries="$gl_libdir"

 GL_INCLUDES="-I$ac_gl_includes"
 GL_LDFLAGS="-L$ac_gl_libraries"
])

eval "$ac_cv_have_gl"
if test "$ac_gl_libraries" = NONE; then
  AC_MSG_ERROR([ Not found , put your GLDIR environnement variable to the OpenGL directory ]);
else
  ac_cv_have_gl="have_gl=yes \
  ac_gl_includes=$ac_gl_includes ac_gl_libraries=$ac_gl_libraries"
  AC_MSG_RESULT([ libraries $ac_gl_libraries, headers $ac_gl_includes ])  
  gl_libraries="$ac_gl_libraries"
  gl_includes="$ac_gl_includes"
  GL_INCLUDES="-I$ac_gl_includes"	
  GL_LDFLAGS="-L$ac_gl_libraries"
fi

AC_SUBST(gl_libraries)
AC_SUBST(gl_includes)
AC_SUBST(GL_INCLUDES)
AC_SUBST(GL_LDFLAGS)

if test ${VAR_WIN32} = 1
then
LIB_GL="-lglu32 -lopengl32"
else
LIB_GL="-lGLU -lGL -lXi -lXmu"
fi
AC_SUBST(LIB_GL)
])


dnl ------------------------------------------------------------------------
dnl Try to find the QT headers and libraries.
dnl $(QT_LDFLAGS) will be -Lqt_lib_location (if needed)
dnl and $(QT_INCLUDES) will be -Iqt_header_location (if needed)
dnl ------------------------------------------------------------------------
AC_DEFUN(AC_PATH_QT,
[
if test ${VAR_WIN32} = 1
then
LIB_QT="-lqt-mt"
else 
if test ${VAR_CYGWIN} = 1
then 
LIB_QT="-lqt-mt"
else
LIB_QT="-lqui"
fi
fi
AC_MSG_CHECKING([for QT])

ac_qt_includes=NO ac_qt_libraries=NO 

AC_ARG_WITH(qt-includes,
    [  --with-qt-includes=DIR   where the QT includes are. ],
    [  ac_qt_includes="$withval"
    ])

AC_ARG_WITH(qt-libraries,
    [  --with-qt-libraries=DIR where the QT libraries are. ],
    [  ac_qt_libraries="$withval"
    ])

AC_CACHE_VAL(ac_cv_have_qt,
[
qt_incdirs="$ac_qt_includes $QTDIR/include $QTDIR/include/qt  /usr/include/qt /usr/lib/qt/include /usr/local/qt/include /usr/include /usr/local/lib/qt/include "
AC_FIND_FILE(qgl.h, $qt_incdirs, qt_incdir)
ac_qt_includes="$qt_incdir"

qt_libdirs="$ac_qt_libraries $QTDIR/lib /usr/lib/qt/lib /usr/local/lib/qt/lib /usr/lib/ /usr/local/lib/"
test -n "$QTDIR" && qt_libdirs="$QTDIR/lib $QTDIR $qt_libdirs"
if test ! "$ac_qt_libraries" = "NO"; then
  qt_libdirs="$ac_qt_libraries $qt_libdirs"
fi
test=NONE
qt_libdir=NONE
for dir in $qt_libdirs; do
  try="ls -1 $dir/libqt*"
  if test=`eval $try 2> /dev/null`; then qt_libdir=$dir; break; else echo "tried $dir" >&AC_FD_CC ; fi
done
ac_qt_libraries="$qt_libdir"

 qt_libraries="$ac_qt_libraries"
 qt_includes="$ac_qt_includes"
 QT_INCLUDES="-I$ac_qt_includes"
 QT_LDFLAGS="-L$ac_qt_libraries"	
])

eval "$ac_cv_have_qt"
if test "$ac_qt_libraries" = NONE; then
  AC_MSG_ERROR([ not found,  put your QTDIR environnement variable to the QT directory  ]);
else
  ac_cv_have_qt="have_qt=yes \
  ac_qt_includes=$ac_qt_includes ac_qt_libraries=$ac_qt_libraries"
  AC_MSG_RESULT([ libraries $ac_qt_libraries, headers $ac_qt_includes ])
  qt_libraries="$ac_qt_libraries"
  qt_includes="$ac_qt_includes"
  QT_INCLUDES="-I$qt_includes"
  QT_LDFLAGS="-L$qt_libraries"	
fi
AC_SUBST(qt_libraries)
AC_SUBST(qt_includes)
AC_SUBST(QT_INCLUDES)
AC_SUBST(QT_LDFLAGS)
AC_SUBST(LIB_QT)
])

AC_DEFUN(MOC_ERROR_MESSAGE,
[
    AC_MSG_ERROR([No Qt meta object compiler (moc) found!
Please check whether you installed Qt correctly. 
You need to have a running moc binary.
configure tried to run $ac_cv_path_moc and the test didn't
succeed. If configure shouldn't have tried this one, set
the environment variable MOC to the right one before running
configure.
])
])

AC_DEFUN(UIC_ERROR_MESSAGE,
[
    AC_MSG_ERROR([No Qt user interface compiler (uic) found!
Please check whether you installed Qt correctly. 
You need to have a running uic binary.
configure tried to run $ac_cv_path_uic and the test didn't
succeed. If configure shouldn't have tried this one, set
the environment variable UIC to the right one before running
configure.
])
])


dnl ------------------------------------------------------------------------
dnl Find the meta object compiler in the PATH, in $QTDIR/bin, and some
dnl more usual places
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN(AC_PATH_QT_MOC,
[
   FIND_PATH(moc, MOC, [$ac_qt_bindir $QTDIR/bin $QTDIR/src/moc \
	    /usr/bin /usr/X11R6/bin /usr/lib/qt/bin \
	    /usr/local/qt/bin], [MOC_ERROR_MESSAGE])
 
   if test -z "$MOC"; then
     if test -n "$ac_cv_path_moc"; then
       output=`eval "$ac_cv_path_moc --help 2>&1 | sed -e '1q' | grep Qt"`
     fi
     echo "configure:__oline__: tried to call $ac_cv_path_moc --help 2>&1 | sed -e '1q' | grep Qt" >&AC_FD_CC
     echo "configure:__oline__: moc output: $output" >&AC_FD_CC

     if test -z "$output"; then
       MOC_ERROR_MESSAGE
    fi
   fi
   
   AC_SUBST(MOC)
])

AC_DEFUN(AC_PATH_QT_UIC,
[
   FIND_PATH(uic, UIC, [$ac_qt_bindir $QTDIR/bin $QTDIR/src/uic \
	    /usr/bin /usr/X11R6/bin /usr/lib/qt/bin \
	    /usr/local/qt/bin], [UIC_ERROR_MESSAGE])
 
   if test -z "$UIC"; then
     if test -n "$ac_cv_path_uic"; then
       output=`eval "$ac_cv_path_uic --help 2>&1 | sed -e '1q' | grep Qt"`
     fi
     echo "configure:__oline__: tried to call $ac_cv_path_uic --help 2>&1 | sed -e '1q' | grep Qt" >&AC_FD_CC
     echo "configure:__oline__: uic output: $output" >&AC_FD_CC

     if test -z "$output"; then
       UIC_ERROR_MESSAGE
    fi
   fi
   
   AC_SUBST(UIC)
])

AC_DEFUN(AC_PATH_MINGW,
[
AC_MSG_CHECKING(for MinGW)

libraries="iconv.dll zlib1.dll freetype*.dll libxml*.dll libjpeg*.dll libpng*.dll mgwz.dll mingwm*.dll"
for lib in $libraries; do
try="ls -1 ${MINGWDIR}/bin/$lib"
if !(test=`eval $try 2> /dev/null`)
then 
AC_MSG_ERROR([Libraries was not found. Put your MINGWDIR environnement variable to the MinGW directory and install : libpng, libjpeg, iconv, zlib, freetype and xml2 libraries ])
fi
done
MINDIR=${MINGWDIR}
AC_SUBST(MINDIR)
])
