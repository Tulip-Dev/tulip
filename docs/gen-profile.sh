#!/bin/bash

echo "<assistantconfig version=\"3.2.0\"><profile>" > profile.adp
echo "<property name=\"name\">Help</property>" >> profile.adp
echo "<property name=\"title\">Help Tulip software</property>" >> profile.adp
echo "<property name=\"applicationicon\">common/logo-icon.png</property>" >> profile.adp
echo "<property name=\"startpage\">userHandbook/html/index.html</property>" >> profile.adp 
echo "<property name=\"aboutmenutext\">About Manuals</property> </profile>" >> profile.adp
if test -f userHandbook/profile.dcf ; then
	cat userHandbook/profile.dcf >> profile.adp ;
fi;
if test -f developerHandbook/profile.dcf ; then
	cat developerHandbook/profile.dcf >> profile.adp ;
fi;
if test -f doxygen/profile.dcf ; then
	cat doxygen/profile.dcf >> profile.adp ;
fi;

echo "</assistantconfig>" >> profile.adp

