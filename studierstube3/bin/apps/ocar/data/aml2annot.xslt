<?xml version="1.0" encoding="UTF-8"?>
<!-- This transformation file converts the general bauml data to 
     datastructures required for the ocar annotation application.
     
     @author Gerhard Reitmayr
     $Id: aml2annot.xslt 3427 2004-07-14 12:20:49Z tamer $
-->
<?xmlspysamplexml smallmodel.xml?>
<xsl:stylesheet version="1.1" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:saxon="http://icl.com/saxon" extension-element-prefixes="saxon">
  <!-- include some tools for list and set manipulation here -->
  <xsl:include href="tools.xslt"/>
  <xsl:include href="../../signpost/data/baumltools.xslt"/>
  <!-- setting output method -->
  <xsl:output method="text" encoding="UTF-8"/>
  
<!--=======================================================================-->
<!-- parameter section
     this parameters can be set from the command line of your XSLT processor 
     or here, if you like. They influence various aspects of the stylesheet
     such as output files. -->
<!--=======================================================================-->
  <xsl:param name="keywords"/>
  <xsl:param name="nestedItems"/>
  <xsl:param name="omitIdentityTransforms" select="true"/>
  <xsl:param name="template"/>
  <!-- these parameters control what information to output from the annotation items -->
  <xsl:param name="model"/>
  <xsl:param name="indexmap"/>
  <xsl:param name="keywordmap"/>
  <xsl:param name="keywordlist" select="'list.iv'"/>
  <!-- includes the templates that actually construct the representations -->
  <!-- <xsl:import href="$template"/> -->
  <!-- Here starts the processing -->
  <xsl:template match="/">
    <xsl:if test="$model">
      <xsl:call-template name="model"/>
    </xsl:if>
    <xsl:if test="$indexmap">
      <xsl:call-template name="indexmap"/>
    </xsl:if>
    <xsl:if test="$keywordmap">
      <xsl:call-template name="keywordmap"/>
    </xsl:if>
    <xsl:if test="$keywordlist">
      <xsl:call-template name="keywordlist"/>
    </xsl:if>
  </xsl:template>

<!--=======================================================================-->
<!-- model section
     creates the models for the annotation objects
 -->
<!--=======================================================================-->
  <xsl:template name="model">
    <xsl:param name="currenttrans" select="'0 0 0'"/>
    <xsl:param name="currentrot" select="'0 0 0 1'"/>
    <xsl:document href="{$model}" mode="text">#Inventor V2.1 ascii
