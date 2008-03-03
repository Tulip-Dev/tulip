<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
    <xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/chunk.xsl"/>
    <xsl:param name="html.stylesheet" select="'../../common/tulip-default.css'"/>
    <xsl:param name="use.extensions" select="0"/>
    <xsl:param name="use.callouts" select="1"/>
    <xsl:param name="callout.graphics.path" select="'../../common/'"/>
    <xsl:param name="generate.toc" select="'book toc,title chapter toc,title'"/>
    <xsl:param name="section.label.includes.component.label" select="0"/>
    <xsl:param name="section.autolabel" select="1"/>
    <xsl:param name="section.autolabel.max.depth" select="1"/>
    <xsl:param name="base.dir" select="'html/'"/>
    <xsl:param name="img.src.path" select="'../'"/>
</xsl:stylesheet>


