#!/bin/sh
QTDOCDIR=""
for A in `find /usr -name "assistant.html"`
do
	QTDOCDIR=`dirname $A` 
done

if [ -z "$QTDOCDIR" ]
then
	QTDOCDIR=$QTDIR/doc/html 
fi

echo "<assistantconfig version=\"3.2.0\"><profile>" > profile.adp
echo "<property name=\"name\">Help</property>" >> profile.adp
echo "<property name=\"title\">Help Tulip software</property>" >> profile.adp
echo "<property name=\"applicationicon\">common/logo-icon.png</property>" >> profile.adp
echo "<property name=\"startpage\">userHandbook/index.html</property>" >> profile.adp 
echo "<property name=\"aboutmenutext\">About Manuals</property>" >> profile.adp
#echo "<property name=\"abouturl\"></property>" >> profile.adp
echo "<property name=\"assistantdocs\">$QTDOCDIR</property> </profile>" >> profile.adp

cat developerHandbook/profile.dcf >> profile.adp
cat userHandbook/profile.dcf >> profile.adp

echo "<dcf ref=\"developerHandbook/index.cache\" title=\"Tulip Library\">" >> profile.adp
echo "</dcf>" >> profile.adp
echo "</assistantconfig>" >> profile.adp

