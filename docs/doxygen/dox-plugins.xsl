<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>
<xsl:output method="text"/>
<!-- éventuellement des paramètres -->
<xsl:template match="tagfile">
    <xsl:apply-templates select="compound[@kind = 'page']"/>
    <xsl:apply-templates select="compound[@kind = 'group']"/>
&lt;/body>&lt;/html>
</xsl:template>

<xsl:template match="compound[@kind = 'page']">
&lt;html>&lt;head>
 &lt;style>
&lt;!--
.description {
  display: none;
  border: 1px solid #aaa;
  background-color: #f9f9f9;
  padding: 5px;
  font-size: 95%;
  width: 600px
}
//-->
  &lt;/style>
  &lt;script language="javascript">
&lt;!--
function togglePluginFontWeight(id) {
  var plugin = document.getElementById('plugin_' + id);
   if (plugin.style["fontWeight"] != "bold")
     plugin.style["fontWeight"] = "bold";
   else {
    var description = document.getElementById('description_' + id);
    if (description.style.display != "block")
      plugin.style["fontWeight"] = "normal";
   } 
}
  
function expandCollapse(id) {
  var description = document.getElementById('description_' + id);
  var button = document.getElementById('button_' + id);
  if (description.style.display == "block") {
    description.style.display = "none";
  button.src = "../common/opened.gif";
  } else {
    description.style.display = "block";
  button.src = "../common/closed.gif";
  }
}
//-->
  &lt;/script>
&lt;/head>
&lt;body width="800" style="color=#000000; font-size: 11pt; font-family: sans-serif">
</xsl:template>
<xsl:template match="compound[@kind = 'group']">
  &lt;h3> 
  <xsl:value-of select="title"/>
  &lt;/h3>&lt;ul>
  <xsl:for-each select="class">
    <xsl:variable name="name_class" select="concat('class',current())"/>
    &lt;li>&lt;span id="plugin_<xsl:value-of select="current()"/>" style="cursor: pointer;" onclick="expandCollapse('<xsl:value-of select="current()"/>');" onmouseover="togglePluginFontWeight('<xsl:value-of select="current()"/>');" onmouseout="togglePluginFontWeight('<xsl:value-of select="current()"/>');">&lt;img src="../common/opened.gif" id="button_<xsl:value-of select="current()"/>"><xsl:value-of select="current()"/>&lt;/span>
   &lt;div id="description_<xsl:value-of select="current()"/>" class="description"> 
  <xsl:for-each select="document(concat('xml/class',current(),'.xml'))/doxygen/compounddef[@id=$name_class]">
    &lt;p>&lt;u>Description&lt;/u>&lt;br/>&lt;/p>
    <xsl:apply-templates select="briefdescription"/>
    <br/>
    <xsl:apply-templates select="detaileddescription"/>
    &lt;br/>
  </xsl:for-each>
  &lt;/div>&lt;/li>
  </xsl:for-each>
  &lt;/ul>
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
