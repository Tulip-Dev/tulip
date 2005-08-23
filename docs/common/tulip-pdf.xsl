<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0' xmlns:fo="http://www.w3.org/1999/XSL/Format">
    <xsl:import href="http://docbook.sourceforge.net/release/xsl/current/fo/docbook.xsl"/>
    <xsl:param name="paper.type" select="'A4'" />
    <xsl:param name="pape.orientation" select="'portrait'" />
    <xsl:param name="chapter.autolabel" select="1" />
    <xsl:param name="section.autolabel" select="1" />
    <xsl:param name="section.label.includes.component.label" select="1" />
    <xsl:param name="use.extensions" select="1"/>
    <xsl:param name="use.callouts" select="1"/>
    <xsl:param name="callout.extension" select="1"/>
    <xsl:param name="callout.unicode" select="1"/>
    <xsl:param name="callout.graphics" select="0"/>
    <xsl:param name="ulink.show" select="1"/>
    <xsl:param name="ulink.footnotes" select="1"/>
    <xsl:param name="passivetex.extensions" select="1"/>
    <xsl:param name="variablelist.as.blocks" select="1"/>

    <xsl:template match="listitem/para">
	<xsl:apply-templates/>
    </xsl:template>
    <xsl:template match="calloutlist/callout/para">
	<xsl:apply-templates/>
    </xsl:template>
</xsl:stylesheet>


