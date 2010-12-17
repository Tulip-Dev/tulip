#!/bin/bash
if [ $# -eq 0 ]; then
QHP=tulip.qhp
else
QHP=$1
fi

echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > ${QHP}
echo "<QtHelpProject version=\"1.0\">" >> ${QHP}
echo "  <namespace>tulip_software.org</namespace>" >> ${QHP}
echo "  <virtualFolder>tulip</virtualFolder>" >> ${QHP}
echo "  <filterSection>" >> ${QHP}
for section in toc keywords
do
  echo "     <${section}>" >> ${QHP}
  for doc_dir in userHandbook developerHandbook doxygen
  do
    if test -f ${doc_dir}/${section}.dcf ; then
       cat ${doc_dir}/${section}.dcf >> ${QHP} ;
    fi;
  done;
  echo "     </${section}>" >> ${QHP}
done;
echo "    <files>" >> ${QHP}
echo "      <file>common/favicon.ico</file>" >> ${QHP} ;
echo "      <file>common/tulip-default.css</file>" >> ${QHP} ;
echo "      <file>common/tulip-doxygen.css</file>" >> ${QHP} ;
echo "      <file>common/closed.gif</file>" >> ${QHP} ;
echo "      <file>common/opened.gif</file>" >> ${QHP} ;
for doc_dir in common developerHandbook/images doxygen/tulip-lib doxygen/tulip-ogl-lib doxygen/tulip-qt-lib userHandbook/images
do
  for file in $(ls ${doc_dir}/*.png)
  do
    echo "      <file>${file}</file>" >> ${QHP} ;
  done ;
done;
for doc_dir in developerHandbook/html doxygen/tulip-lib doxygen/tulip-ogl-lib doxygen/tulip-qt-lib userHandbook/html
do
  for file in $(ls ${doc_dir}/*.html)
  do
    echo "      <file>${file}</file>" >> ${QHP} ;
  done ;
done;
echo "      <file>doxygen/allPlugins.html</file>" >> ${QHP} ;
echo "    </files>" >> ${QHP}
echo "  </filterSection>" >> ${QHP}
echo "</QtHelpProject>" >> ${QHP}