# annotation data from AML source file by aml2annot.xslt
SoMultiSwitch { 
    ordered TRUE
    whichChildren -2
  <xsl:apply-templates select="*">
        <xsl:with-param name="currenttrans" select="$currenttrans"/>
        <xsl:with-param name="currentrot" select="$currentrot"/>
        <xsl:with-param name="currentkeywords"/>
      </xsl:apply-templates>
}
    </xsl:document>
  </xsl:template>
  <!-- walks through the tree of SpatialContainerType objects and updates transformation -->
  <xsl:template match="*[@baseType='SpatialContainerType']" name="ContainerObject">
    <xsl:param name="currenttrans" select="'0 0 0'"/>
    <xsl:param name="currentrot" select="'0 0 1 0'"/>
    <xsl:param name="currentkeywords"/>
    <!-- currently no flattening of transformation hierarchies !
  <xsl:variable name="localtranslation">
    <xsl:choose>
      <xsl:when test="pose/Transformation/@translation">
        <xsl:call-template name="update_translation">
          <xsl:param name="pathtrans" select="$currenttrans"/>
          <xsl:param name="pathrot" select="$currentrot"/>
          <xsl:param name="localtrans" select="pose/Transformation/@translation"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$currenttrans"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="localrotation">
    <xsl:choose>
      <xsl:when test="pose/Transformation/@rotation">
        <xsl:call-template name="update_rotation">
          <xsl:param name="pathtrans" select="$currenttrans"/>
          <xsl:param name="pathrot" select="$currentrot"/>
          <xsl:param name="localrot" select="pose/Transformation/@rotation"/>
          <xsl:param name="rottype" select="pose/Transformation/@rotationType"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$currentrot"/>
      </xsl:otherwise>
    </xsl:choose>  
  </xsl:variable>
  -->
    <!-- compute result tree fragment for children -->
    <xsl:variable name="childresults">
      <xsl:apply-templates select="children/*">
        <xsl:with-param name="currenttrans" select="$currenttrans"/>
        <xsl:with-param name="currentrot" select="$currentrot"/>
        <xsl:with-param name="currentkeywords" select="concat($currentkeywords, annotation/Keywords)"/>
      </xsl:apply-templates>
    </xsl:variable>
    <!-- only do a separator and a transform, if there is a transform or empty transforms should not be omitted -->
    <xsl:if test="not(string($childresults) = '')">
      <xsl:if test="((pose/Transformation/@translation or pose/Transformation/@rotation or pose/Transformation/@scale) or not($omitIdentityTransforms)) and $nestedItems">
        <xsl:if test="@id">DEF <xsl:value-of select="@id"/>
        </xsl:if> Separator { 
    <!-- <xsl:call-template name="generateTransform" /> -->
      </xsl:if>
      <xsl:value-of select="$childresults"/>
      <xsl:if test="((pose/Transformation/@translation or pose/Transformation/@rotation or pose/Transformation/@scale) or not($omitIdentityTransforms)) and $nestedItems">
} #  <xsl:if test="@id">
          <xsl:value-of select="@id"/>
          <xsl:text>
  </xsl:text>
        </xsl:if>
      </xsl:if>
    </xsl:if>
  </xsl:template>
  <!-- walks through SpatialObjectType and tries to match with keywords -->
  <xsl:template match="Annotation" name="SpatialObject">
    <xsl:param name="currenttrans" select="'0 0 0'"/>
    <xsl:param name="currentrot" select="'0 0 1 0'"/>
    <xsl:param name="currentkeywords"/>
    <xsl:variable name="testResult">
      <xsl:call-template name="testKeywords">
        <xsl:with-param name="given" select="concat($currentkeywords, annotation/Keywords)"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:if test="$testResult = 1">
      <xsl:apply-templates mode="representation" select="."/>
    </xsl:if>
  </xsl:template>
  <!-- tests the annotation data for the globally specified keywords -->
  <xsl:template name="testKeywords">
    <xsl:param name="given" select="annotation/Keywords"/>
    <xsl:call-template name="isSubsetOf">
      <xsl:with-param name="a" select="$keywords"/>
      <xsl:with-param name="b" select="$given"/>
    </xsl:call-template>
  </xsl:template>
  <!-- catches all text nodes to not output anything ! -->
  <xsl:template match="text()"/>

<!--=======================================================================-->
<!-- the templates that actually construct the representation. These are
     simple ones that can be overriden by using the template parameter to 
     include another stylesheet that defines specialized ones              -->
<!--=======================================================================-->
  <xsl:template match="Annotation" mode="representation" priority="0.1">
    <xsl:if test="annotation/Content">
      <xsl:if test="@id">DEF <xsl:value-of select="@id"/>
      </xsl:if> Separator { # item <xsl:value-of select="@id"/>
      <xsl:text>
      </xsl:text>
    SoContextSwitch {
      index 11
      defaultChild 0
      <!-- the first child is the 3D representation used for picking and
           is derived from the representation part of the schema -->
      <xsl:call-template name="rep-pick"/>
      <!-- the second child is the 3D icon displayed and
           is derived from the representation part of the schema -->
      <xsl:call-template name="rep-icon"/>
      <!-- the third child is the actual content displayed to the user. 
           this is derived from the annotation/Content element -->
      <xsl:call-template name="rep-content"/>
    }    
  } # end item <xsl:value-of select="@id"/>
      <xsl:text>
  </xsl:text>
    </xsl:if>
  </xsl:template>

  <!-- creates a simple geometry for picking -->
  <xsl:template name="rep-pick"># Pick geometry
      Separator {                
        <xsl:call-template name="GenerateTransformation"/>
        DEF GEOM <xsl:choose>
      <xsl:when test="representation">
        <xsl:call-template name="GenerateFaceSet"/>
      </xsl:when>
      <xsl:otherwise>
            Cube { width 2 depth 2 height 2 }
          </xsl:otherwise>
    </xsl:choose>
      }</xsl:template>
      
  <!-- creates a shape node with transformation and switches on content type using matching -->
  <xsl:template name="rep-icon"># Rendering geometry 
      Separator {
        <xsl:call-template name="GenerateTransformation"/>
        DrawStyle { style FILLED }
        LightModel { model BASE_COLOR }
        SoStencilBuffer {            
            clearValue 0
            clear TRUE
            zPass REPLACE
            zFail REPLACE
            fail REPLACE
            function ALWAYS
            ref 1
            mask 1
        }        
        SoFrameBuffer { mask NONE }
        DepthBuffer { enable FALSE }
        USE GEOM
        SoFrameBuffer { mask ALL }
        SoStencilBuffer {
            function NOTEQUAL
            ref  1
            mask 1
            zPass KEEP
            zFail KEEP
            fail KEEP
        }
        Transform { scaleFactor 1.05 1.05 1.05 }
        Material { diffuseColor 1 1 0 }
        USE GEOM
      }</xsl:template>

