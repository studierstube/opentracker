<?xml version="1.0" encoding="UTF-8"?>
<!--=======================================================================-->
<!-- This transformation file collects templates to transform bauml into
     Open Inventor files for general purpose such as editing etc. Ideally
     these templates should be reused in more specific transformations.
     
     Transformation templates
       TransformObject
       TransformSpatialObject
       TransformSpatialContainer
          
     @author Gerhard Reitmayr
     $Id: bauml2iv.xslt 3427 2004-07-14 12:20:49Z tamer $
-->
<!--=======================================================================-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <!-- setting output method -->
  <xsl:output method="text" encoding="ASCII"/>
  <!-- including some tools -->
  <xsl:include href="baumltools.xslt"/>
  <!-- basic template that creates the necessary inventor file stuff for testing -->
  <xsl:template match="/" priority="-1">#Inventor V2.1 ascii
<xsl:apply-templates select="*"/>
  </xsl:template>
  <!--=======================================================================-->
  <!-- TransformObject
     This template transforms any element derived from a ObjectType. As these
     objects do not define a spatial representation, they are represented in 
     an abstract way, using an icon that is defined in the template 
     GenerateObjectIcon and can be overriden by an importing style sheet.
     It will match these elements or can be called directly.
     -->
  <!--=======================================================================-->
  <xsl:template name="TransformObject" match="*[@baseType='ObjectType']">
    <xsl:text>
</xsl:text>
    <xsl:call-template name="GenerateDefName"/>Separator { # represents <xsl:value-of select="name(.)"/> - <xsl:value-of select="@id"/>
    <xsl:text>
</xsl:text>
    <xsl:call-template name="GenerateObjectIcon"/>
}
</xsl:template>
  <!--=======================================================================-->
  <!-- TransformSpatialObject
     This template transforms any element derived from a SpatialObjectType.
     It will match these elements or can be called directly.
     -->
  <!--=======================================================================-->
  <xsl:template name="TransformSpatialObject" match="*[@baseType='SpatialObjectType']">
    <xsl:text>
</xsl:text>
    <xsl:call-template name="GenerateDefName"/>Separator { # represents <xsl:value-of select="name(.)"/> - <xsl:value-of select="@id"/>
    <xsl:text>
</xsl:text>
    <xsl:call-template name="GenerateTransformation"/>
    <xsl:call-template name="GenerateFaceSet"/>
}
</xsl:template>
  <!--=======================================================================-->
  <!-- TransformSpatialContainer
     This template transforms any element derived from a SpatialContainerType.
     It creates the appropriate scene graph hierarchy for any children.
     It will match these elements or can be called directly. 
     -->
  <!--=======================================================================-->
  <xsl:template name="TransformSpatialContainer" match="*[@baseType='SpatialContainerType']">
    <xsl:text>
</xsl:text>
    <xsl:call-template name="GenerateDefName"/>Separator { # represents <xsl:value-of select="name(.)"/> - <xsl:value-of select="@id"/>
    <xsl:text>
</xsl:text>
    <xsl:call-template name="GenerateTransformation"/>
    <xsl:call-template name="GenerateFaceSet"/>
    <xsl:if test="children/*"># children for <xsl:value-of select="@id"/>
      <xsl:text>
</xsl:text>
    </xsl:if>
    <xsl:apply-templates select="children/*"/>
}
</xsl:template>
</xsl:stylesheet>
