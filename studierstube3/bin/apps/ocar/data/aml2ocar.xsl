<?xml version="1.0" encoding="UTF-8"?>
<!--=======================================================================-->
<!-- This transformation file converts the general bauml data to 
     datastructures required for the ocar applications. It outputs up to five
     different files :
     
     modelfile contains the 3D model for buildings in the area
     modelindex contains an MFString field in the same order as the buildings 
         in the 3D model 
     navpoints file contains the 3D representation of waypoints 
     navedges file contains the 3D representation of edges in between waypoints
     navindex contains MF fields for indexing the model files and to do wayfinding
         on the graph 
     navlist contains a MF field with interesting navigation targets to be 
         displayed in the user interface. Currently it looks for an annotation
         being present to filter the waypoints.
     navkeys contains a MF field with the list of found keywords
     navmap contains the mapping engines necessary to map keywords to
         destinations
         
     a special parameter target allows to output pure openinventor files for
     testing purposes. if the target = 'stb' it will output the Studierstube specific
     format otherwise pure openinventor.
     
     @author Gerhard Reitmayr
     $Id: aml2ocar.xsl 3427 2004-07-14 12:20:49Z tamer $
-->
<!--=======================================================================-->
<xsl:stylesheet version="1.1" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" xmlns:saxon="http://icl.com/saxon" extension-element-prefixes="saxon">
  <!-- include some tools for list and set manipulation here -->
  <xsl:include href="tools.xslt"/>
  <xsl:include href="../../signpost/data/bauml2iv.xslt"/>
  <!-- setting output method -->
  <xsl:output method="text" encoding="UTF-8"/>
  <!--=======================================================================-->
  <!-- parameter section
     this parameters can be set from the command line of your XSLT processor 
     or here, if you like. They influence various aspects of the stylesheet
     such as output files. -->
  <!--=======================================================================-->
  <xsl:param name="model"/>
  <xsl:param name="modelindex"/>
  <xsl:param name="navpoints"/>
  <xsl:param name="navedges"/>
  <xsl:param name="navindex"/>
  <xsl:param name="navlist"/>
  <xsl:param name="navkeys"/>
  <xsl:param name="navmap"/>
  <xsl:param name="target">stb</xsl:param>
  <!-- here we decide which files need to be generated and process the input
     file in different modes accordingly -->
  <xsl:template match="/">
    <xsl:if test="$model">
      <xsl:call-template name="modelfile"/>
    </xsl:if>
    <xsl:if test="$modelindex">
      <xsl:call-template name="modelindex"/>
    </xsl:if>
    <xsl:if test="$navpoints">
      <xsl:call-template name="navpoints"/>
    </xsl:if>
    <xsl:if test="$navedges">
      <xsl:call-template name="navedges"/>
    </xsl:if>
    <xsl:if test="$navindex">
      <xsl:call-template name="navindex"/>
    </xsl:if>
    <xsl:if test="$navlist">
      <xsl:call-template name="navlist"/>
    </xsl:if>
    <xsl:if test="$navkeys">
      <xsl:call-template name="navkeys"/>
    </xsl:if>
    <xsl:if test="$navmap">
      <xsl:call-template name="navmap"/>
    </xsl:if>
  </xsl:template>
  <!--=======================================================================-->
  <!-- modelfile section
     These templates generate a file containing the geometrical model of
     buildings in the aml file. It assumes that the first element is a 
     container containing a list of Buildings. -->
  <!--=======================================================================-->
  <xsl:template name="modelfile">
generating modelfile <xsl:value-of select="$model"/> ...
<xsl:document href="{$model}" mode="text">#Inventor V2.1 ascii
# Model file derived from AML source file by aml2ocar.xsl

<xsl:call-template name="switch"/>
      <xsl:apply-templates select="./*/children/*" mode="modelfile"/>
}
</xsl:document>
  </xsl:template>
  
  <xsl:template match="Building|SpatialObject" mode="modelfile">
    <xsl:if test="@id">    DEF <xsl:value-of select="@id"/>
    </xsl:if> Separator {
    <xsl:call-template name="GenerateTransformation"/>
      ShapeHints { vertexOrdering COUNTERCLOCKWISE shapeType SOLID }
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
      }
    }
</xsl:template>
<!-- empty default template for text nodes -->
<xsl:template match="text()" mode="modelfile" />
  <!--=======================================================================-->
  <!-- modelindex section
     These templates generate an MFString field containing the ids of the
     Buildings in the same order as they appear in the modelfile. This is
     very useful for SoListBox widgets and applications -->
  <!--=======================================================================-->
  <xsl:template name="modelindex">