<!-- switches on content type by using template matching -->
<xsl:template name="rep-content"># Content geometry
    Group { 
        Separator {  # 2D display
            DEF HUD_CAM OrthographicCamera { aspectRatio 1.33333 viewportMapping ADJUST_CAMERA }
            Transform { translation -1.25 +0.85 0 }
          DEF CONTENT2D Separator { # Content Group
            DEF PARENT Separator {  # Title geometry        
                Separator {
                    SurroundScale { numNodesUpToContainer 2 numNodesUpToReset 1 }
                    Transform { translation 0 0 -2 scaleFactor 1.05 1.1 1.1 }
                    Material { diffuseColor .9 0.9 0.2 }
                    Cube {}
                }
                Material { diffuseColor 0.4 0.4 1 }
                Font { name "Arial Fett" size 0.07 }
                AsciiText { justification LEFT string "<xsl:value-of select="annotation/Title"/>" }
            }
<xsl:apply-templates mode="rep-content-2D" select="annotation/Content"/>
          } # end content group
          Transform { translation 0 0 -1 }
          DEF Panel Separator {
    DEF CORNERRADIUS Info { string "0.1" }
    DEF XPADDING Info { string "0.05" }
    DEF Body Separator { 
        Transform {
            translation = DEF BODYCALC Calculator {
                C = DEF BBOX ComputeBoundingBox { 
                    node USE CONTENT2D
                }.boxCenter
                A = USE BBOX.min
                B = USE BBOX.max
                a = USE XPADDING.string
                expression [ "oA = B - A; oA[0] = oA[0] + 2*a"
                             "oB = C; oB[2] = 0" ]
            }.oB
            scaleFactor = USE BODYCALC.oA                         
        }    
        DEF Back Material { diffuseColor 0 0 0.8 transparency 0.5 }
        IndexedFaceSet {
            vertexProperty VertexProperty {
                vertex  [ 0.5 -0.5 0,
                          -0.5 -0.5 0,
                          -0.5 0.5 0,
                          0.5 0.5  0]
                orderedRGBA [  ]
            }
            coordIndex [ 2, 1, 0, 3, -1 ]
        }
    }
    DEF UPPER Separator {
        Transform {
            translation = DEF UPPERCALC Calculator {
                A = USE BBOX.boxCenter
                B = USE BBOX.max
                C = USE BBOX.min
                a = USE XPADDING.string
                h = USE CORNERRADIUS.string
                expression [ "oA[0] = A[0]; oA[1] = B[1]; oA[2] = 0"
                             "oB[0] = B[0] - C[0] - 2*h + 2*a; oB[1] = 1; oB[2] = 1" ]
            }.oA
            scaleFactor = USE UPPERCALC.oB
        }
        USE Back
        IndexedFaceSet {
            vertexProperty VertexProperty {
                vertex  [ 0.5 0.1 0,
                         -0.5 0.1 0,
                         -0.5 0 0,
                          0.5 0 0 ]
                orderedRGBA [  ]
              }        
              coordIndex    [ 0, 1, 2, 3, -1 ]
        }
    }
    DEF LOWER Separator {
        Transform {
            translation = DEF LOWERCALC Calculator {
                A = USE BBOX.boxCenter
                B = USE BBOX.max
                C = USE BBOX.min
                a = USE XPADDING.string
                h = USE CORNERRADIUS.string
                expression [ "oA[0] = A[0]; oA[1] = C[1]; oA[2] = 0"
                             "oB[0] = B[0] - C[0] - 2*h + 2*a; oB[1] = 1; oB[2] = 1" ]
            }.oA
            scaleFactor = USE LOWERCALC.oB
        }
        USE Back
        IndexedFaceSet {
            vertexProperty VertexProperty {
                vertex  [ -0.5 -0.1 0,
                           0.5 -0.1 0,
                           0.5 0 0,
                          -0.5 0 0 ]
                orderedRGBA [  ]
            }
            coordIndex    [ 0, 1, 2, 3, -1 ]
        }    
    }
    DEF UPPERLEFT Separator {
        Transform {
            translation = Calculator {
                A = USE BBOX.min
                B = USE BBOX.max
                a = USE XPADDING.string
                h = USE CORNERRADIUS.string
                expression [ "oA[0] = A[0] + h - a; oA[1] = B[1]; oA[2] = 0" ]
            }.oA
        }
        USE Back
        IndexedFaceSet {
            vertexProperty VertexProperty {
                   vertex  [ 0 0.1 0,
                      -0.02291 0.09736 0,
                      -0.04396 0.08983 0,
                      -0.06252 0.07802 0,
                      -0.07802 0.06252 0,
                      -0.08983 0.04396 0,
                      -0.09736 0.02291 0,
                      -0.1 0 0,
                      0 0 0 ]
                orderedRGBA [  ]
              }        
              coordIndex    [ 0, 1, 2, 3, 4, 5, 6, 7, 8, -1 ]
        }
    }
    DEF UPPERRIGHT Separator {
        Transform {
            translation = Calculator {
                A = USE BBOX.max
                a = USE XPADDING.string
                h = USE CORNERRADIUS.string                
                expression [ "oA = A; oA[2] = 0; oA[0] = oA[0] -h + a" ]
            }.oA
        }
        USE Back
        IndexedFaceSet {
            vertexProperty VertexProperty {
                vertex  [ 0.1 0 0,
                      0.09736 0.02291 0,
                      0.08983 0.04396 0,
                      0.07802 0.06252 0,
                      0.06252 0.07802 0,
                      0.04396 0.08983 0,
                      0.02291 0.09736 0,
                      0 0.1 0,
                      0 0 0 ]
                orderedRGBA [  ]
              }        
              coordIndex    [ 0, 1, 2, 3, 4, 5, 6, 7, 8, -1 ]
        }
    }
    DEF LOWERRIGHT Separator {
        Transform {
            translation = Calculator {
                A = USE BBOX.min
                B = USE BBOX.max
                a = USE XPADDING.string
                h = USE CORNERRADIUS.string
                expression [ "oA[0] = B[0] - h + a; oA[1] = A[1]; oA[2] = 0" ]
            }.oA
        }
        USE Back
        IndexedFaceSet {
            vertexProperty VertexProperty {
                vertex  [   0 -0.1 0,
                      0.02291 -0.09736 0,
                      0.04396 -0.08983 0,
                      0.06252 -0.07802 0,
                      0.07802 -0.06252 0,
                      0.08983 -0.04396 0,
                      0.09736 -0.02291 0,
                      0.1 0 0,
                      0 0 0 ]
                orderedRGBA [  ]
            }
            coordIndex    [ 0, 1, 2, 3, 4, 5, 6, 7, 8, -1 ]
        }    
    }
    DEF LOWERLEFT Separator {
        Transform {
            translation = Calculator {
                A = USE BBOX.min
                a = USE XPADDING.string
                h = USE CORNERRADIUS.string
                expression [ "oA = A; oA[2] = 0; oA[0] = oA[0] + h - a" ]
            }.oA        
        }
        USE Back
        IndexedFaceSet {
            vertexProperty VertexProperty {
                vertex  [ -0.1 -0 0,
                      -0.09736 -0.02291 0,
                      -0.08983 -0.04396 0,
                      -0.07802 -0.06252 0,
                      -0.06252 -0.07802 0,
                      -0.04396 -0.08983 0,
                      -0.02291 -0.09736 0,
                      0 -0.1 0,
                      0 0 0 ]
                orderedRGBA [  ]
              }
              coordIndex    [ 0, 1, 2, 3, 4, 5, 6, 7, 8, -1 ]
        }
    }
}

        }  # end 2D Content
<xsl:apply-templates mode="rep-content-3D" select="annotation/Content"/>        
    }
