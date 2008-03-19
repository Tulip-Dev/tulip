dnl ------------------------------------------------------------------------
dnl Find a file (or one of more files in a list of dirs)
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_FIND_FILE],
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

AC_DEFUN([FIND_PATH],
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

dnl @synopsis AC_PROG_JAVA
dnl
dnl Here is a summary of the main macros:
dnl
dnl AC_PROG_JAVAC: finds a Java compiler.
dnl
dnl AC_PROG_JAVA: finds a Java virtual machine.
dnl
dnl AC_CHECK_CLASS: finds if we have the given class (beware of
dnl CLASSPATH!).
dnl
dnl AC_CHECK_RQRD_CLASS: finds if we have the given class and stops
dnl otherwise.
dnl
dnl AC_TRY_COMPILE_JAVA: attempt to compile user given source.
dnl
dnl AC_TRY_RUN_JAVA: attempt to compile and run user given source.
dnl
dnl AC_JAVA_OPTIONS: adds Java configure options.
dnl
dnl AC_PROG_JAVA tests an existing Java virtual machine. It uses the
dnl environment variable JAVA then tests in sequence various common
dnl Java virtual machines. For political reasons, it starts with the
dnl free ones. You *must* call [AC_PROG_JAVAC] before.
dnl
dnl If you want to force a specific VM:
dnl
dnl - at the configure.in level, set JAVA=yourvm before calling
dnl AC_PROG_JAVA
dnl
dnl   (but after AC_INIT)
dnl
dnl - at the configure level, setenv JAVA
dnl
dnl You can use the JAVA variable in your Makefile.in, with @JAVA@.
dnl
dnl *Warning*: its success or failure can depend on a proper setting of
dnl the CLASSPATH env. variable.
dnl
dnl TODO: allow to exclude virtual machines (rationale: most Java
dnl programs cannot run with some VM like kaffe).
dnl
dnl Note: This is part of the set of autoconf M4 macros for Java
dnl programs. It is VERY IMPORTANT that you download the whole set,
dnl some macros depend on other. Unfortunately, the autoconf archive
dnl does not support the concept of set of macros, so I had to break it
dnl for submission.
dnl
dnl A Web page, with a link to the latest CVS snapshot is at
dnl <http://www.internatif.org/bortzmeyer/autoconf-Java/>.
dnl
dnl This is a sample configure.in Process this file with autoconf to
dnl produce a configure script.
dnl
dnl    AC_INIT(UnTag.java)
dnl
dnl    dnl Checks for programs.
dnl    AC_CHECK_CLASSPATH
dnl    AC_PROG_JAVAC
dnl    AC_PROG_JAVA
dnl
dnl    dnl Checks for classes
dnl    AC_CHECK_RQRD_CLASS(org.xml.sax.Parser)
dnl    AC_CHECK_RQRD_CLASS(com.jclark.xml.sax.Driver)
dnl
dnl    AC_OUTPUT(Makefile)
dnl
dnl @category Java
dnl @author Stephane Bortzmeyer <bortzmeyer@pasteur.fr>
dnl @version 2000-07-19
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_JAVA],[
AC_REQUIRE([AC_EXEEXT])dnl
if test x$JAVAPREFIX = x; then
        test x$JAVA = x && AC_CHECK_PROGS(JAVA, kaffe$EXEEXT java$EXEEXT)
else
        test x$JAVA = x && AC_CHECK_PROGS(JAVA, kaffe$EXEEXT java$EXEEXT, $JAVAPREFIX)
fi
test x$JAVA = x && AC_MSG_ERROR([no acceptable Java virtual machine found in \$PATH])
AC_PROG_JAVA_WORKS
AC_PROVIDE([$0])dnl
])

