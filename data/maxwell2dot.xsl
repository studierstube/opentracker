<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
<xsl:output method="text" />

<xsl:template match="TrackerServer">
digraph "<xsl:value-of select=".//Network/@name"/>" {
<xsl:apply-templates/>
}
</xsl:template>

<xsl:template match="configuration" priority="2"/>

<!--
<xsl:template match="TrackerServer/*" priority="1">
<xsl:number count="*" format="a" level="any"/>[label="<xsl:value-of select="name(.)"/>"];
<xsl:apply-templates/>
</xsl:template>
-->

<xsl:template match="Station" priority="1">
<xsl:param name="parent"></xsl:param>
<xsl:number count="*" format="a" level="any"/>[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@name"/>"];
<xsl:if test="not(name(..)='TrackerServer')" >
<xsl:copy-of select="$parent"/> -> <xsl:number count="*" format="a" level="any"/>;
</xsl:if>
<xsl:apply-templates>
<xsl:with-param name="parent"><xsl:number count="*" format="a" level="any"/></xsl:with-param>
</xsl:apply-templates>
</xsl:template>

<xsl:template match="*">
<xsl:param name="parent"></xsl:param>
<xsl:number count="*" format="a" level="any"/>[label="<xsl:value-of select="name(.)"/>"];
<xsl:if test="not(name(..)='TrackerServer')" >
<xsl:copy-of select="$parent"/> -> <xsl:number count="*" format="a" level="any"/>;
</xsl:if>
<xsl:apply-templates>
<xsl:with-param name="parent"><xsl:number count="*" format="a" level="any"/></xsl:with-param>
</xsl:apply-templates>
</xsl:template>

</xsl:stylesheet>
