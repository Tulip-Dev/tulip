<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:output method="text"/>
<!-- éventuellement des paramètres -->
<xsl:template match="tagfile">
    <xsl:apply-templates select="compound[@kind = 'group']"/>
</xsl:template>

<xsl:template match="compound[@kind = 'group']">
     <xsl:for-each select="class">
         &lt;keyword name="<xsl:value-of select="current()"/>" ref="./doxygen/allPlugins.html#<xsl:value-of select="current()"/>" id="<xsl:value-of select="current()"/>"/>
        </xsl:for-each>
</xsl:template>

</xsl:stylesheet>
