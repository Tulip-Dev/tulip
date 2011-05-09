#!/bin/sh

THISDIR=$(dirname $0)
if [ -f $THISDIR/bin/tulip ]; then
  THISDIR=$THISDIR/bin
fi

case "`uname -s`" in
    *darwin* | *Darwin* | *DARWIN*)
	export DYLD_LIBRARY_PATH=${THISDIR}/../Frameworks:${THISDIR}/../lib/tlp:${THISDIR}/../lib/tlp/interactors:${THISDIR}/../lib/tlp/view:${DYLD_LIBRARY_PATH}
        # check for missing link to QtCLucene lib
	if [ ! -f $THISDIR/../Frameworks/libQtCLucene.4.dylib ]; then
	    QT_LUCENE=`ls $THISDIR/../Frameworks/libQtCLucene*.dylib`
	    ln -s $QT_LUCENE $THISDIR/../Frameworks/libQtCLucene.4.dylib
	fi
	;;
    *Linux*|*GNU/kFreeBSD*)
	export LD_LIBRARY_PATH=${THISDIR}/../lib:${THISDIR}/../lib/tlp:${THISDIR}/../lib/tlp/interactors:${THISDIR}/../lib/tlp/view:${LD_LIBRARY_PATH}
	# make some additional stuff if we are in the bundle (Qt libs in ../lib)
	QT_LIBS=$(ls ${THISDIR}/../lib/libQt*.* 2>&1 >/dev/null)
	if [ $? -eq 0 ]; then
	  # define PYTHONHOME
	  export PYTHONHOME=${THISDIR}/../lib
          # check tulip_app 32/64 bits version
	  uname -m | grep x86_64 2>&1 >/dev/null
	  if [ $? -eq 0 ]; then
	    EXE_TYPE="ELF 64-bit"
          else
            EXE_TYPE="ELF 32-bit"
	  fi
          file -b $THISDIR/tulip_app | grep "$EXE_TYPE" 2>&1 >/dev/null
          if [ $? -ne 0 ]; then
	    echo tulip "($EXE_TYPE)" cannot be executed on your system
            exit
          fi
	fi
	;;
esac

export LC_NUMERIC=C

TULIP_NEED_RESTART=TRUE
while [ "$TULIP_NEED_RESTART" = "TRUE" ];
do
  $THISDIR/tulip_app $@
  TULIP_NEED_RESTART=`$THISDIR/tulip_need_restart`
done
