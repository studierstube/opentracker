<!--=======================================================================-->
<!-- This transformation file contains various helper templates to deal with
     lists, sets etc. It is included by other transformation files that
     make use of these templates. Each template is documented with input 
     parameters and output format.

     The templates are probably not very optimised. Some templates act as a
     facade to the actual implementation which might take additional parameters
     or a special input format. The facade template provides a
     clean interface and some pre-processing to improve performance and simplify.
     The implementation templates are sort of 'internal' and should not be used
     from the outside.
     
     space separated lists of values :
       elementOf
       isSubsetOf
       generateNumbers
       generateListRepeat
       length
       
     space separated lists as sets :
       makeSet
       union 

     OIV style string lists (quoted and separated by ',') :
       quoteList
          
     @author Gerhard Reitmayr
     $Id: tools.xslt 3427 2004-07-14 12:20:49Z tamer $
-->
<!--=======================================================================--> 
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!--=======================================================================-->
<!-- tests if a given string without spaces is in a space separated list of 
     strings.
     Inputs : element - the element to look for
              list    - the list to look into
     Outputs: 0       - if not found
              1       - if found
-->
<!--=======================================================================-->   
<xsl:template name="elementOf">
  <xsl:param name="element"/>
  <xsl:param name="list"/>
  <xsl:choose>
    <xsl:when test="$element = $list">1</xsl:when>
    <xsl:when test="string-length($list) > string-length($element)">
      <xsl:choose>
        <xsl:when test="starts-with($list, concat($element,' '))">1</xsl:when>
        <xsl:when test="contains($list,concat(' ', $element, ' '))">1</xsl:when>
        <xsl:when test="substring($list, string-length($list)-string-length($element)+1) = $element">1</xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>      
    </xsl:when>
    <xsl:otherwise>0</xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!--=======================================================================-->
<!-- tests if all elements in list a are included in list b. it implements
     a set like inclusion test, that is the list a need not appear in order 
     in b.
     Inputs : a     - the sub list to test for
              b     - the list to look into
     Outputs: 0     - if not found
              1     - if found
-->
<!--=======================================================================-->
<xsl:template name="isSubsetOf">
  <xsl:param name="a"/>
  <xsl:param name="b"/>
  <xsl:variable name="head">
    <xsl:choose>
      <xsl:when test="contains($a,' ')"><xsl:value-of select="substring-before($a,' ')"/></xsl:when>
      <xsl:otherwise><xsl:value-of select="$a"/></xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="tail">
    <xsl:if test="contains($a,' ')">
      <xsl:value-of select="normalize-space(substring-after($a,' '))"/>
    </xsl:if>      
  </xsl:variable>
  <xsl:choose>
    <xsl:when test="not($head = '')">
      <xsl:variable name="test">
        <xsl:call-template name="elementOf">
          <xsl:with-param name="element" select="$head"/>
          <xsl:with-param name="list" select="$b"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$test = 1">
          <xsl:call-template name="isSubsetOf">
            <xsl:with-param name="a" select="$tail"/>
            <xsl:with-param name="b" select="$b"/>  
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:otherwise>1</xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!--=======================================================================-->
<!-- generates space separated list of numbers starting from next until len
     Inputs : next  - the first number, 0 by default
              len   - the last number, 0 by default
     Outputs: next next+1 ... len-1 len
-->
<!--=======================================================================-->
<xsl:template name="generateNumbers">
  <xsl:param name="len" select="'0'"/>
  <xsl:param name="next" select="'0'"/>
  <xsl:if test="$next &lt; $len"><xsl:value-of select="$next"/><xsl:if test="$next &lt; $len - 1"><xsl:text> </xsl:text></xsl:if>
    <xsl:call-template name="generateNumbers">
      <xsl:with-param name="len" select="$len"/>
      <xsl:with-param name="next" select="$next + 1"/>  
    </xsl:call-template>
  </xsl:if>
</xsl:template>
  
<!--=======================================================================-->
<!-- quotes a space separated list and comma-separates it for inventor 
     string lists
     Inputs : list         - input list on the format a b c d
              itemsPerLine - number of items to put on a line
     Outputs: "a", "b", "c", "d"
-->
<!--=======================================================================-->
<xsl:template name="quoteList">
  <xsl:param name="list" />
  <xsl:param name="itemsPerLine" select="'-1'"/>
  <xsl:call-template name="quoteListImpl">
    <xsl:with-param name="list" select="normalize-space($list)"/>
    <xsl:with-param name="count" select="'0'" />
    <xsl:with-param name="itemsPerLine" select="$itemsPerLine"/>
  </xsl:call-template>
</xsl:template>

