all : doc

doc : check saxon

check :
	$(XMLLINT) --valid -noout index.docbook

clean :
	rm -f *.html

saxon :
	$(JAVA) -cp "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar"  com.icl.saxon.StyleSheet \
	index.docbook ../common/xsl-stylesheet/html/chunk.xsl\
	html.stylesheet="../common/tulip-default.css" \
	use.extensions=1 use.callouts=1 \
	callout.graphics.path="../common/"


