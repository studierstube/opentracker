<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
	<xsl:output method="text"/>
<xsl:template match="OpenTracker">
digraph "<xsl:value-of select=".//NetworkSinkConfig/@name"/>" 
{ 
	<xsl:apply-templates/>
} 
</xsl:template>

<xsl:template match="configuration" priority="2"/>

<xsl:template name="options">
	<xsl:choose>
		<xsl:when test="name(.)='NetworkSink'">
			<xsl:call-template name="NetworkSink"/>
		</xsl:when>
		<xsl:when test="name(.)='ARToolKitSource'">
			<xsl:call-template name="ARToolKitSource"/>			
		</xsl:when>
		<xsl:when test="name(.)='ConsoleSink'">
			<xsl:call-template name="ConsoleSink"/>			
		</xsl:when>
		<xsl:when test="name(.)='NetworkSource'">
			<xsl:call-template name="NetworkSource"/>			
		</xsl:when>
		<xsl:when test="name(.)='TestSource'">
			<xsl:call-template name="TestSource"/>
		</xsl:when>
		<xsl:when test="name(.)='ConsoleSource'">
			<xsl:call-template name="ConsoleSource"/>
		</xsl:when>
		<xsl:otherwise>[label="<xsl:value-of select="name(.)"/>",shape=box]</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="NetworkSink">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@name"/> - <xsl:value-of select="@number"/>"]</xsl:template>

<xsl:template name="ARToolKitSource">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@tag-file"/>"]</xsl:template>

<xsl:template name="ConsoleSink">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@comment"/>"]</xsl:template>

<xsl:template name="NetworkSource">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>

<xsl:template name="TestSource">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>

<xsl:template name="ConsoleSource">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>

<xsl:template match="*">
	<xsl:param name="parent"/>
	<xsl:number count="*" format="a" level="any"/><xsl:call-template name="options"/>; 
	<xsl:if test="not(name(..)='OpenTracker')">
		 <xsl:number count="*" format="a" level="any"/> -> <xsl:copy-of select="$parent"/>; 
	</xsl:if>
	<xsl:apply-templates>
		<xsl:with-param name="parent">
			<xsl:number count="*" format="a" level="any"/>
		</xsl:with-param>
	</xsl:apply-templates>
</xsl:template>
</xsl:stylesheet>
