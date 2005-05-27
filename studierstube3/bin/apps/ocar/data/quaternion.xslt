<?xml version="1.0" encoding="UTF-8"?>
<!-- 
Vector and Quaternion library for XSLT
Copyright (C) 2003  Vienna University of Technology

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

For further information please contact Gerhard Reitmayr under
<reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna, Austria.
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
  <!-- Vector and Quaternion library for XSLT
      
       @author Gerhard Reitmayr 
       $header$ 
  -->
  <!-- 
	  3D vectors   "x y z" 
	  4D vectorc   "x y z h"
	  quaternions  "x y z w"
	  axis-angle   "x y z a" angle in radians
	  ? 3x3 matrices "a11 a12 a13 a21 a22 a23 a31 a32 a33" in row first order
	  ? 4x4 matrices "a11 a12 a13 a14 a21 a22 a23 a24 a31 a32 a33 a34 a41 a42 a43 a44" in row first order
	  -->
  <!-- current problems 
       - no implementations for sin, cos, sqrt. might take them from FXSL fxsl.sourceforge.net
    -->       
  <!-- plus a b : adds two 3 or 4 vectors -->
  <xsl:template name="plus">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:param name="b" select="0 0 0"/>
    <xsl:variable name="ax">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ay">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="az">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="aw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bx">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="by">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bz">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$ax+$bx"/><xsl:text> </xsl:text>
    <xsl:value-of select="$ay+$by"/><xsl:text> </xsl:text>
    <xsl:value-of select="$az+$bz"/>
    <xsl:if test="$aw and $bw">
      <xsl:text> </xsl:text>
      <xsl:value-of select="$aw+$bw"/>
    </xsl:if>
  </xsl:template>
  <!-- multScalar a b : computes the scalar product of a 3 or 4 vector and a scalar (b)-->
  <xsl:template name="multScalar">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:param name="b" select="1"/>
    <xsl:variable name="ax">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ay">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="az">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="aw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$ax*$b"/><xsl:text> </xsl:text>
    <xsl:value-of select="$ay*$b"/><xsl:text> </xsl:text>
    <xsl:value-of select="$az*$b"/>
    <xsl:if test="$aw">
      <xsl:text> </xsl:text>
      <xsl:value-of select="$aw*$b"/>
    </xsl:if>
  </xsl:template>
  <!-- minus a b : = plus a (multScalar b -1) : computes the difference of two 3 or 4 vectors -->
  <xsl:template name="minus">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:param name="b" select="0 0 0"/>
    <xsl:variable name="temp">
      <xsl:call-template name="multScalar">
        <xsl:with-param name="a" select="$b"/>
        <xsl:with-param name="b" select="-1"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="plus">
      <xsl:with-param name="a" select="$a"/>
      <xsl:with-param name="b" select="$temp"/>
    </xsl:call-template>
  </xsl:template>
  <!-- multComp a b : multiplies two 3 or 4 vectors component wise for scaling stuff -->
  <xsl:template name="multComp">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:param name="b" select="0 0 0"/>
    <xsl:variable name="ax">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ay">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="az">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="aw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bx">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="by">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bz">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$ax*$bx"/><xsl:text> </xsl:text>
    <xsl:value-of select="$ay*$by"/><xsl:text> </xsl:text>
    <xsl:value-of select="$az*$bz"/>
    <xsl:if test="$aw and $bw">
      <xsl:text> </xsl:text>
      <xsl:value-of select="$aw*$bw"/>
    </xsl:if>
  </xsl:template>
  <!-- dot a b : computes the scalar product of two 3 or 4 vectors -->
  <xsl:template name="dot">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:param name="b" select="0 0 0"/>
    <xsl:variable name="ax">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ay">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="az">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="aw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bx">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="by">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bz">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$aw and $bw">
        <xsl:value-of select="$ax*$bx+$ay*$by+$az*$bz+$aw*$bw"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$ax*$bx+$ay*$by+$az*$bz"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- norm a : = dot a a : computes the norm of a 3 or 4 vector -->
  <xsl:template name="norm">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:call-template name="dot">
      <xsl:param name="a" select="$a"/>
      <xsl:param name="b" select="$a"/>
    </xsl:call-template>
  </xsl:template>
  <!-- len a : = sqrt( norm a ) : computes the length of a 3 or 4 vector -->
  <xsl:template name="len">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:variable name="len">
      <xsl:call-template name="norm">
        <xsl:param name="a" select="$a"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="sqrt($len)"/>
  </xsl:template>
  <!-- normalize a : = multScalar a 1/(len a) : normalizes a 3 or 4 vector -->
  <xsl:template name="normalize">
    <xsl:param name="a" select="1 0 0"/>
    <xsl:variable name="len">
      <xsl:call-template name="len">
        <xsl:param name="a" select="$a"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="$len = 0"><xsl:value-of select="$a"/></xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="multScalar">
          <xsl:with-param name="a" select="$a"/>
          <xsl:with-param name="b" select="1 div $len"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!-- cross a b : computes the cross product of two 3 vectors -->
  <xsl:template name="cross">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:param name="b" select="0 0 0"/>
    <xsl:variable name="ax">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ay">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="az">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bx">
      <xsl:call-template name="X"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="by">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bz">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$b"/></xsl:call-template>
    </xsl:variable>
    <!-- todo implementation here -->
  </xsl:template>
  <!-- multQuat a b : computes the product of two quaternions a, b as a*b -->
  <xsl:template name="multQuat">
    <xsl:param name="a" select="0 0 0 1"/>
    <xsl:param name="b" select="0 0 0 1"/>
    <xsl:variable name="ax">
      <xsl:call-template name="X">
        <xsl:with-param name="var" select="$a"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ay">
      <xsl:call-template name="Y">
        <xsl:with-param name="var" select="$a"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="az">
      <xsl:call-template name="Z">
        <xsl:with-param name="var" select="$a"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="aw">
      <xsl:call-template name="W">
        <xsl:with-param name="var" select="$a"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bx">
      <xsl:call-template name="X">
        <xsl:with-param name="var" select="$b"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="by">
      <xsl:call-template name="Y">
        <xsl:with-param name="var" select="$b"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bz">
      <xsl:call-template name="Z">
        <xsl:with-param name="var" select="$b"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="bw">
      <xsl:call-template name="W">
        <xsl:with-param name="var" select="$b"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="$aw*$bx + $ax*$bw + $ay*$bz - $az*$by"/><xsl:text> </xsl:text>
    <xsl:value-of select="$aw*$by - $ax*$bz + $ay*$bw + $az*$bx"/><xsl:text> </xsl:text>
    <xsl:value-of select="$aw*$bz + $ax*$by - $ay*$bx + $az*$bw"/><xsl:text> </xsl:text>
    <xsl:value-of select="$aw*$bw - $ax*$bx - $ay*$by - $az*$bz"/>
  </xsl:template>
  <!-- conj a : computes the conjugate of a quaternion -->
  <xsl:template name="conj">
    <xsl:param name="a" select="0 0 0 1"/>
    <xsl:variable name="ax">
    <xsl:call-template name="X"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="ay">
      <xsl:call-template name="Y"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="az">
      <xsl:call-template name="Z"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="aw">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="-$ax"/><xsl:text> </xsl:text>
    <xsl:value-of select="-$ay"/><xsl:text> </xsl:text>
    <xsl:value-of select="-$az"/><xsl:text> </xsl:text>
    <xsl:value-of select="$aw"/>
  </xsl:template>
  <!-- inverse a : multScalar (conj a) 1/(norm a) : computes the inverse of a quaternion -->
  <xsl:template name="inverse">
    <xsl:param name="a" select="0 0 0 1"/>
    <xsl:variable name="n">
      <xsl:call-template name="norm">
        <xsl:with-param name="a" select="$a"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="multScalar">
      <xsl:with-param name="a">
        <xsl:call-template name="conj">
          <xsl:with-param name="a" select="$a"/>
        </xsl:call-template>
      </xsl:with-param>
      <xsl:with-param name="b">
        <xsl:value-of select="1 div $n"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  <!-- rotate a b : multQuat (multQuat b (a 0)) (conj b) : rotates the 3 vector a by the quaternion b -->
  <xsl:template name="rotate">
    <xsl:param name="a" select="0 0 0"/>
    <xsl:param name="b" select="0 0 0 1"/>
    <xsl:variable name="temp">
      <xsl:call-template name="multQuat">
        <xsl:with-param name="a">
          <xsl:call-template name="multQuat">
            <xsl:with-param name="a" select="$b"/>
            <xsl:with-param name="b"><xsl:value-of select="$a"/><xsl:text> 0</xsl:text></xsl:with-param>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="b">
          <xsl:call-template name="conj">
            <xsl:with-param name="a" select="$b"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:call-template name="R"><xsl:with-param name="var" select="$temp"/></xsl:call-template>
  </xsl:template>  
  <!-- axisAngle2Quat a : = (multScalar (R a) sin((W a)/ 2)) ' ' cos((W a)/ 2 ) : 
       transforms an axis angle representation in a 4 vector into a quaternion -->
  <xsl:template name="axisAngle2Quat">
    <xsl:param name="a" select="0 0 1 0"/>
    <xsl:variable name="angle">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$a"/></xsl:call-template>
    </xsl:variable>
    <xsl:variable name="sina2" select="sin($angle div 2)"/>
    <xsl:variable name="cosa2" select="cos($angle div 2)"/>
    <xsl:call-template name="multScalar">
      <xsl:with-param name="a">
        <xsl:call-template name="R"><xsl:with-param name="var" select="$a"/></xsl:call-template>
      </xsl:with-param>
      <xsl:with-param name="b" select="$sina2"/>
    </xsl:call-template><xsl:text> </xsl:text>
    <xsl:value-of select="$cosa2"/>
  </xsl:template>
  <!-- helper templates for simple operations -->
  <!-- extracts X component of vectors or quaternions -->
  <xsl:template name="X">
    <xsl:param name="var" select="0 0 0"/>
    <xsl:value-of select="substring-before($var,' ')"/>
  </xsl:template>
  <!-- extracts Y component of vectors or quaternions -->
  <xsl:template name="Y">
    <xsl:param name="var" select="0 0 0"/>
    <xsl:value-of select="substring-before(normalize-space(substring-after($var,' ')),' ')"/>
  </xsl:template>
  <!-- extracts Z component of vectors or quaternions -->
  <xsl:template name="Z">
    <xsl:param name="var" select="0 0 0"/>
    <xsl:value-of select="substring-before(normalize-space(substring-after(normalize-space(substring-after($var,' ')),' ')),' ')"/>
  </xsl:template>
  <!-- extracts W component of vectors or quaternions -->
  <xsl:template name="W">
    <xsl:param name="var" select="0 0 0 1"/>
    <xsl:value-of select="normalize-space(substring-after(normalize-space(substring-after(normalize-space(substring-after($var,' ')),' ')),' '))"/>
  </xsl:template>
    <!-- extracts the R vector component of a quaternion-->
  <xsl:template name="R">
    <xsl:param name="var" select="0 0 0 1"/>
    <xsl:variable name="w">
      <xsl:call-template name="W"><xsl:with-param name="var" select="$var"/></xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="substring($var,1,string-length($var)-string-length($w)-1)"/>
  </xsl:template>
</xsl:stylesheet>