dnl @synopsis AC_PROG_JAVA_WORKS
dnl
dnl Internal use ONLY.
dnl
dnl Note: This is part of the set of autoconf M4 macros for Java
dnl programs. It is VERY IMPORTANT that you download the whole set,
dnl some macros depend on other. Unfortunately, the autoconf archive
dnl does not support the concept of set of macros, so I had to break it
dnl for submission. The general documentation, as well as the sample
dnl configure.in, is included in the AC_PROG_JAVA macro.
dnl
dnl @category Java
dnl @author Stephane Bortzmeyer <bortzmeyer@pasteur.fr>
dnl @version 2000-07-19
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_JAVA_WORKS], [
AC_CHECK_PROG(uudecode, uudecode$EXEEXT, yes)
if test x$uudecode = xyes; then
AC_CACHE_CHECK([if uudecode can decode base 64 file], ac_cv_prog_uudecode_base64, [
dnl /**
dnl  * Test.java: used to test if java compiler works.
dnl  */
dnl public class Test
dnl {
dnl
dnl public static void
dnl main( String[] argv )
dnl {
dnl     System.exit (0);
dnl }
dnl
dnl }
cat << \EOF > Test.uue
begin-base64 644 Test.class
yv66vgADAC0AFQcAAgEABFRlc3QHAAQBABBqYXZhL2xhbmcvT2JqZWN0AQAE
bWFpbgEAFihbTGphdmEvbGFuZy9TdHJpbmc7KVYBAARDb2RlAQAPTGluZU51
bWJlclRhYmxlDAAKAAsBAARleGl0AQAEKEkpVgoADQAJBwAOAQAQamF2YS9s
YW5nL1N5c3RlbQEABjxpbml0PgEAAygpVgwADwAQCgADABEBAApTb3VyY2VG
aWxlAQAJVGVzdC5qYXZhACEAAQADAAAAAAACAAkABQAGAAEABwAAACEAAQAB
AAAABQO4AAyxAAAAAQAIAAAACgACAAAACgAEAAsAAQAPABAAAQAHAAAAIQAB
AAEAAAAFKrcAErEAAAABAAgAAAAKAAIAAAAEAAQABAABABMAAAACABQ=
====
EOF
if uudecode$EXEEXT Test.uue; then
        ac_cv_prog_uudecode_base64=yes
else
        echo "configure: __oline__: uudecode had trouble decoding base 64 file 'Test.uue'" >&AC_FD_CC
        echo "configure: failed file was:" >&AC_FD_CC
        cat Test.uue >&AC_FD_CC
        ac_cv_prog_uudecode_base64=no
fi
rm -f Test.uue])
fi
if test x$ac_cv_prog_uudecode_base64 != xyes; then
        rm -f Test.class
        AC_MSG_WARN([I have to compile Test.class from scratch])
        if test x$ac_cv_prog_javac_works = xno; then
                AC_MSG_ERROR([Cannot compile java source. $JAVAC does not work properly])
        fi
        if test x$ac_cv_prog_javac_works = x; then
                AC_PROG_JAVAC
        fi
fi
AC_CACHE_CHECK(if $JAVA works, ac_cv_prog_java_works, [
JAVA_TEST=Test.java
CLASS_TEST=Test.class
TEST=Test
changequote(, )dnl
cat << \EOF > $JAVA_TEST
/* [#]line __oline__ "configure" */
public class Test {
public static void main (String args[]) {
        System.exit (0);
} }
EOF
changequote([, ])dnl
if test x$ac_cv_prog_uudecode_base64 != xyes; then
        if AC_TRY_COMMAND($JAVAC $JAVACFLAGS $JAVA_TEST) && test -s $CLASS_TEST; then
                :
        else
          echo "configure: failed program was:" >&AC_FD_CC
          cat $JAVA_TEST >&AC_FD_CC
          AC_MSG_ERROR(The Java compiler $JAVAC failed (see config.log, check the CLASSPATH?))
        fi
fi
if AC_TRY_COMMAND($JAVA $JAVAFLAGS $TEST) >/dev/null 2>&1; then
  ac_cv_prog_java_works=yes
else
  echo "configure: failed program was:" >&AC_FD_CC
  cat $JAVA_TEST >&AC_FD_CC
  AC_MSG_ERROR(The Java VM $JAVA failed (see config.log, check the CLASSPATH?))
fi
rm -fr $JAVA_TEST $CLASS_TEST Test.uue
])
AC_PROVIDE([$0])dnl
]
)

