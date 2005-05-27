<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
  <xsl:strip-space elements="*"/>
  
  <xsl:template match="Waypoint">
    <xsl:variable name="result">
     <xsl:for-each select="@neighbors">
     <xsl:call-template name="checkNeighbors">
       <xsl:with-param name="okList" />
       <xsl:with-param name="toCheck" select="."/>
     </xsl:call-template>
     </xsl:for-each>     
    </xsl:variable>
    <xsl:copy>
      <xsl:apply-templates select="@*"/>
      <xsl:attribute name="neighbors"><xsl:value-of select="$result"/></xsl:attribute>
      <xsl:apply-templates select="*"/>
    </xsl:copy>
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
  
  <xsl:template name="checkNeighbors">
    <xsl:param name="okList" />
    <xsl:param name="toCheck" />
    <xsl:choose>
      <xsl:when test="$toCheck">
        <xsl:variable name="toTest">
          <xsl:choose>
            <xsl:when test="substring-before(normalize-space($toCheck),' ')">
              <xsl:value-of select="substring-before(normalize-space($toCheck),' ')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="normalize-space($toCheck)"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="rest" select="normalize-space(substring-after(normalize-space($toCheck),' '))"/>
        <xsl:variable name="result"><xsl:value-of select="$okList"/>
          <xsl:if test="//Waypoint[@id=$toTest]"><xsl:text> </xsl:text><xsl:value-of select="$toTest"/></xsl:if>
        </xsl:variable>
        <xsl:call-template name="checkNeighbors">
          <xsl:with-param name="okList" select="$result"/>
          <xsl:with-param name="toCheck" select="$rest"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="normalize-space($okList)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
</xsl:stylesheet>
