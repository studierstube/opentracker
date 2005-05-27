<?xml version="1.0" encoding="UTF-8"?>
<?xmlspysamplexml E:\studierstube\bin\apps\ocar\data\tuwien-map.gml?>
<!-- ===================================================================== -->
<!-- This file extracts interesting data from a GML2 file and converts it to aml.
     some current features :
   
     coordinates are converted in the following manner :
     easting, northing -> easting 0 northing
     this is a left handed coordinate system !
     
     There are two modes currently :
     * route extracts information stored in ROUTESEGs. It will apply an offset for the center point as well.
     * address extracts ADDRESS_point information into Waypoints. It does NOT apply any further transformation !
-->
<!-- ===================================================================== -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:saxon="http://icl.com/saxon" extension-element-prefixes="saxon" xmlns:gml2="http://www.safe.com/gml2" xmlns:gml="http://www.opengis.net/gml">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
  
  <!-- this parameter determines which parts of the GML file will be processed -->
  <xsl:param name="mode"/>
  
  <!-- this variables store the center. all data will be refitted to match the center -->
  <xsl:variable name="centerx" saxon:assignable="yes" select="'0'"/>
  <xsl:variable name="centerz" saxon:assignable="yes" select="'0'"/>
  <!-- this variable stores a output fragment for any additional navigation points derived from route points -->
  <xsl:variable name="newpoints" saxon:assignable="yes"/>
  
<!-- ===================================================================== -->
<!-- top level template decides which kinds of processing are done         -->
<!-- ===================================================================== -->         
  <xsl:template match="/">
    <!-- create a SpatialContainer with all Waypoints in there -->
    <xsl:element name="SpatialContainer">
      <xsl:attribute name="id">Whatever</xsl:attribute>
      <xsl:element name="children">
      <!-- test for route mode -->
      <xsl:if test="contains($mode, 'route')">
        <xsl:call-template name="route"/>
      </xsl:if>
      <!-- test for address point mode -->
      <xsl:if test="contains($mode,'address')">
        <xsl:call-template name="address"/>
      </xsl:if>
      </xsl:element>
    </xsl:element>  
  </xsl:template>
  
<!-- ===================================================================== -->
<!-- route mode extracts all navigation network information and constructs
     Waypoints and associated neighbor definitions from that               -->