dnl @synopsis AC_PROG_JAVAC
dnl
dnl AC_PROG_JAVAC tests an existing Java compiler. It uses the
dnl environment variable JAVAC then tests in sequence various common
dnl Java compilers. For political reasons, it starts with the free
dnl ones.
dnl
dnl If you want to force a specific compiler:
dnl
dnl - at the configure.in level, set JAVAC=yourcompiler before calling
dnl AC_PROG_JAVAC
dnl
dnl - at the configure level, setenv JAVAC
dnl
dnl You can use the JAVAC variable in your Makefile.in, with @JAVAC@.
dnl
dnl *Warning*: its success or failure can depend on a proper setting of
dnl the CLASSPATH env. variable.
dnl
dnl TODO: allow to exclude compilers (rationale: most Java programs
dnl cannot compile with some compilers like guavac).
dnl
dnl Note: This is part of the set of autoconf M4 macros for Java
dnl programs. It is VERY IMPORTANT that you download the whole set,
dnl some macros depend on other. Unfortunately, the autoconf archive
dnl does not support the concept of set of macros, so I had to break it
dnl for submission. The general documentation, as well as the sample
dnl configure.in, is included in the AC_PROG_JAVA macro.
dnl
dnl @category Java
dnl @author Stephane Bortzmeyer <bortzmeyer@pasteur.fr>
dnl @version 2000-07-19
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_JAVAC],[
AC_REQUIRE([AC_EXEEXT])dnl
if test "x$JAVAPREFIX" = x; then
        test "x$JAVAC" = x && AC_CHECK_PROGS(JAVAC, "gcj$EXEEXT -C" guavac$EXEEXT jikes$EXEEXT javac$EXEEXT)
else
        test "x$JAVAC" = x && AC_CHECK_PROGS(JAVAC, "gcj$EXEEXT -C" guavac$EXEEXT jikes$EXEEXT javac$EXEEXT, $JAVAPREFIX)
fi
test "x$JAVAC" = x && AC_MSG_ERROR([no acceptable Java compiler found in \$PATH])
AC_PROG_JAVAC_WORKS
AC_PROVIDE([$0])dnl
])

dnl @synopsis AC_PROG_JAVAC_WORKS
dnl
dnl Internal use ONLY.
dnl
dnl Note: This is part of the set of autoconf M4 macros for Java
dnl programs. It is VERY IMPORTANT that you download the whole set,
dnl some macros depend on other. Unfortunately, the autoconf archive
dnl does not support the concept of set of macros, so I had to break it
dnl for submission. The general documentation, as well as the sample
dnl configure.in, is included in the AC_PROG_JAVA macro.
dnl
dnl @category Java
dnl @author Stephane Bortzmeyer <bortzmeyer@pasteur.fr>
dnl @version 2000-07-19
dnl @license GPLWithACException

AC_DEFUN([AC_PROG_JAVAC_WORKS],[
AC_CACHE_CHECK([if $JAVAC works], ac_cv_prog_javac_works, [
JAVA_TEST=Test.java
CLASS_TEST=Test.class
cat << \EOF > $JAVA_TEST
/* [#]line __oline__ "configure" */
public class Test {
}
EOF
if AC_TRY_COMMAND($JAVAC $JAVACFLAGS $JAVA_TEST) >/dev/null 2>&1; then
  ac_cv_prog_javac_works=yes
else
  AC_MSG_ERROR([The Java compiler $JAVAC failed (see config.log, check the CLASSPATH?)])
  echo "configure: failed program was:" >&AC_FD_CC
  cat $JAVA_TEST >&AC_FD_CC
fi
rm -f $JAVA_TEST $CLASS_TEST
])
AC_PROVIDE([$0])dnl
])


