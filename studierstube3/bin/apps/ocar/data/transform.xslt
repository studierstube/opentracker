<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
  <xsl:strip-space elements="*"/>
  
  <xsl:param name="translation" select="'0 0 0'"/>
  <xsl:param name="scale" select="'1 1 1'"/>
  <xsl:param name="tx" select="number(substring-before($translation,' '))"/>
  <xsl:param name="ty" select="number(substring-before(substring-after($translation,' '),' '))"/>
  <xsl:param name="tz" select="number(substring-after(substring-after($translation,' '),' '))"/>
  <xsl:param name="sx" select="number(substring-before($scale,' '))"/>
  <xsl:param name="sy" select="number(substring-before(substring-after($scale,' '),' '))"/>
  <xsl:param name="sz" select="number(substring-after(substring-after($scale,' '),' '))"/>
  
  <xsl:template match="Transformation">
    <xsl:variable name="x" select="number(substring-before(@translation,' '))"/>
    <xsl:variable name="y" select="number(substring-before(substring-after(@translation,' '),' '))"/>
    <xsl:variable name="z" select="number(substring-after(substring-after(@translation,' '),' '))"/>
    <xsl:element name="Transformation">
      <xsl:if test="@translation">
        <xsl:attribute name="translation"><xsl:value-of select="$x*$sx+$tx"/><xsl:text> </xsl:text><xsl:value-of select="$y*$sy+$ty"/><xsl:text> </xsl:text><xsl:value-of select="$z*$sz+$tz"/></xsl:attribute>
      </xsl:if>
    </xsl:element>
  </xsl:template>
  
  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