generating model index file <xsl:value-of select="$modelindex"/> ...
<xsl:document href="{$modelindex}" mode="text">#Inventor V2.1 ascii
# Model index data derived from AML source file by aml2ocar.xsl

values [
  <xsl:apply-templates select="./*/children/*" mode="modelindex"/>
]
</xsl:document>
  </xsl:template>
  <xsl:template match="Building|SpatialObject" mode="modelindex">
    <xsl:if test="@id">"<xsl:value-of select="@id"/>"<xsl:if test="not(position()=last())">,</xsl:if>
    </xsl:if>
  </xsl:template>
  <!-- empty default template for text nodes -->
<xsl:template match="text()" mode="modelindex" />
  <!--=======================================================================-->
  <!-- navpoints section
     These templates generate an Inventor file containing the geometry for
     the navigation application for ocar. -->
  <!--=======================================================================-->
  <xsl:template name="navpoints">
generating navigation waypoint file <xsl:value-of select="$navpoints"/> ...
<xsl:document href="{$navpoints}" mode="text">#Inventor V2.1 ascii
# Navigation geometry of waypoints for the ocar navigation app generated from AML source file by aml2ocar.xsl
<xsl:call-template name="switch"/>
      <xsl:for-each select="./*/children/Waypoint">
  DEF <xsl:value-of select="@id"/> Separator {
    <xsl:call-template name="transform"/>
        <xsl:choose>
          <xsl:when test="position() = 1">    DEF Waypoint_Geometry Group { 
          Transform { translation 0 2 0 }
          Material { diffuseColor 0.9 0.2 0.2 }
          Cylinder { radius 0.5 height 4 }
          Transform { translation 0 2.1 0 }
          Material { diffuseColor 1 1 1 }
          Font { name "Arial Fett" size 14 }          
      }</xsl:when>
          <xsl:otherwise>    USE Waypoint_Geometry</xsl:otherwise>
        </xsl:choose>
    Text2 { justification CENTER string "<xsl:value-of select="@id"/>" }
  }
  </xsl:for-each>
}
</xsl:document>
  </xsl:template>
  <!--=======================================================================-->
  <!-- navedges section
     These templates generate an Inventor file containing the geometry for
     the navigation application for ocar. -->
  <!--=======================================================================-->
  <xsl:template name="navedges">
generating navigation edges file <xsl:value-of select="$navedges"/> ...
<xsl:document href="{$navedges}" mode="text">#Inventor V2.1 ascii

# Edges with strokes or lines
<xsl:call-template name="switch"/>
      <xsl:for-each select="./*/children/Waypoint">
        <xsl:variable name="neighbors" select="@neighbors"/>
        <xsl:variable name="tail-id" select="@id"/>
        <xsl:variable name="start-pos" select="pose/Transformation/@translation"/>
        <xsl:variable name="pos" select="position()"/>
        <xsl:for-each select="//Waypoint">
          <xsl:variable name="test">
            <xsl:call-template name="testNeighbor">
              <xsl:with-param name="neighbors" select="$neighbors"/>
              <xsl:with-param name="id" select="@id"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="$test = 1">DEF <xsl:value-of select="$tail-id"/>_<xsl:value-of select="@id"/> Separator {
    <xsl:choose>
              <xsl:when test="$target='stb'">  SoStroke {
        point [ <xsl:value-of select="$start-pos"/>, <xsl:value-of select="pose/Transformation/@translation"/> ]
        scale DIRECTION
        geometry <xsl:choose>
                  <xsl:when test="$pos=1">DEF Edge_Geometry Group {
            Transform { rotation 0 0 1 -1.57 }
            Material { diffuseColor 1 0.5 0 emissiveColor 1 0.5 0 }
            Cylinder { height 1 radius 0.1 }
            Transform { translation 0 0.5 0 }    
            Cone { bottomRadius 0.4 height 0.111111 }
        }
          </xsl:when>
                  <xsl:otherwise> USE Edge_Geometry
          </xsl:otherwise>
                </xsl:choose>
      }</xsl:when>
              <xsl:otherwise>  Translation { translation 0 1 0 }
      DrawStyle { lineWidth 2 }
      IndexedLineSet {
        vertexProperty VertexProperty { vertex [ <xsl:value-of select="$start-pos"/>, <xsl:value-of select="pose/Transformation/@translation"/> ] }
        coordIndex [ 0, 1, -1 ]
      }</xsl:otherwise>
            </xsl:choose>              
    }
    </xsl:if>
        </xsl:for-each>
      </xsl:for-each>
}
</xsl:document>
  </xsl:template>
  <!--=======================================================================-->
  <!-- navindex section
     These templates generate the index data for the navigation application.
     This includes the waypoint graph as well as again an index field. -->
  <!--=======================================================================-->
  <xsl:template name="navindex">
