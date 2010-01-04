#!/bin/sh
HTML_PAGE=allPlugins.html
cat header-allPlugins.html > $HTML_PAGE
cat allPluginsBody.html >> $HTML_PAGE
echo "</BODY>" >> $HTML_PAGE