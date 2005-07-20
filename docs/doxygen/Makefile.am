all : doc


REP_COMMON=`pwd`/../common
HEAD_FILE=tulipdoc.html
HEADER=header.html
FOOTER=footer.html


doc :
	sh gen-index.sh

clean :
	rm -rf tulip-lib/html tulip-libogl/html tulip-libqt/html tulip-soft/html tulip-plugins/html
	rm tulip-lib/tulip-lib.tag tulip-libogl/tulip-libogl.tag tulip-libqt/tulip-libqt.tag 
	rm tulip-plugins/tulip-plugins.tag
