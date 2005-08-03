all : 

docs : html

check :
	$(XMLLINT) --valid -noout index.docbook

clean :
	rm -f *.html *.out *.log *.out *.fo *.aux *.dvi *.pdf

html : index.html

index.html : index.docbook ../common/tulip-html.xsl ../common/CatalogManager.properties
	$(JAVA) --classpath "$(top_builddir)/thirdparty/saxon/saxon653.jar:$(top_builddir)/thirdparty/saxon/saxon.jar:../common:$(top_builddir)/thirdparty/xml-commons/resolver.jar"\
	com.icl.saxon.StyleSheet \
	-x org.apache.xml.resolver.tools.ResolvingXMLReader \
	-y org.apache.xml.resolver.tools.ResolvingXMLReader \
	-r org.apache.xml.resolver.tools.CatalogResolver -u\
	index.docbook  ../common/tulip-html.xsl


pdf :   devHandbook.fo devHandbook.pdf

devHandbook.fo : index.docbook ../common/tulip-pdf.xsl ../common/CatalogManager.properties
	$(JAVA) --classpath "$(top_srcdir)/thirdparty/saxon/saxon653.jar:$(top_srcdir)/thirdparty/saxon/saxon.jar:../common:/usr/share/java/xml-commons-resolver.jar" \
	com.icl.saxon.StyleSheet -r org.apache.xml.resolver.tools.CatalogResolver \
	-o  devHandbook.fo index.docbook  ../common/tulip-pdf.xsl

devHandbook.pdf : devHandbook.fo
	pdfxmltex devHandbook.fo && pdfxmltex devHandbook.fo