<xsl:template name="quoteListImpl">
  <xsl:param name="list"/>
  <xsl:param name="count" select="'0'"/>
  <xsl:param name="itemsPerLine" select="'-1'"/>
  <xsl:choose>
    <xsl:when test="contains($list,' ')">
      <xsl:text>"</xsl:text><xsl:value-of select="substring-before($list,' ')"/><xsl:text>",</xsl:text>
      <xsl:if test="($itemsPerLine > 0) and ($count+1) = $itemsPerLine"><xsl:text>
</xsl:text></xsl:if>
      <xsl:call-template name="quoteListImpl">
        <xsl:with-param name="list" select="substring-after($list,' ')"/>
        <xsl:with-param name="itemsPerLine" select="$itemsPerLine"/>
        <xsl:with-param name="count" select="($count + 1) mod $itemsPerLine" />
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>"<xsl:value-of select="$list"/>"</xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!--=======================================================================-->
<!-- generates a space separated list of len by repeating one item
     Inputs : item   - item to use, defaults to @id
              len    - number of entries in the list
     Outputs: item item ... item 
                 len times
-->
<!--=======================================================================-->
<xsl:template name="generateListRepeat">
  <xsl:param name="item" select="@id"/>
  <xsl:param name="len" select="'0'"/>
  <xsl:if test="$len > 0"><xsl:value-of select="$item"/><xsl:if test="$len > 1"><xsl:text> </xsl:text></xsl:if>
    <xsl:call-template name="generateListRepeat">
      <xsl:with-param name="item" select="$item"/>
      <xsl:with-param name="len" select="$len - 1"/>
    </xsl:call-template>
  </xsl:if>
</xsl:template>

<!--=======================================================================-->
<!-- throws out doubles of a space separated list, effectively generating
     Inputs : list   - space separated list, possibly containing multiple 
                       copies of a value
     Outputs: space separated list containing only unique items
-->
<!--=======================================================================-->
<xsl:template name="makeSet">
  <xsl:param name="list"/>
  <xsl:call-template name="makeSetImpl">
    <xsl:with-param name="list" select="normalize-space($list)"/>
  </xsl:call-template>
</xsl:template>

<!-- actual implementation of the function. The facade template provides a
     clean interface and some pre-processing to improve performance and simplify
     code -->
<xsl:template name="makeSetImpl">
  <xsl:param name="list"/>
  <xsl:param name="result"/>
  <xsl:variable name="head">
    <xsl:choose>
      <xsl:when test="contains($list,' ')"><xsl:value-of select="substring-before($list,' ')"/></xsl:when>
      <xsl:otherwise><xsl:value-of select="$list"/></xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="tail">
    <xsl:if test="contains($list,' ')">
      <xsl:value-of select="normalize-space(substring-after($list,' '))"/>
    </xsl:if>      
  </xsl:variable>
  <xsl:choose>
    <xsl:when test="$head = ''"><xsl:value-of select="$result"/></xsl:when>
    <xsl:otherwise>
      <xsl:variable name="test">
        <xsl:call-template name="elementOf">
          <xsl:with-param name="element" select="$head"/>
          <xsl:with-param name="list" select="$result"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$test = 1">
          <xsl:call-template name="makeSetImpl">
            <xsl:with-param name="list" select="$tail"/>
            <xsl:with-param name="result" select="$result"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:call-template name="makeSetImpl">
            <xsl:with-param name="list" select="$tail"/>
            <xsl:with-param name="result">
              <xsl:value-of select="$result"/>
              <xsl:text> </xsl:text>
              <xsl:value-of select="$head"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!--=======================================================================-->
<!-- generates the union of two space separated sets, returning a new set 
     Inputs : a - space separated list
              b - space separated list
     Outputs: space separated list being the union of a and b and 
              containing only unique items
-->
<!--=======================================================================-->
<xsl:template name="union">
  <xsl:param name="a"/>
  <xsl:param name="b"/>
  <xsl:call-template name="makeSet">
    <xsl:with-param name="list">
      <xsl:value-of select="$a"/>
      <xsl:text> </xsl:text>
      <xsl:value-of select="$b"/>
    </xsl:with-param>
  </xsl:call-template>
</xsl:template>

<!--=======================================================================-->
<!-- returns the length of a space separated list. It uses a simple string 
     translation trick to avoid a recursive counting implementation.
     Inputs : list - space separated list
              b - space separated list
     Outputs: number of items in list
-->
<!--=======================================================================-->
<xsl:template name="length">
  <xsl:param name="list"/>
  <xsl:choose>
    <xsl:when test="$list = ''">0</xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="string-length(translate(normalize-space($list),' ,abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.&quot;_','.'))+1"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

</xsl:stylesheet>