generating navigation file <xsl:value-of select="$navindex"/> ...
<xsl:document href="{$navindex}" mode="text">#Inventor V2.1 ascii
# Navigation information for the ocar navigation app generated from AML source file by aml2ocar.xsl

# index values for SoListBox
waypoint [
  <xsl:apply-templates select="./*/children/Waypoint" mode="navindex-index"/>
]

# positions of individual Waypoints
waypointPosition [
  <xsl:for-each select="./*/children/Waypoint">
        <xsl:value-of select="pose/Transformation/@translation"/>
        <xsl:if test="not(position()=last())">,
</xsl:if>
      </xsl:for-each>
]

<xsl:variable name="numEdgeList" saxon:assignable="yes"/>
      <xsl:variable name="count" saxon:assignable="yes" select="0"/>

# graph data for navigation application
edge [
<xsl:for-each select="./*/children/Waypoint">
        <xsl:variable name="neighbors" select="@neighbors"/>
        <saxon:assign name="count" select="0"/>
        <xsl:for-each select="//Waypoint">
          <xsl:variable name="test">
            <xsl:call-template name="testNeighbor">
              <xsl:with-param name="neighbors" select="$neighbors"/>
              <xsl:with-param name="id" select="@id"/>
            </xsl:call-template>
          </xsl:variable>
          <xsl:if test="$test = 1">
            <xsl:number count="Waypoint" value="position()-1" from="/"/>
            <xsl:text>,</xsl:text>
            <saxon:assign name="count" select="$count + 1"/>
          </xsl:if>
        </xsl:for-each>
        <xsl:text>
  </xsl:text>
        <saxon:assign name="numEdgeList" select="concat($numEdgeList, ',', $count)"/>
      </xsl:for-each>
]

numEdge [  
  <xsl:value-of select="substring($numEdgeList,2)"/>
]  

</xsl:document>
  </xsl:template>
  <xsl:template match="Waypoint" mode="navindex-index">
    <xsl:if test="@id">"<xsl:value-of select="@id"/>"<xsl:if test="not(position()=last())">,</xsl:if>
      <xsl:if test="position() mod 5 = 0">
        <xsl:text>
</xsl:text>
      </xsl:if>
    </xsl:if>
  </xsl:template>
  <!--=======================================================================-->
  <!-- navlist section
     These templates generate the a selected index of possible navigation targets -->
  <!--=======================================================================-->
  <xsl:template name="navlist">
generating navigation target file <xsl:value-of select="$navlist"/> ...
<xsl:document href="{$navlist}" mode="text">#Inventor V2.1 ascii
# Navigation target list values for SoListBox
values [
<xsl:for-each select="./*/children/Waypoint">
        <xsl:if test="annotation">
          <xsl:if test="@id">"<xsl:value-of select="@id"/>",
</xsl:if>
        </xsl:if>
      </xsl:for-each>]
</xsl:document>
  </xsl:template>
  <!--=======================================================================-->
  <!-- navkeys section
     this template generates the keyword list for selecting a possible
-->
  <!--=======================================================================-->
  <xsl:template name="navkeys">
generating navigation keyword list file <xsl:value-of select="$navkeys"/> ...
<xsl:document href="{$navkeys}" mode="text">#Inventor V2.1 ascii
# Navigation information for the ocar navigation app generated from AML source file by aml2ocar.xsl
values [
  <xsl:call-template name="quoteList">
        <xsl:with-param name="itemsPerLine" select="'5'"/>
        <xsl:with-param name="list">
          <xsl:call-template name="makeSet">
            <xsl:with-param name="list">
              <xsl:for-each select="//Waypoint">
                <xsl:if test="annotation/WaypointTitle">
                  <xsl:call-template name="waypointKeywords"/>
                </xsl:if>
              </xsl:for-each>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
]
</xsl:document>
  </xsl:template>
  <!-- gets all navigation relevant keywords for a given waypoint node.
     needs to merge the keywords from the waypoint itself and from all spatialobjects
     that reference the waypoint node with Addresspoint -->
  <xsl:template name="waypointKeywords">
    <xsl:variable name="localKeywords" select="annotation/Keywords"/>
    <xsl:variable name="objectKeywords">
      <xsl:for-each select="//*[@baseType='SpatialObjectType' or @baseType='SpatialContainerType'][annotation/Addresspoint/@ref=@id]">
        <xsl:value-of select="annotation/Keywords"/>
        <xsl:text> </xsl:text>
      </xsl:for-each>
    </xsl:variable>
    <xsl:call-template name="union">
      <xsl:with-param name="a" select="$localKeywords"/>
      <xsl:with-param name="b" select="$objectKeywords"/>
    </xsl:call-template>
  </xsl:template>
  <!--=======================================================================-->
  <!-- navmap section
     this template generates the keyword list for selecting a possible
     Right now it breaks the mf string fields after five entries. It would
     be possible to use the line breaks to group keywords, but this would
     obfuscate the data processing somewhat. right now its easier to 
     understand.
