<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:saxon="http://icl.com/saxon" extension-element-prefixes="saxon">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
  <xsl:strip-space elements="*"/>
  
  <xsl:param name="min" select="'0 0 0'"/>
  <xsl:param name="max" select="'0 0 0'"/>

  <xsl:variable name="xmin" select="number(substring-before(normalize-space($min),' '))"/>
  <xsl:variable name="ymin" select="number(substring-before(normalize-space(substring-after(normalize-space($min),' ')),' '))"/>
  <xsl:variable name="zmin" select="number(substring-after(normalize-space(substring-after(normalize-space($min),' ')),' '))"/>
  <xsl:variable name="xmax" select="number(substring-before(normalize-space($max),' '))"/>
  <xsl:variable name="ymax" select="number(substring-before(normalize-space(substring-after(normalize-space($max),' ')),' '))"/>
  <xsl:variable name="zmax" select="number(substring-after(normalize-space(substring-after(normalize-space($max),' ')),' '))"/>
  
  <xsl:template match="Building">
    <xsl:variable name="test">
      <xsl:call-template name="testBuilding">
        <xsl:with-param name="xmin" select="$xmin"/>
        <xsl:with-param name="ymin" select="$ymin"/>
        <xsl:with-param name="zmin" select="$zmin"/>
        <xsl:with-param name="xmax" select="$xmax"/>
        <xsl:with-param name="ymax" select="$ymax"/>
        <xsl:with-param name="zmax" select="$zmax"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:if test="$test = 'true'">
      <xsl:copy>
        <xsl:apply-templates select="*|@*"/>
      </xsl:copy>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="Waypoint">
    <xsl:variable name="test">
      <xsl:call-template name="testPosition">
        <xsl:with-param name="xmin" select="$xmin"/>
        <xsl:with-param name="ymin" select="$ymin"/>
        <xsl:with-param name="zmin" select="$zmin"/>
        <xsl:with-param name="xmax" select="$xmax"/>
        <xsl:with-param name="ymax" select="$ymax"/>
        <xsl:with-param name="zmax" select="$zmax"/>
        <xsl:with-param name="position" select="pose/Transformation/@translation"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:if test="$test = 'true'">
      <xsl:copy>
        <xsl:apply-templates select="*|@*"/>
      </xsl:copy>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="*|@*">
    <xsl:copy>
      <xsl:apply-templates select="*|@*"/>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="annotation">
    <xsl:copy>
      <xsl:apply-templates mode="copy" />
    </xsl:copy>
  </xsl:template>

  <xsl:template match="node()|@*" mode="copy">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*" mode="copy"/>
    </xsl:copy>
  </xsl:template>
   
  <xsl:template name="testBuilding">
    <xsl:param name="xmin" select="0"/>
    <xsl:param name="ymin" select="0"/>
    <xsl:param name="zmin" select="0"/>
    <xsl:param name="xmax" select="0"/>
    <xsl:param name="ymax" select="0"/>
    <xsl:param name="zmax" select="0"/>
    <xsl:variable name="result" select="0" saxon:assignable="yes"/>
    <xsl:for-each select="representation/Vertex">
      <xsl:variable name="test">
        <xsl:call-template name="testPosition">
          <xsl:with-param name="xmin" select="$xmin"/>
          <xsl:with-param name="ymin" select="$ymin"/>
          <xsl:with-param name="zmin" select="$zmin"/>
          <xsl:with-param name="xmax" select="$xmax"/>
          <xsl:with-param name="ymax" select="$ymax"/>
          <xsl:with-param name="zmax" select="$zmax"/>
          <xsl:with-param name="position" select="@position"/>
        </xsl:call-template>
      </xsl:variable>
      <saxon:assign name="result" select="boolean($result) or ($test = 'true')"/>
    </xsl:for-each>    
    <xsl:value-of select="$result"/>    
  </xsl:template>
  
  <xsl:template name="testPosition">
    <xsl:param name="xmin" select="0"/>
    <xsl:param name="ymin" select="0"/>
    <xsl:param name="zmin" select="0"/>
    <xsl:param name="xmax" select="0"/>
    <xsl:param name="ymax" select="0"/>
    <xsl:param name="zmax" select="0"/>
    <xsl:param name="position" select="'0 0 0'"/>
    <xsl:variable name="x" select="number(substring-before($position,' '))"/>
    <xsl:variable name="y" select="number(substring-before(normalize-space(substring-after($position,' ')),' '))"/>
    <xsl:variable name="z" select="number(substring-after(normalize-space(substring-after($position,' ')),' '))"/>
    <xsl:choose>
      <xsl:when test="($x >= $xmin) and ($xmax >= $x) and ($y >= $ymin) and ($ymax >= $y) and ($z >= $zmin) and ($zmax >= $z)">true</xsl:when>
      <xsl:otherwise>false</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
