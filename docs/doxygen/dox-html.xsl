<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:output method="text"/>
<!-- éventuellement des paramètres -->

<xsl:template match="tagfile">
    <xsl:apply-templates select="compound[@kind = 'page']"/>
    <xsl:apply-templates select="compound[@kind = 'group']"/>
</xsl:template>

<xsl:template match="compound[@kind = 'page']">
&lt;table width="100%" style="border : solid 1px #597ba8 ; border-right-style: none; border-top-style: none; border-left-style: none;">&lt;tr>&lt;td>Plug-ins Reference&lt;/td>&lt;td align="right">
&lt;img src="../common/logo.png" width="32"/>&lt;/td>&lt;/tr>&lt;/table>
</xsl:template>
<xsl:template match="compound[@kind = 'group']">
    &lt;a name="<xsl:value-of select="name"/>">
        &lt;table border="0" width="100%" align="bottom" height="70px" bgcolor="#E5E5E5">&lt;tr>&lt;td>&lt;h1 align="right"><xsl:value-of select="title"/> &lt;/h1>&lt;/td>&lt;/tr>&lt;/table>
    &lt;/a>
        <xsl:for-each select="class">
            <xsl:variable name="name_class" select="concat('class',current())"/>
            &lt;a name="<xsl:value-of select="current()"/>">&lt;br/>
            &lt;h2 align="left"><xsl:value-of select="current()"/>&lt;hr style="border:1px; background-color:#597ba8; height:1px"/>&lt;/h2>&lt;/a>
            <xsl:for-each select="document(concat('xml/class',current(),'.xml'))/doxygen/compounddef[@id=$name_class]">
                &lt;p>&lt;u>Description&lt;/u>&lt;br/>&lt;/p>
                <xsl:apply-templates select="briefdescription"/>
                <br/>
                <xsl:apply-templates select="detaileddescription"/>

                &lt;br/>
            </xsl:for-each>
        </xsl:for-each>
</xsl:template>

<xsl:template match="briefdescription|detaileddescription">
    <xsl:apply-templates select="para"/>
</xsl:template>

<xsl:template match="para">
    &lt;p>
        <xsl:apply-templates/>
    &lt;/p>
</xsl:template>

<xsl:template match="ulink">
    &lt;a href="<xsl:value-of select="@url"/>"><xsl:value-of select="current()"/>&lt;/a>
</xsl:template>

<xsl:template match="simplesect[@kind='author']">
    &lt;dl>&lt;dt>&lt;b>AUTHORS&lt;/b>&lt;/dt>
    &lt;dd>
        <xsl:apply-templates/>
    &lt;/dd>&lt;/dl>
</xsl:template>

<xsl:template match="simplesect[@kind='note']">
    &lt;dl>&lt;dt>&lt;b>NOTES&lt;/b>&lt;/dt>
    &lt;dd>
        <xsl:apply-templates/>
    &lt;/dd>&lt;/dl>
</xsl:template>

<xsl:template match="simplesect[@kind='version']">
    &lt;dl>&lt;dt>&lt;b>VERSION&lt;/b>&lt;/dt>
    &lt;dd>
        <xsl:apply-templates/>
    &lt;/dd>&lt;/dl>
</xsl:template>
<xsl:template match="itemizedlist">
    &lt;ul>
        <xsl:apply-templates/>
    &lt;/ul>
</xsl:template>
<xsl:template match="listitem">
    &lt;li>
        <xsl:apply-templates/>
    &lt;/li>
</xsl:template>

<xsl:template match="bold">
    &lt;b>
        <xsl:apply-templates/>
    &lt;/b>
</xsl:template>

<xsl:template match="ref">
    &lt;font color="#6c94cb">
        <xsl:apply-templates/>
    &lt;/font>
</xsl:template>

<xsl:template match="linebreak">
    &lt;br/>
    <xsl:apply-templates/>
</xsl:template>
</xsl:stylesheet>



<!--<xsl:for-each select="document('tulip-plugins.tag')/tagfile/compound[@kind ='class']">
                <xsl:if test="$name_class = name">
            &lt;keyword ref="doxygen/tulip-plugins/<xsl:value-of select="filename"/>"><xsl:value-of select="name"/>&lt;/keyword>
                </xsl:if>
            </xsl:for-each>-->