<!-- ===================================================================== -->       
  <xsl:template name="route">
    <xsl:comment>Waypoint navigation network extracted from ROUTESEG information</xsl:comment>
        
    <!--calculate center point of all waypoints-->
    <xsl:variable name="currentNode" saxon:assignable="yes"/>
    <xsl:variable name="count" saxon:assignable="yes" select="'0'"/>
    <xsl:for-each select=".//gml2:FRNODE|.//gml2:TONODE">
      <xsl:sort select="."/>
      <xsl:choose>
        <!-- start a new node -->
        <xsl:when test="not($currentNode=string(.))">
          <!-- reassign variables -->
          <saxon:assign name="currentNode" select="string(.)"/>
          <!-- position needs to distinguish between from and to nodes to use the correct coordinates -->
          <xsl:variable name="position">
            <xsl:choose>
              <xsl:when test="local-name()='FRNODE'">
                <xsl:value-of select="substring-before(following::gml:lineStringProperty/gml:LineString/gml:coordinates,',')"/>
                <xsl:text> </xsl:text>
                <xsl:value-of select="substring-before(substring-after(following::gml:lineStringProperty/gml:LineString/gml:coordinates,','),' ')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="getlastcoord2d">
                  <xsl:with-param name="positionstring" select="string(following::gml:lineStringProperty/gml:LineString/gml:coordinates)"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <!-- update center -->
          <saxon:assign name="centerx" select="number($centerx) + number(substring-before($position, ' '))"/>
          <saxon:assign name="centerz" select="number($centerz) + number(substring-after($position, ' '))"/>
          <saxon:assign name="count" select="$count+1"/>
        </xsl:when>
      </xsl:choose>
    </xsl:for-each>
    <!-- divide the sum of positions by the count to get the center -->
    <saxon:assign name="centerx" select="$centerx div $count"/>
    <saxon:assign name="centerz" select="$centerz div $count"/>
    <xsl:comment>center point is <xsl:value-of select="$centerx"/>, <xsl:value-of select="$centerz"/></xsl:comment>
    
    <!-- build Waypoints for each unique node -->
    <!-- currently we are not dealing with intermediary nodes ... -->
    <saxon:assign name="currentNode"/>
    <xsl:variable name="toList" saxon:assignable="yes"/>
    <xsl:variable name="position" saxon:assignable="yes"/>
    <xsl:variable name="start" saxon:assignable="yes" select="true"/>
    <xsl:for-each select=".//gml2:FRNODE|.//gml2:TONODE">
      <xsl:sort select="."/>
      <xsl:choose>
        <!-- start a new node -->
        <xsl:when test="not($currentNode=string(.))">
          <!-- if not the first, then write out the last node -->
          <xsl:if test="$start='false'">
            <xsl:element name="Waypoint">
              <xsl:attribute name="id"><xsl:value-of select="concat('Point_',$currentNode,'_')"/></xsl:attribute>
              <xsl:attribute name="neighbors"><xsl:value-of select="substring($toList,2)"/></xsl:attribute>
              <xsl:element name="pose">
                <xsl:element name="Transformation">
                  <xsl:attribute name="translation"><xsl:value-of select="$position"/></xsl:attribute>
                </xsl:element>
              </xsl:element>
            </xsl:element>
          </xsl:if>
          <!-- reassign variables -->
          <saxon:assign name="start" select="'false'"/>
          <saxon:assign name="currentNode" select="string(.)"/>
          <saxon:assign name="toList"/>
          <!-- position needs to distinguish between from and to nodes to use the correct coordinates -->
          <saxon:assign name="position">
            <xsl:choose>
              <xsl:when test="local-name()='FRNODE'">
                <xsl:value-of select="number(substring-before    (following::gml:lineStringProperty/gml:LineString/gml:coordinates,',')) - $centerx"/>
                <xsl:text> </xsl:text>0<xsl:text> </xsl:text>
                <xsl:value-of select="number(substring-before(substring-after    (following::gml:lineStringProperty/gml:LineString/gml:coordinates,','),' ')) - $centerz"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="getlastcoord">
                  <xsl:with-param name="positionstring" select="string    (following::gml:lineStringProperty/gml:LineString/gml:coordinates)"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>
          </saxon:assign>
          <!-- collect neigbors -->
          <xsl:for-each select="//gml2:ROUTESEG_line">
            <xsl:variable name="len">
              <xsl:call-template name="getPointNum">
                <xsl:with-param name="positionstring" select="gml:lineStringProperty/gml:LineString/gml:coordinates"/>
              </xsl:call-template>
            </xsl:variable>                
            <xsl:choose>
              <!-- test for current node being the from node -->
              <xsl:when test="string(gml2:FRNODE)=$currentNode">
                <xsl:choose>
                  <xsl:when test="$len = 2">
                    <xsl:if test="not(contains($toList,concat('Point_',string(gml:TONODE),'_')))">
                      <saxon:assign name="toList" select="concat($toList,' ','Point_',string(gml2:TONODE),'_')"/>
                    </xsl:if>
                  </xsl:when>
                  <xsl:otherwise>
                    <saxon:assign name="toList">
                      <xsl:value-of select="$toList"/><xsl:text> </xsl:text>
                      <xsl:call-template name="buildFromNeighbors"/>
                    </saxon:assign>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>
              <!-- or test for current node being the to node -->
              <xsl:when test="string(gml2:TONODE)=$currentNode">
                <xsl:choose>
                  <xsl:when test="$len = 2">
                    <xsl:if test="not(contains($toList,concat('Point_',string(gml2:FRNODE),'_')))">
                      <saxon:assign name="toList" select="concat($toList,' ','Point_',string(gml2:FRNODE),'_')"/>
                    </xsl:if>
                  </xsl:when>
                  <xsl:otherwise>  
                    <saxon:assign name="toList" select="concat($toList,' ','Point_',string(gml2:FRNODE),'T',string    (gml2:TONODE),'M',string($len - 2),'_')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>
            </xsl:choose>
          </xsl:for-each>
          <!-- otherwise, we processed that node already ! -->
        </xsl:when>
      </xsl:choose>
    </xsl:for-each>
    <!-- finally print out the last Waypoint as well -->
    <xsl:element name="Waypoint">
      <xsl:attribute name="id"><xsl:value-of select="concat('Point_',$currentNode,'_')"/></xsl:attribute>
      <xsl:attribute name="neighbors"><xsl:value-of select="substring($toList,2)"/></xsl:attribute>
      <xsl:element name="pose">
        <xsl:element name="Transformation">
          <xsl:attribute name="translation"><xsl:value-of select="$position"/></xsl:attribute>
        </xsl:element>
      </xsl:element>
    </xsl:element>
    <!-- print out the collected new points at the end -->
    <xsl:copy-of select="$newpoints"/>
  </xsl:template>

