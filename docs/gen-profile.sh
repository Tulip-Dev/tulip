#!/bin/bash

if [ -d "$QTDIR/doc/html" ]
then
    QTDOCDIR=$QTDIR/doc/html;
elif [ -d "/usr/share/doc/qt-devel-3.3.4/html" ]
then
    QTDOCDIR=/usr/share/doc/qt-devel-3.3.4/html
elif [ -n "$QTDOC" ]
then
	QTDOCDIR=$QTDOC 
else
    echo 
    echo "**************************************************"
    echo "  WARNING : "
    echo "  We can not locate the documentations of Qt, "
    echo "  please set the environment variable QTDOC to "
    echo "  specify it. Refer the directory where you can"
    echo "  find \"assistant.html\" "
    echo "**************************************************"
    echo
    QTDOCDIR=""
fi
:
echo "<assistantconfig version=\"3.2.0\"><profile>" > profile.adp
echo "<property name=\"name\">Help</property>" >> profile.adp
echo "<property name=\"title\">Help Tulip software</property>" >> profile.adp
echo "<property name=\"applicationicon\">common/logo-icon.png</property>" >> profile.adp
echo "<property name=\"startpage\">userHandbook/html/index.html</property>" >> profile.adp 
echo "<property name=\"aboutmenutext\">About Manuals</property>" >> profile.adp
#echo "<property name=\"abouturl\"></property>" >> profile.adp
echo "<property name=\"assistantdocs\">$QTDOCDIR</property> </profile>" >> profile.adp
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

