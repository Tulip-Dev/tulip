dnl check for tulip
AC_DEFUN([AC_PATH_TULIP],
[
AC_ARG_WITH(tulip-config,
        [  --with-tulip-config=DIR   optional path list where to prepend to $PATH to find tulip-config],
        [ac_tulip_config="$withval:$PATH"],
        [ac_tulip_config="$PATH"])

AC_MSG_CHECKING(for tulip-config)

AC_PATH_PROG([TULIP_CONFIG], [tulip-config], [no], [$ac_tulip_config:$PATH])
if test "x$TULIP_CONFIG" = "xno" ; then
        AC_MSG_ERROR([tulip-config was not found. Verify tulip is installed])
fi

TULIP_INCLUDE=`$TULIP_CONFIG --cxxflags`
TULIP_LDFLAGS=`$TULIP_CONFIG --libs`
TULIP_OGL_INCLUDE=""
TULIP_OGL_LDFLAGS=""
TULIP_QT_INCLUDE=""
TULIP_QT_LDFLAGS=""
TULIP_PLUGINPATH=`$TULIP_CONFIG --pluginpath`
TULIP_QTDESIGNERPLUGIN="$TULIP_PLUGINPATH/designer"
AC_SUBST(TULIP_INCLUDE)
AC_SUBST(TULIP_LDFLAGS)
AC_SUBST(TULIP_OGL_INCLUDE)
AC_SUBST(TULIP_OGL_LDFLAGS)
AC_SUBST(TULIP_QT_INCLUDE)
AC_SUBST(TULIP_QT_LDFLAGS)
AC_SUBST(TULIP_PLUGINPATH)
AC_SUBST(TULIP_QTDESIGNERPLUGIN)
])