</xsl:template>

<!-- template for text content -->
<xsl:template name="rep-text" match="*[@mime-type='text/text']" mode="rep-content-2D" priority="0.1">
            Transform { 
                translation = Calculator {          
                    A = DEF BBOX ComputeBoundingBox { node USE PARENT }.min
                    B = USE BBOX.max
                    expression "oA[0] = 0; oA[1] = A[1]-B[1]; oA[2] = 0"
                }.oA
            }
            DEF PARENT Separator {
                Material { diffuseColor 1 1 1 }
                Font {  size 0.04 name "Arial Fett" }
                AsciiText{ spacing 1.5 justification LEFT string [<xsl:value-of select="."/>] }
            }
</xsl:template>

<!-- template for image content -->
<xsl:template name="rep-image" match="*[@mime-type='image/jpeg']" mode="rep-content-2D" priority="0.1">
            Transform { 
                translation = Calculator {          
                    A = DEF BBOX ComputeBoundingBox { node USE PARENT }.min
                    B = USE BBOX.max
                    expression "oA[0] = 0; oA[1] = A[1]-B[1]; oA[2] = 0"
                }.oA
            }
            DEF PARENT Separator {
                Texture2 { filename "<xsl:value-of select="."/>" model DECAL }
                Transform { 
                    scaleFactor = Calculator {
                        A = USE BBOX.min
                        B = USE BBOX.max
                        expression "oA[0] = B[0] - A[0]; oA[1] = oA[0]; oA[2] = 1"
                    }.oA
                }                
                Coordinate3 { point [0 -1 0, 1 -1 0, 1 0 0, 0 0 0] }
                TextureCoordinateBinding { value PER_VERTEX }
                TextureCoordinate2 { point [ 0 0, 0 1, 1 1, 1 0 ] }
                IndexedFaceSet { coordIndex [ 1, 2, 3, 0, -1 ] }
            }
