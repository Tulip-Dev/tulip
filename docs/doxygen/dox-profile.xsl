<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:output method="text"/>
<!-- éventuellement des paramètres -->
<xsl:template match="tagfile">
    <xsl:apply-templates select="compound[@kind = 'page']"/>
    <xsl:apply-templates select="compound[@kind = 'group']"/>
&lt;/dcf>
</xsl:template>

<xsl:template match="compound[@kind = 'page']">
    &lt;dcf ref="doxygen/tulip-plugins/<xsl:value-of select="filename"/>.html" title="<xsl:value-of select="title"/>">
</xsl:template>
<xsl:template match="compound[@kind = 'group']">
    &lt;section ref="doxygen/tulip-plugins/<xsl:value-of select="filename"/>" title="<xsl:value-of select="title"/>">
    &lt;/section>
</xsl:template>

<!--<xsl:template match="compound[@kind = 'class']">
     &lt;keyword ref="doxygen/tulip-plugins/<xsl:value-of select="filename"/>"><xsl:value-of select="name"/>&lt;/keyword>"/>
</xsl:template>-->

<!--<xsl:template match="compound[@kind = 'class']">
    <xsl:if test="(base = 'Clustering') or (base   = 'Export') or (base  = 'Import') or (base  = 'Layout') or (base  = 'Metric') or (base  = 'Selection')">
        <xsl:attribute name="{name}" namespace="Clustering">
    &lt;keyword ref="doxygen/tulip-plugins/<xsl:value-of select="filename"/>"><xsl:value-of select="name"/>&lt;/keyword>"/></xsl:attribute>
    </xsl:if>
</xsl:template>-->

<!--<xsl:template match="compound" mode="class">
    <xsl:if test="@kind ='class'">
        <xsl:if test="(base = 'Clustering') or (base   = 'Export') or (base  = 'Import') or (base  = 'Layout') or (base  = 'Metric') or (base  = 'Selection')">
            &lt;keyword ref="doxygen/tulip-plugins/<xsl:value-of select="filename"/>"><xsl:value-of select="name"/>&lt;/keyword>
            &lt;section ref="<xsl:value-of select="filename"/>" title="<xsl:value-of select="name"/>">
            &lt;/section>
        </xsl:if>
    </xsl:if>
</xsl:template>
-->

<!---->
</xsl:stylesheet>