dnl ------------------------------------------------------------------------
dnl Try to find the Open GL headers and libraries.
dnl $(GL_LDFLAGS) will be -Lglliblocation (if needed)
dnl and $(GL_INCLUDES) will be -Iglheaderlocation (if needed)
dnl ------------------------------------------------------------------------
dnl
AC_DEFUN([AC_PATH_GL],
[
AC_REQUIRE([AC_PATH_X])
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
  if test=`eval $try 2> /dev/null`; then 
    gl_libdir=$dir
    if test ${VAR_WIN32} = 0 ; then
      try="ls -1 $gl_libdir/libGLEW.*"
      if test=`eval $try 2> /dev/null`; then break; else AC_MSG_ERROR([ libGLEW not found , please install it in $gl_libdir ]); fi
    else
      break
    fi
  else 
    echo "tried $dir" >&AC_FD_CC
  fi
done
ac_gl_libraries="$gl_libdir"
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
if test ${VAR_MACOSX} = 1
then
  GL_INCLUDES="-I$ac_gl_includes"
  GL_LDFLAGS=""
else
  GL_INCLUDES="-I$ac_gl_includes"	
  GL_LDFLAGS="-L$ac_gl_libraries"
fi
fi

dnl MAC PORT
if test ${VAR_MACOSX} = 1
then
  GL_INCLUDES="$GL_INCLUDES -I/usr/include/malloc"
fi
AC_SUBST(gl_libraries)
AC_SUBST(gl_includes)
AC_SUBST(GL_INCLUDES)
AC_SUBST(GL_LDFLAGS)

if test ${VAR_WIN32} = 1
then
  LIB_GL="-lglew32 -lglu32 -lopengl32"
else
  if test ${VAR_MACOSX} = 1
  then
    LIB_GL="/usr/local/lib/libGLEW.dylib -framework OpenGL"
  else
    LIB_GL="-lGLEW -lGLU -lGL"
  fi
fi
AC_SUBST(LIB_GL)
])