</xsl:template>

<!-- template for inventor -->
<xsl:template name="rep-inventor" match="*[@mime-type='application/inventor']" mode="rep-content-3D" priority="0">
  <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="text()" mode="rep-content-3D"/>
<xsl:template match="text()" mode="rep-content-2D"/>

<!--=======================================================================-->
<!-- indexmap section
     creates a SoStringMap mapping annotation ids to indices in the geometry
 -->
<!--=======================================================================-->
  <xsl:template name="indexmap">
    <xsl:document href="{$indexmap}" mode="text">
      <!-- compute a list of all matching annotation elements -->
      <xsl:variable name="matching">
        <xsl:for-each select=".//Annotation">
          <xsl:variable name="currentkeywords">
            <xsl:for-each select="./ancestor-or-self::node()">
              <xsl:value-of select="annotation/Keywords"/>
              <xsl:text> </xsl:text>
            </xsl:for-each>
          </xsl:variable>
          <xsl:variable name="testResult">
            <xsl:call-template name="testKeywords">
              <xsl:with-param name="given" select="$currentkeywords"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="$testResult = 1">"<xsl:value-of select="@id"/>",</xsl:if>
        </xsl:for-each>
      </xsl:variable>
      <!-- compute length of the list -->
      <xsl:variable name="count" select="string-length(translate($matching,', _abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.&quot;',','))"/>
      <xsl:text>      a [</xsl:text>
      <xsl:value-of select="substring($matching,1,string-length($matching)-1)"/>]
      b [<xsl:call-template name="quoteList">
        <xsl:with-param name="list">
          <xsl:call-template name="generateNumbers">
            <xsl:with-param name="len" select="$count"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>]
  </xsl:document>
  </xsl:template>

<!--=======================================================================-->
<!-- keywordmap section
     creates a SoStringMap mapping keywords to annotation ids and also a list
     of strings containing the available keywords.
 -->
