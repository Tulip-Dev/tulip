#!/bin/sh

THISDIR=$(dirname $0)

case "`uname -s`" in
    *darwin* | *Darwin* | *DARWIN*)
	export DYLD_LIBRARY_PATH=${THISDIR}/../Frameworks:${THISDIR}/../lib/tlp:${THISDIR}/../lib/tlp/view:${DYLD_LIBRARY_PATH}
	;;
    *Linux*|*GNU/kFreeBSD*)
	export LD_LIBRARY_PATH=${THISDIR}/../lib:${THISDIR}/../lib/tlp:${THISDIR}/../lib/tlp/view:${LD_LIBRARY_PATH}
	;;	
esac

TULIP_NEED_RESTART=TRUE
while [ "$TULIP_NEED_RESTART" = "TRUE" ];
do
  $THISDIR/tulip_app $@
  TULIP_NEED_RESTART=`$THISDIR/tulip_need_restart`
done