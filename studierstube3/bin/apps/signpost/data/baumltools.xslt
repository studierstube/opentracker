<?xml version="1.0" encoding="UTF-8"?>
<!--=======================================================================-->
<!-- This transformation file collects templates to work with bauml. For
     example, it contains the basic building blocks to transform bauml into
     Open Inventor files for general purpose such as editing etc. Ideally
     these templates should be reused in more specific transformations.
     There are no active templates in this file, all need to be called by
     name.
     
     See bauml2iv.xslt for an active transformation file.
          
     Generator templates 
       GenerateFaceSet
       GenerateTransformation
       GenerateObjectIcon
       GenerateDefName
     
     @author Gerhard Reitmayr
     $Id: baumltools.xslt 3427 2004-07-14 12:20:49Z tamer $
-->
<!--=======================================================================-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <!-- setting output method -->
  <xsl:output method="text" encoding="ASCII"/>
  <!--=======================================================================-->
  <!-- GenerateFaceSet
     This template generates an IndexedFaceSet from the current node. It
     assumes that the current node is derived from SpatialObjectType. If no
     presentation part is present, it will generate nothing. It
     does not include the transformation part.
     -->
  <!--=======================================================================-->
  <xsl:template name="GenerateFaceSet">
    <xsl:if test="representation">
    IndexedFaceSet {
        vertexProperty VertexProperty {
            vertex [ <xsl:for-each select="representation/Vertex">
        <xsl:value-of select="@position"/>
        <xsl:if test="not(position()=last())">,
                     </xsl:if>
      </xsl:for-each>
            ]
        }
        coordIndex [ <xsl:for-each select="representation/Polygon">
        <xsl:value-of select="translate(normalize-space(@vertices),' ',',')"/>,-1<xsl:if test="not(position()=last())">,
                     </xsl:if>
      </xsl:for-each>
        ]
    }</xsl:if>
  </xsl:template>
  <!--=======================================================================-->
  <!-- GenerateTransformation
     This template generates a transform from the current node. It
     assumes that the current node is derived from SpatialObjectType. If no
     transformation part is present, it will generate nothing..
     -->
  <!--=======================================================================-->
  <xsl:template name="GenerateTransformation">
    <xsl:if test="pose/Transformation">    Transform {
        <xsl:if test="pose/Transformation/@translation">translation <xsl:value-of select="pose/Transformation/@translation"/>
        <xsl:text>
        </xsl:text>
      </xsl:if>
      <xsl:if test="pose/Transformation/@rotation and pose/Transformation/@rotationtype = 'axisangle'">rotation <xsl:value-of select="pose/Transformation/@rotation"/>
        <xsl:text>
        </xsl:text>
      </xsl:if>
      <xsl:if test="pose/Transformation/@scale">scaleFactor <xsl:value-of select="pose/Transformation/@scale"/>
        <xsl:text>
        </xsl:text>
      </xsl:if>}
</xsl:if>
  </xsl:template>
  <!--=======================================================================-->
  <!-- GenerateObjectIcon
     This template generates an icon for any object without a visual
     representation. as there is no pose information associated with
     such an objects, the icons are subject to the local transformation
     per default.
     -->
  <!--=======================================================================-->
  <xsl:template name="GenerateObjectIcon">    
</xsl:template>
  <!--=======================================================================-->
  <!-- GenerateDefName
     Tests for an @id attribute present and generates the 'DEF id' string 
     if it is present.
     -->
  <!--=======================================================================-->
  <xsl:template name="GenerateDefName">
    <xsl:if test="@id">DEF <xsl:value-of select="@id"/>
      <xsl:text> </xsl:text>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>
