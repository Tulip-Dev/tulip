#!/bin/sh

echo "<assistantconfig version=\"3.2.0\"><profile>" > profile.adp
echo "<property name=\"name\">Help</property>" >> profile.adp
echo "<property name=\"title\">Help Tulip software</property>" >> profile.adp
echo "<property name=\"applicationicon\">common/logo-icon.png</property>" >> profile.adp
echo "<property name=\"startpage\">userHandbook/index.html</property>" >> profile.adp 
echo "<property name=\"aboutmenutext\">About Manuals</property>" >> profile.adp
echo "<property name=\"abouturl\">www.tulip-software.org</property>" >> profile.adp
echo "<property name=\"assistantdocs\">common</property> </profile>" >> profile.adp

cat developerHandbook/profile.dcf >> profile.adp
cat userHandbook/profile.dcf >> profile.adp

echo "</assistantconfig>" >> profile.adp