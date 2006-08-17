EXTRA_DIST = general general-html header.html footer.html tulip.html

all : 

html: tulip-lib/index.html tulip-libogl/index.html tulip-libqt/index.html tulip-soft/index.html tulip-plugins/index.html tulipdoc.html

tulipdoc.html: ../common/gen-index.sh
	sh ../common/gen-index.sh

tulip-lib/index.html: tulip-lib.doxygen header.html footer.html 
	$(DOXYGEN) tulip-lib.doxygen

tulip-lib.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip " >> $@
	echo "HTML_OUTPUT = tulip-lib">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-libogl/index.html: tulip-libogl.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libogl.doxygen

tulip-libogl.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-ogl " >> $@
	echo "HTML_OUTPUT = tulip-libogl">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-libqt/index.html: tulip-libqt.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libqt.doxygen

tulip-libqt.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip QT Library\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/library/tulip-qt " >> $@
	echo "EXCLUDE_PATTERNS = *Data.h *Data.cpp moc_*">> $@
	echo "HTML_OUTPUT = tulip-libqt">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-soft/index.html: tulip-soft.doxygen header.html footer.html 
	$(DOXYGEN) tulip-soft.doxygen

tulip-soft.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Development Software\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/software/tulip " >> $@
	echo "EXCLUDE_PATTERNS = *Data.h *Data.cpp moc_*">> $@
	echo "HTML_OUTPUT = tulip-soft">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

tulip-plugins/index.html: tulip-plugins.doxygen header.html footer.html 
	$(DOXYGEN) tulip-plugins.doxygen

tulip-plugins.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Plug-ins\"" > $@
	echo "PROJECT_NUMBER = $(VERSION)" >> $@
	cat general >> $@
	echo "INPUT = $(top_srcdir)/plugins " >> $@
	echo "EXCLUDE_PATTERNS = *Data.h *Data.cpp moc_*">> $@
	echo "HTML_OUTPUT = tulip-plugins">> $@
	cat general-html >> $@
	echo "DOT_PATH = "`dirname $(DOT)`>> $@

clean:
	rm -rf tulipdoc.html *~
	rm -rf tulip-libogl tulip-libogl.doxygen
	rm -rf tulip-libqt tulip-libqt.doxygen
	rm -rf tulip-soft tulip-soft.doxygen
	rm -rf tulip-lib tulip-lib.doxygen
	rm -rf tulip-plugins tulip-plugins.doxygen