-->
  <!--=======================================================================-->
  <xsl:template name="navmap">
generating navigation keyword map file <xsl:value-of select="$navmap"/> ...
<xsl:document href="{$navmap}" mode="text">
# Navigation information for the ocar navigation app generated from AML source file by aml2ocar.xsl
SoFanIn { type SoMFString
  in0 = Gate { type SoMFString 
    enable = BoolOperation { operation NOT_A
      a = USE showDest.on
    }.output
    input = SoStringMap {  # maps keywords to waypoint ids
      mode INTERSECTION    # other modes are MAPPING | UNION | 
    a [        # contains keywords
    <xsl:call-template name="quoteList">
        <xsl:with-param name="itemsPerLine" select="'5'"/>
        <xsl:with-param name="list">
          <xsl:for-each select="//Waypoint">
            <xsl:if test="annotation/WaypointTitle">
              <xsl:call-template name="waypointKeywords"/>
            </xsl:if>
          </xsl:for-each>
        </xsl:with-param>
      </xsl:call-template>
    ]
    b [        # contains waypoint ids
    <xsl:call-template name="quoteList">
        <xsl:with-param name="itemsPerLine" select="'5'"/>
        <xsl:with-param name="list">
          <xsl:for-each select="//Waypoint">
            <xsl:if test="annotation/WaypointTitle">
              <xsl:call-template name="generateListRepeat">
                <xsl:with-param name="len">
                  <xsl:call-template name="length">
                    <xsl:with-param name="list">
                      <xsl:call-template name="waypointKeywords"/>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:with-param>
              </xsl:call-template>
              <xsl:text> </xsl:text>
            </xsl:if>
          </xsl:for-each>
        </xsl:with-param>
      </xsl:call-template>
    ]
    aIn = USE keywordList.selected
  }.bOut
  }.output
  in1 = Gate { type SoMFString 
      enable = USE showDest.on
    input [ <xsl:for-each select="./*/children/Waypoint">
        <xsl:if test="annotation">
          <xsl:if test="@id">"<xsl:value-of select="@id"/>",
              </xsl:if>
        </xsl:if>
      </xsl:for-each> ]
  }.output
}.out
</xsl:document>
  </xsl:template>
  <!--=======================================================================-->
  <!-- helper templates
     various tools and helper templates ... -->
  <!--=======================================================================-->
  <!-- constructs a transformation node from a given pose/Transformation subtree -->
  <xsl:template name="transform">Transform { 
      <xsl:if test="pose/Transformation/@translation">translation <xsl:value-of select="pose/Transformation/@translation"/>
    </xsl:if>
    <xsl:if test="pose/Transformation/@rotation">rotation    <xsl:value-of select="pose/Transformation/@rotation"/>
    </xsl:if>
    }
</xsl:template>
  <!-- constructs the beginning of the encapsulating switch node, depending on the output target -->
  <xsl:template name="switch">
    <xsl:choose>
      <xsl:when test="$target='stb'">SoMultiSwitch {
  whichChildren -2
  </xsl:when>
      <xsl:otherwise>Switch {
  whichChild -3
  </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- tests if a given id is in the list of neighbors (a space separated list of strings) -->
  <xsl:template name="testNeighbor">
    <xsl:param name="neighbors"/>
    <xsl:param name="id"/>
    <xsl:choose>
      <xsl:when test="$neighbors = $id">1</xsl:when>
      <xsl:when test="string-length($neighbors) > string-length($id)">
        <xsl:choose>
          <xsl:when test="starts-with($neighbors, concat($id,' '))">1</xsl:when>
          <xsl:when test="contains($neighbors,concat(' ', $id, ' '))">1</xsl:when>
          <xsl:when test="substring($neighbors, string-length($neighbors)-string-length($id)+1) = $id">1</xsl:when>
          <xsl:otherwise>0</xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>