<!-- ===================================================================== -->
<!-- address mode extracts all address points                              -->
<!-- ===================================================================== -->       
  <xsl:template name="address">
    <xsl:comment>Waypoint navigation network extracted from ROUTESEG information</xsl:comment>
    <xsl:for-each select=".//gml2:ADDRESS_point">
      <xsl:element name="Waypoint">
        <xsl:variable name="idstring">
          <xsl:value-of select="gml2:STREETNAME"/>
          <xsl:text> </xsl:text>
          <xsl:value-of select="gml2:HNUMBER"/>
        </xsl:variable>
        <xsl:variable name="temp"><xsl:value-of select="translate(normalize-space($idstring),' -äöüß','__aous')"/></xsl:variable>
        <xsl:attribute name="id">
          <xsl:choose>
            <xsl:when test="starts-with($temp,'0') or starts-with($temp,'1') or starts-with($temp,'2') or starts-with($temp,'3') or starts-with($temp,'4') or starts-with($temp,'5') or starts-with($temp,'6') or starts-with($temp,'7') or starts-with($temp,'8') or starts-with($temp,'9')"><xsl:value-of select="concat('_',$temp)"/></xsl:when>
            <xsl:otherwise><xsl:value-of select="$temp"/></xsl:otherwise>
          </xsl:choose>
          <xsl:text>_</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="neighbors"/>
        <!-- add a waypoint title to the annotation to mark this as a visible waypoint -->
        <xsl:element name="annotation">
          <xsl:element name="WaypointTitle"><xsl:value-of select="$idstring"/></xsl:element>
        </xsl:element>
        <xsl:element name="pose">
          <xsl:element name="Transformation">
            <xsl:attribute name="translation"><xsl:value-of select="gml2:EASTING"/> 0 <xsl:value-of select="gml2:NORTHING"/></xsl:attribute>
          </xsl:element>
        </xsl:element>
      </xsl:element>
    </xsl:for-each>
  </xsl:template>
  
