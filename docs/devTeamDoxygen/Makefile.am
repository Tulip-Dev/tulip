EXTRA_DIST = general general-html header.html footer.html tulip.html

all : html

html: tulip-lib/index.html tulip-libogl/index.html tulip-libqt/index.html tulip-soft/index.html tulip-plugins/index.html tulipdoc.html

tulipdoc.html: ../common/gen-index.sh
	sh ../common/gen-index.sh

tulip-lib/index.html: tulip-lib.doxygen header.html footer.html 
	$(DOXYGEN) tulip-lib.doxygen

tulip-lib.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Graph Library\"" > tulip-lib.doxygen
	echo "PROJECT_NUMBER = 2.0.1" >> tulip-lib.doxygen
	cat general >> tulip-lib.doxygen
	echo "INPUT = $(top_srcdir)/library/tulip " >> tulip-lib.doxygen
	echo "HTML_OUTPUT = tulip-lib">> tulip-lib.doxygen
	cat general-html >> tulip-lib.doxygen
	echo "DOT_PATH = $(DOT)">>tulip-lib.doxygen

tulip-libogl/index.html: tulip-libogl.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libogl.doxygen

tulip-libogl.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Open GL Library\"" > tulip-libogl.doxygen
	echo "PROJECT_NUMBER = 2.0.1" >> tulip-libogl.doxygen
	cat general >> tulip-libogl.doxygen
	echo "INPUT = $(top_srcdir)/library/tulip-ogl " >> tulip-libogl.doxygen
	echo "HTML_OUTPUT = tulip-libogl">> tulip-libogl.doxygen
	cat general-html >> tulip-libogl.doxygen
	echo "DOT_PATH = $(DOT)">>tulip-libogl.doxygen

tulip-libqt/index.html: tulip-libqt.doxygen header.html footer.html 
	$(DOXYGEN) tulip-libqt.doxygen

tulip-libqt.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip QT Library\"" > tulip-libqt.doxygen
	echo "PROJECT_NUMBER = 2.0.1" >> tulip-libqt.doxygen
	cat general >> tulip-libqt.doxygen
	echo "INPUT = $(top_srcdir)/library/tulip-qt " >> tulip-libqt.doxygen
	echo "EXCLUDE_PATTERNS = *Data.h *Data.cpp moc_*">> tulip-libqt.doxygen
	echo "HTML_OUTPUT = tulip-libqt">> tulip-libqt.doxygen
	cat general-html >> tulip-libqt.doxygen
	echo "DOT_PATH = $(DOT)">>tulip-libqt.doxygen

tulip-soft/index.html: tulip-soft.doxygen header.html footer.html 
	$(DOXYGEN) tulip-soft.doxygen

tulip-soft.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Development Software\"" > tulip-soft.doxygen
	echo "PROJECT_NUMBER = 2.0.1" >> tulip-soft.doxygen
	cat general >> tulip-soft.doxygen
	echo "INPUT = $(top_srcdir)/software/tulip " >> tulip-soft.doxygen
	echo "EXCLUDE_PATTERNS = *Data.h *Data.cpp moc_*">> tulip-soft.doxygen
	echo "HTML_OUTPUT = tulip-soft">> tulip-soft.doxygen
	cat general-html >> tulip-soft.doxygen
	echo "DOT_PATH = $(DOT)">>tulip-soft.doxygen

tulip-plugins/index.html: tulip-plugins.doxygen header.html footer.html 
	$(DOXYGEN) tulip-plugins.doxygen

tulip-plugins.doxygen: general general-html
	echo "PROJECT_NAME = \"Tulip Plug-ins\"" > tulip-plugins.doxygen
	echo "PROJECT_NUMBER = 2.0.1" >> tulip-plugins.doxygen
	cat general >> tulip-plugins.doxygen
	echo "INPUT = $(top_srcdir)/plugins " >> tulip-plugins.doxygen
	echo "EXCLUDE_PATTERNS = *Data.h *Data.cpp moc_*">> tulip-plugins.doxygen
	echo "HTML_OUTPUT = tulip-plugins">> tulip-plugins.doxygen
	cat general-html >> tulip-plugins.doxygen
	echo "DOT_PATH = $(DOT)">>tulip-plugins.doxygen

clean:
	rm -rf tulipdoc.html *~
	rm -rf tulip-libogl tulip-libogl.doxygen
	rm -rf tulip-libqt tulip-libqt.doxygen
	rm -rf tulip-soft tulip-soft.doxygen
	rm -rf tulip-lib tulip-lib.doxygen
	rm -rf tulip-plugins tulip-plugins.doxygen