dnl ------------------------------------------------------------------------
dnl Try to find the QT headers and libraries.
dnl $(QT_LDFLAGS) will be -Lqt_lib_location (if needed)
dnl and $(QT_INCLUDES) will be -Iqt_header_location (if needed)
dnl ------------------------------------------------------------------------
AC_DEFUN([AC_PATH_QT],
[
AC_MSG_CHECKING([for QT 4])

ac_qt_dir=NO ac_qt_includes=NO ac_qt_libraries=NO 

AC_ARG_WITH(qt-dir,
    [  --with-qt-dir=DIR       where QT is installed. ],
    [  ac_qt_dir="$withval"
    ])

AC_ARG_WITH(qt-includes,
    [  --with-qt-includes=DIR  where the QT includes are. ],
    [  ac_qt_includes="$withval"
    ])

AC_ARG_WITH(qt-libraries,
    [  --with-qt-libraries=DIR where the QT libraries are. ],
    [  ac_qt_libraries="$withval"
    ])

AC_CACHE_VAL(ac_cv_have_qt,
[
if test ! "$ac_qt_dir" = "NO"; then
   QTDIR=$ac_qt_dir
fi

qt_incdirs="$ac_qt_includes $ac_qt_includes/Qt ${QTDIR}/include ${QTDIR}/include/qt ${QTDIR}/include/Qt  /usr/include/qt /usr/lib/qt/include /usr/local/qt/include /usr/include /usr/local/lib/qt/include "
AC_FIND_FILE(QtCore, $qt_incdirs, qt_incdir)
QT_PACKAGE=qt4
qtlib_prefix=libQt
AC_SUBST(QT_PACKAGE)

ac_qt_includes="$qt_incdir -I$qt_incdir/Qt"

dnl we add QT3_SUPPORT compilation flag as indicated 
dnl in http://doc.trolltech.com/4.0/porting4.html
dnl an QT_NO_DEBUG to enable the widgets plugins integration in Qt designer
  QT_CPPFLAGS="$QT_CPPFLAGS -DQT3_SUPPORT  -DQT_NO_DEBUG"

qt_libdirs="$ac_qt_libraries ${QTDIR}/lib /usr/lib/qt4/lib /usr/local/lib/qt/lib /usr/lib/ /usr/local/lib/"
test -n "${QTDIR}" && qt_libdirs="${QTDIR}/lib ${QTDIR} $qt_libdirs"
if test ! "$ac_qt_libraries" = "NO"; then
  qt_libdirs="$ac_qt_libraries $qt_libdirs"
fi
test=NONE
qt_libdir=NONE
for dir in $qt_libdirs; do
  try="ls -1 $dir/$qtlib_prefix*"
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
AC_SUBST(QT_CPPFLAGS)
dnl With the version 4 of Qt, its features are splitted in many libs
dnl Qt3Support lib is for Qt 3 compatibility purpose
if test ${VAR_WIN32} = 1
then
  LIB_QT_ASSISTANT="-L${QTDIR}/lib -lQtAssistantClient4"
  LIB_QT="-lQtCore4 -lQtGui4 -lQt3Support4 -lQtOpenGL4 -lQtNetwork4 -lQtXml4"
dnl For linking purpose, we need to copy some libs
dnl Copy $QTDIR/lib/Qt*4.dll in $QTDIR/lib/libQt*4.dll if needed
  if test ! -f ${QTDIR}/lib/libQtCore4.dll ; then
    cp ${QTDIR}/lib/QtCore4.dll ${QTDIR}/lib/libQtCore4.dll
  fi
  if test ! -f ${QTDIR}/lib/libQtGui4.dll ; then
    cp ${QTDIR}/lib/QtGui4.dll ${QTDIR}/lib/libQtGui4.dll
  fi
  if test ! -f ${QTDIR}/lib/libQt3Support4.dll ; then
    cp ${QTDIR}/lib/Qt3Support4.dll ${QTDIR}/lib/libQt3Support4.dll
  fi
  if test ! -f ${QTDIR}/lib/libQtOpenGL4.dll ; then
    cp ${QTDIR}/lib/QtOpenGL4.dll ${QTDIR}/lib/libQtOpenGL4.dll
  fi
  if test ! -f ${QTDIR}/lib/libQtNetwork4.dll ; then
    cp ${QTDIR}/lib/QtNetwork4.dll ${QTDIR}/lib/libQtNetwork4.dll
  fi
  if test ! -f ${QTDIR}/lib/libQtXml4.dll ; then
    cp ${QTDIR}/lib/QtXml4.dll ${QTDIR}/lib/libQtXml4.dll
  fi
else
  if test ${VAR_MACOSX} = 1
  then
    LIB_QT="-F$ac_qt_libraries -framework QtCore -framework QtGui -framework Qt3Support -framework QtOpenGL -framework QtNetwork -framework QtSql -framework QtXml"
    if test -d ${QTDIR}/lib/QtAssistantClient.framework
    then
      LIB_QT_ASSISTANT="-F$ac_qt_libraries -framework QtAssistantClient"
    else
      if test -d ${QTDIR}/lib/QtAssistant.framework
      then
        LIB_QT_ASSISTANT="-F$ac_qt_libraries -framework QtAssistant"
      fi
    fi
  else
    LIB_QT_ASSISTANT="-L${QTDIR}/lib -lQtAssistantClient"
    LIB_QT="-lQtCore -lQt3Support -lQtGui -lQtOpenGL"
  fi
fi

AC_SUBST(LIB_QT)
AC_SUBST(LIB_QT_ASSISTANT)
AC_SUBST(QTDIR)
])

AC_DEFUN([MOC_ERROR_MESSAGE],
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

AC_DEFUN([UIC_ERROR_MESSAGE],
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
AC_DEFUN([AC_PATH_QT_MOC],
[
   FIND_PATH(moc, MOC, [$ac_qt_bindir ${QTDIR}/bin ${QTDIR}/src/moc \
	    /usr/bin /usr/X11R6/bin /usr/lib/qt4/bin \
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
   else
     $MOC -v 2>&1 | grep 'Qt 4' > /dev/null
     if test $? -eq 1; then
       qt_bin_dirs="$ac_qt_bindir ${QTDIR}/bin ${QTDIR}/src/moc /usr/bin /usr/X11R6/bin /usr/lib/qt4/bin /usr/local/qt/bin ";
       AC_FIND_FILE(moc-qt4, $qt_bin_dirs, qt_bin_dir)
       MOC=$qt_bin_dir/moc-qt4
       if test ! -x $MOC; then
         MOC_ERROR_MESSAGE
       else
         AC_MSG_RESULT([ for QT 4 found $MOC ])
       fi
     fi
   fi
   AC_SUBST(MOC)
])

AC_DEFUN([AC_PATH_QT_UIC],
[
dnl we use uic3
   FIND_PATH(uic3, UIC, [$ac_qt_bindir ${QTDIR}/bin ${QTDIR}/src/uic \
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
   AC_MSG_CHECKING([whether $UIC runs])
   if test `$UIC -version 2>&1 | grep -c version` -ne 0; then
     AC_MSG_RESULT([yes])
   else
     AC_MSG_RESULT([no])
     UIC_ERROR_MESSAGE
   fi
   AC_SUBST(UIC)
dnl look for Qt minor version and add QT_MINOR_REL
   QT_MINOR_VERSION=`$UIC -version 2>&1 | tail -n 1 | awk -F'.' '{print $(NF - 1)}'`
   QT_CPPFLAGS="$QT_CPPFLAGS -DQT_MINOR_REL=$QT_MINOR_VERSION"
   AC_SUBST(QT_MINOR_VERSION)
])

AC_DEFUN([AC_PATH_MINGW],
[
AC_MSG_CHECKING(for MinGW needed libraries)
MINGWDIR=$(grep mingw /etc/fstab)
MINGWDIR=$(echo $MINGWDIR | sed 's/ \/mingw//')
GLDIR=/mingw/lib
libraries="iconv.dll freetype*.dll glew*.dll jpeg*.dll libpng*.dll libxml*.dll mingwm*.dll zlib1.dll"
for lib in $libraries; do
try="ls -1 /mingw/bin/$lib"
if !(test=`eval $try 2> /dev/null`)
then 
AC_MSG_RESULT($lib not found)
AC_MSG_ERROR([All the freetype, jpeg, iconv, libpng, xml2, and zlib1 libraries must be installed in ${MINGWDIR}/bin])
fi
done
AC_MSG_RESULT(yes)
dnl For linking purpose, we need to copy some libs
dnl Copy libraries from windows/system32
dnl in ${GLDIR} if needed
if !(test -f $GLDIR/libglu32.dll); then
  cp $SYSTEMROOT/system32/glu32.dll $GLDIR/libglu32.dll
  cp $SYSTEMROOT/system32/opengl32.dll $GLDIR/libopengl32.dll
fi
dnl Copy libraries from${MINGWDIR}/bin
dnl in ${MINGWDIR}/lib if needed
if !(test -f /mingw/lib/libfreetype.dll); then
   library=$(ls -1 /mingw/bin/freetype*.dll)
   cp ${library} /mingw/lib/libfreetype.dll
fi
if !(test -f /mingw/lib/libglew32.dll); then
   library=$(ls -1 /mingw/bin/glew32.dll)
   cp ${library} /mingw/lib/libglew32.dll
fi
if !(test -f /mingw/lib/libjpeg.dll); then
   library=$(ls -1 /mingw/bin/jpeg*.dll)
   cp ${library} /mingw/lib/libjpeg.dll
fi
if !(test -f /mingw/lib/libxml2.dll); then
   library=$(ls -1 /mingw/bin/libxml*.dll)
   cp ${library} /mingw/lib/libxml2.dll
fi
if !(test -f /mingw/lib/libz.dll); then
   cp /mingw/bin/zlib1.dll /mingw/lib/libz.dll
fi

MINDIR=${MINGWDIR}
AC_SUBST(MINDIR)
])