<!-- ===================================================================== -->
<!-- various helper templates typically called by name                     -->
<!-- ===================================================================== --> 
  <!-- get last position in 3D with y = 0 -->
  <xsl:template name="getlastcoord">
    <xsl:param name="positionstring"/>
    <xsl:choose>
      <xsl:when test="contains($positionstring,' ')">
        <xsl:call-template name="getlastcoord">
          <xsl:with-param name="positionstring" select="substring-after($positionstring,' ')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="number(substring-before($positionstring,',')) - $centerx"/>
        <xsl:text> </xsl:text>0<xsl:text> </xsl:text>
        <xsl:value-of select="number(substring-after($positionstring,',')) - $centerz"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- get last position in 2D -->
  <xsl:template name="getlastcoord2d">
    <xsl:param name="positionstring"/>
    <xsl:choose>
      <xsl:when test="contains($positionstring,' ')">
        <xsl:call-template name="getlastcoord2d">
          <xsl:with-param name="positionstring" select="substring-after($positionstring,' ')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="substring-before($positionstring,',')"/>
        <xsl:text> </xsl:text>
        <xsl:value-of select="substring-after($positionstring,',')"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- count number of points in a positionstring -->
  <xsl:template name="getPointNum">
    <xsl:param name="positionstring"/>
    <xsl:param name="count" select="'1'"/>
    <xsl:choose>
      <xsl:when test="contains($positionstring,' ')">
        <xsl:call-template name="getPointNum">
          <xsl:with-param name="count" select="$count + 1"/>
          <xsl:with-param name="positionstring" select="substring-after($positionstring,' ')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$count"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- build a list of waypoints for intermediary nodes and return the correct neighbor for the from node -->
  <xsl:template name="buildFromNeighbors">
    <!-- compute number of points -->
    <xsl:variable name="len">
      <xsl:call-template name="getPointNum">
        <xsl:with-param name="positionstring" select="gml:lineStringProperty/gml:LineString/gml:coordinates"/>
      </xsl:call-template>
    </xsl:variable>
    <!-- if len == 2 simple case with only a to node neighbor -->
    <xsl:choose>
      <xsl:when test="$len = 2">        
        <xsl:value-of select="concat('Point_',string(gml2:TONODE),'_')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="nextPoint">
          <xsl:with-param name="baseName" select="concat('Point_',string(gml2:FRNODE),'T',string(gml2:TONODE),'M')"/>
          <xsl:with-param name="from" select="concat('Point_',string(gml2:FRNODE),'_')"/>
          <xsl:with-param name="to" select="concat('Point_',string(gml2:TONODE),'_')"/>
          <xsl:with-param name="positionstring" select="substring-after(gml:lineStringProperty/gml:LineString/gml:coordinates,' ')"/>
          <xsl:with-param name="count" select="'1'"/>
          <xsl:with-param name="total" select="$len"/>
        </xsl:call-template>
        <xsl:value-of select="concat('Point_',string(gml2:FRNODE),'T',string(gml2:TONODE),'M1_')"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- build intermediary points -->
  <xsl:template name="nextPoint">
    <xsl:param name="baseName"/>
    <xsl:param name="from"/>
    <xsl:param name="to"/>
    <xsl:param name="positionstring"/>
    <xsl:param name="count"/>
    <xsl:param name="total"/>
    <xsl:if test="$count != ($total - 1)">
      <!-- get coordinates -->
      <xsl:variable name="x" select="number(substring-before($positionstring,',')) - $centerx"/>
      <xsl:variable name="z" select="number(substring-after(substring-before($positionstring,' '),',')) - $centerz"/>
      <!-- check out neighbors -->
      <xsl:variable name="neighbors">
        <xsl:choose>
          <xsl:when test="$count = 1">
            <xsl:value-of select="$from"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$baseName"/>
            <xsl:value-of select="$count - 1"/>_</xsl:otherwise>
        </xsl:choose>
        <xsl:text> </xsl:text>
        <xsl:choose>
          <xsl:when test="$count = ($total - 2)">
            <xsl:value-of select="$to"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$baseName"/>
            <xsl:value-of select="$count + 1"/>_</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <!-- add new node to newpoints list -->
      <xsl:comment>inside of <xsl:value-of select="concat($baseName,$count,'_')"/>
      </xsl:comment>
      <saxon:assign name="newpoints">
        <xsl:copy-of select="$newpoints"/>
        <xsl:element name="Waypoint">
          <xsl:attribute name="id"><xsl:value-of select="concat($baseName,$count,'_')"/></xsl:attribute>
          <xsl:attribute name="neighbors"><xsl:value-of select="$neighbors"/></xsl:attribute>
          <xsl:element name="pose">
            <xsl:element name="Transformation">
              <xsl:attribute name="translation"><xsl:value-of select="$x"/> 0 <xsl:value-of select="$z"/></xsl:attribute>
            </xsl:element>
          </xsl:element>
        </xsl:element>
      </saxon:assign>
      <xsl:call-template name="nextPoint">
        <xsl:with-param name="baseName" select="$baseName"/>
        <xsl:with-param name="from" select="$from"/>
        <xsl:with-param name="to" select="$to"/>
        <xsl:with-param name="positionstring" select="substring-after($positionstring,' ')"/>
        <xsl:with-param name="count" select="$count + 1"/>
        <xsl:with-param name="total" select="$total"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>
  <!-- compute the correct neighbor name for a to node -->
</xsl:stylesheet>
