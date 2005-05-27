<?xml version="1.0" encoding="UTF-8"?>
<!--=======================================================================-->
<!-- This transformation file collects templates to transform bauml into
     Open Inventor files for general purpose such as editing etc. Ideally
     these templates should be reused in more specific transformations.
          
     @author Gerhard Reitmayr
     $Id: bauml2info.xslt 3427 2004-07-14 12:20:49Z tamer $
-->
<!--=======================================================================-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:info="http://www.studierstube.org/info">
<!-- include some tools for list and set manipulation here -->
<xsl:include href="bauml2iv.xslt"/>
<!-- setting output method -->
<xsl:output method="text" encoding="ASCII"/>

<xsl:template match="/">#Inventor V2.1 ascii
# information viz structure for the YARD demonstrator

DEF InfoData SoMultiSwitch { 
    ordered TRUE
    whichChildren -2

<xsl:apply-templates select="*"/>
}
</xsl:template>

<!--=======================================================================-->
<!-- This template matches the special marked info items and will generate
     the representing Open Inventor structure. It depends on helper
     templates from bauml2iv.xslt.
     -->
<!--=======================================================================-->
<xsl:template match="*[@baseType='SpatialObjectType'][annotation/info:info]" priority="2">
<xsl:call-template name="GenerateDefName"/>Separator { 
    DEF ROOM Info { string "<xsl:value-of select="annotation/info:info/info:room"/>" }
    DEF TEL Info { string "<xsl:value-of select="annotation/info:info/info:tel"/>" }
    DEF PERSON Info { string "<xsl:value-of select="annotation/info:info/info:description"/>" }
<xsl:call-template name="GenerateTransformation"/>
    SoContextSwitch {
        index 11
        defaultChild 0
        # visible style
        Separator {
            DrawStyle { style LINES lineWidth 3 }
            Material { diffuseColor 1 1 1 emissiveColor 1 1 1 }
            DEF FACE <xsl:call-template name="GenerateFaceSet"/>
        }
        # selected style 
        Separator {
            Material { diffuseColor 1 1 1 emissiveColor 1 1 0 transparency 0.5 }
            USE FACE
            Transform { translation <xsl:value-of select="representation/Vertex[position()=2]/@position" />}
            Transform { translation 0 -0.2 0 }
            Material { diffuseColor 0.9 0.9 0.1 }
            FontStyle { family SANS size 18 }
            GLTweak { depthTest FALSE }
            DEF TEXT Text2 { justification LEFT
              string [ "<xsl:value-of select="annotation/info:info/info:room"/>", "<xsl:value-of select="annotation/info:info/info:tel"/>", "<xsl:value-of select=" annotation/info:info/info:description"/>" ]
            }
            GLTweak { depthTest TRUE }
        }
        # Show style
        Separator {
            DrawStyle { style LINES lineWidth 3 }
            Material { diffuseColor 1 1 1 emissiveColor 1 1 1 }
            USE FACE
            Transform { translation <xsl:value-of select="representation/Vertex[position()=2]/@position" />}
            Transform { translation 0 -0.2 0 }
            Material { diffuseColor 0.9 0.9 0.1 }
            FontStyle { family SANS size 18 }
            GLTweak { depthTest FALSE }
            USE TEXT         
            GLTweak { depthTest TRUE }
        }
    }
}
</xsl:template>

<!--=======================================================================-->
<!-- These templates override the matching templates in the standard bauml2iv
     stylesheet because of their higher priority. They will only pass on 
     processing but not generate any iv content.
     -->
<!--=======================================================================-->
<xsl:template match="*[@baseType='SpatialObjectType']" priority="1"/>
<xsl:template match="*[@baseType='SpatialContainerType']" priority="1">
<xsl:apply-templates select="children/*"/>
</xsl:template>
</xsl:stylesheet>