<!--=======================================================================-->
  <xsl:template name="keywordmap">
    <xsl:document href="{$keywordmap}" mode="text">
      <!-- compute a list of keywords and a list of matching ids -->
      <xsl:variable name="words" saxon:assignable="yes"/>
      <xsl:variable name="ids" saxon:assignable="yes"/>
      <xsl:for-each select=".//Annotation">
        <xsl:variable name="currentkeywords">
          <xsl:for-each select="./ancestor-or-self::node()">
            <xsl:value-of select="annotation/Keywords"/>
            <xsl:text> </xsl:text>
            <xsl:if test="local-name() = 'Building'">
              <xsl:value-of select="@id"/>
              <xsl:text> </xsl:text>
            </xsl:if>
          </xsl:for-each>
        </xsl:variable>
        <xsl:variable name="testResult">
          <xsl:call-template name="testKeywords">
            <xsl:with-param name="given" select="$currentkeywords"/>
          </xsl:call-template>
        </xsl:variable>
        <!-- if it matches add all the (keyword,id) pairs to the lists -->
        <xsl:if test="$testResult = 1">
          <saxon:assign name="words">
            <xsl:if test="$words">
              <xsl:value-of select="$words"/>,
          </xsl:if>
            <xsl:call-template name="quoteList">
              <xsl:with-param name="list" select="$currentkeywords"/>
            </xsl:call-template>
          </saxon:assign>
          <xsl:variable name="len" select="string-length(translate(normalize-space($currentkeywords),' ,_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.&quot;','.'))+1"/>
          <saxon:assign name="ids">
            <xsl:if test="$ids">
              <xsl:value-of select="$ids"/>,
          </xsl:if>
            <xsl:call-template name="quoteList">
              <xsl:with-param name="list">
                <xsl:call-template name="generateListRepeat">
                  <xsl:with-param name="item" select="@id"/>
                  <xsl:with-param name="len" select="$len"/>
                </xsl:call-template>
              </xsl:with-param>
            </xsl:call-template>
          </saxon:assign>
        </xsl:if>
      </xsl:for-each>
      <xsl:text>    a [ </xsl:text>
      <xsl:value-of select="$words"/> ]
    b [ <xsl:value-of select="$ids"/> ]
  </xsl:document>
  </xsl:template>

<!--=======================================================================-->
<!-- keywordlist section
     creates a list of strings containing the available keywords.
 -->
<!--=======================================================================-->
  <xsl:template name="keywordlist">
    <xsl:document href="{$keywordlist}" mode="text">
      <!-- compute a list of keywords and a list of matching ids -->
      <xsl:variable name="allkeywords">
        <xsl:for-each select=".//Annotation">
          <xsl:variable name="currentkeywords">
            <xsl:for-each select="./ancestor-or-self::node()">
              <xsl:value-of select="annotation/Keywords"/>
              <xsl:text> </xsl:text>
            </xsl:for-each>
          </xsl:variable>
          <xsl:variable name="testResult">
            <xsl:call-template name="testKeywords">
              <xsl:with-param name="given" select="$currentkeywords"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="$testResult = 1">
            <xsl:value-of select="$currentkeywords"/>
            <xsl:text> </xsl:text>
          </xsl:if>
        </xsl:for-each>
      </xsl:variable>
      <xsl:text>     [ </xsl:text>
      <xsl:call-template name="quoteList">
        <xsl:with-param name="list">
          <xsl:call-template name="makeSet">
            <xsl:with-param name="list" select="normalize-space($allkeywords)"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template> ]
  </xsl:document>
  </xsl:template>

<!--=======================================================================-->
<!-- helper templates
     various tools and helper templates ... -->
<!--=======================================================================-->
  <xsl:template name="update_translation">
    <xsl:param name="pathtrans" select="'0 0 0'"/>
    <xsl:param name="pathrot" select="'0 0 0 1'"/>
    <xsl:param name="localtrans" select="'0 0 0'"/>
    <!-- todo implementation here ! -->
  </xsl:template>
  <xsl:template name="update_rotation">
    <xsl:param name="pathtrans" select="'0 0 0'"/>
    <xsl:param name="pathrot" select="'0 0 0 1'"/>
    <xsl:param name="localrot" select="'0 0 0'"/>
    <xsl:param name="rottype"/>
    <!-- todo implementation here ! -->
  </xsl:template>
</xsl:stylesheet>
