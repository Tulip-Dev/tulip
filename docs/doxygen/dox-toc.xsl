<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:output method="text"/>
<!-- éventuellement des paramètres -->
<xsl:template match="tagfile">
    <xsl:apply-templates select="compound[@kind = 'page']"/>
    <xsl:apply-templates select="compound[@kind = 'group']"/>
&lt;/section>
</xsl:template>

<xsl:template match="compound[@kind = 'page']">
    &lt;section ref="doxygen/allPlugins.html" title="<xsl:value-of select="title"/>">
</xsl:template>
<xsl:template match="compound[@kind = 'group']">
    &lt;section ref="doxygen/allPlugins.html#<xsl:value-of select="name"/>" title="<xsl:value-of select="title"/>">
        <xsl:for-each select="class">
            &lt;section ref="doxygen/allPlugins.html#<xsl:value-of select="current()"/>" title="<xsl:value-of select="current()"/>">
            &lt;/section>
        </xsl:for-each>
    &lt;/section>
</xsl:template>

</xsl:stylesheet>



