<?xml version="1.0" encoding="UTF-8"?>
<?xmlspysamplexml aml.xsd?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <xsl:output method="xml" omit-xml-declaration="yes" indent="yes" doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN" encoding="iso-8859-1"/>

  <!-- 
    some global variables
    -->
    <xsl:variable name="whitespace"><xsl:text>                                   </xsl:text></xsl:variable>
  
  <!-- master template that generates the html file. Probably everything fixed goes in here. -->
  <xsl:template match="xsd:schema">
    <html>
      <xsl:call-template name="htmlheader"/>
      <body>
      
        <!-- title from schema annotation element -->
        <h1>
        <xsl:choose>
          <xsl:when test="xsd:annotation/xsd:documentation/title">
          <xsl:value-of select="xsd:annotation/xsd:documentation/title"/>
          </xsl:when>
          <xsl:otherwise>Schema Documentation</xsl:otherwise>
        </xsl:choose>
        </h1>
        <xsl:apply-templates select="xsd:annotation"/>
        
        <!-- other sections -->
        <h2>Global Simple Types</h2>
        <xsl:apply-templates select="xsd:simpleType"/>
        <h2>Global Complex Types</h2>
        <xsl:apply-templates select="xsd:complexType"/>
        <h2>Global Elements</h2>
        <xsl:apply-templates select="xsd:element"/>
        <h2>Comments</h2>
        <xsl:apply-templates mode="comments"/>
      </body>
    </html>
  </xsl:template>  
  <!-- 
      comments are output in some special fashion ?? 
    -->
  <xsl:template match="comment()" mode="comments">
    <p>
      <em>
        <xsl:value-of select="."/>
      </em>
    </p>
  </xsl:template>
  <xsl:template match="text()" mode="comments"/>  
  
  <!-- 
      general template for global simple types 
    -->
<xsl:template match="xsd:simpleType">
    <div class="element"><xsl:attribute name="id">st-<xsl:value-of select="@name"/></xsl:attribute>
      <h3>
        <span class="elname">
          <xsl:value-of select="@name"/>
        </span>
      </h3>
      <h4>Syntax</h4>
<pre class="syntax">
<xsl:apply-templates mode="pretty" select="."/>
</pre>
      <h4>Description</h4>
       <xsl:apply-templates select="./xsd:annotation"/>
  </div>
  </xsl:template>
  
  <!-- 
      general template for global complex types 
    -->
  <xsl:template match="xsd:complexType">
    <xsl:variable name="level" select="count(ancestor-or-self::xsd:complexType|ancestor-or-self::xsd:element)"/>
    <div class="element"><xsl:attribute name="id">ct-<xsl:value-of select="@name"/></xsl:attribute>
      <h3>
        <span class="elname">
          <xsl:value-of select="@name"/>
        </span>
      </h3>
      <h4>Syntax</h4>
<pre class="syntax">
<xsl:apply-templates mode="pretty" select="."/>
</pre>
      <!-- attributes -->
      <xsl:if test=".//xsd:attribute[count(ancestor-or-self::xsd:complexType|ancestor-or-self::xsd:element) = $level]">
      <h4>Attributes</h4>
      <dl class="attribute-list">
        <xsl:for-each select=".//xsd:attribute">
        <xsl:if test="count(ancestor-or-self::xsd:complexType|ancestor-or-self::xsd:element) = $level">
        <dt><xsl:attribute name="id">attr-presentation-<xsl:value-of select="@name"/></xsl:attribute><xsl:value-of select="@name"/></dt>
        <dd><xsl:apply-templates select="./xsd:annotation"/>
        </dd>
        </xsl:if>
        </xsl:for-each>
      </dl>
      </xsl:if>
      <!-- description -->
      <h4>Description</h4>
       <xsl:apply-templates select="./xsd:annotation"/>
      <!-- sub elements -->
      <xsl:if test=".//xsd:element[@name]">
         <h4>Sub elements</h4>
        <xsl:apply-templates select="*" mode="sub"/>
      </xsl:if>
  </div>
  </xsl:template>
  
  <!-- 
        general template for global elements 
    -->
  <xsl:template match="xsd:element">
    <xsl:variable name="level" select="count(ancestor-or-self::xsd:element)"/>
    <div class="element"><xsl:attribute name="id">el-<xsl:value-of select="@name"/></xsl:attribute>
      <h3>
        <span class="elname">
          <xsl:value-of select="@name"/>
        </span>
      </h3>
      <!-- syntax -->
      <h4>Syntax</h4>
<pre class="syntax">
<xsl:apply-templates mode="pretty" select="."/>
</pre>
      <!-- attributes -->
      <xsl:if test=".//xsd:attribute[count(ancestor-or-self::xsd:element) = $level]">      
      <h4>Attributes</h4>
      <dl class="attribute-list">
        <xsl:for-each select=".//xsd:attribute">
        <xsl:if test="count(ancestor-or-self::xsd:element) = $level">
        <dt><xsl:attribute name="id">attr-presentation-<xsl:value-of select="@name"/></xsl:attribute><xsl:value-of select="@name"/></dt>
        <dd><xsl:apply-templates select="./xsd:annotation"/>
        </dd>
        </xsl:if>
        </xsl:for-each>
      </dl>
      </xsl:if>
      <!-- description -->
      <h4>Description</h4>
      <xsl:apply-templates select="./xsd:annotation"/>
      <!-- sub elements -->
      <xsl:if test=".//xsd:element[@name]">
       <h4>Sub elements</h4>
       <xsl:apply-templates select="*" mode="sub"/>
      </xsl:if>
    </div>
  </xsl:template>

  <!--
      templates for embedded elements. This describes subelements of global types and elements.
    -->
   <xsl:template match="xsd:element[@name]" mode="sub">
    <xsl:variable name="level" select="count(ancestor-or-self::xsd:element)"/>
    <div class="element"><xsl:attribute name="id">el-<xsl:value-of select="@name"/></xsl:attribute>
      <h3>
        <span class="elname"><xsl:value-of select="@name"/></span>
      </h3>
      <!-- attributes -->
      <xsl:if test=".//xsd:attribute[count(ancestor-or-self::xsd:element) = $level]">      
      <h4>Attributes</h4>
      <dl class="attribute-list">
        <xsl:for-each select=".//xsd:attribute">
        <xsl:if test="count(ancestor-or-self::xsd:element) = $level">
        <dt><xsl:attribute name="id">attr-presentation-<xsl:value-of select="@name"/></xsl:attribute><xsl:value-of select="@name"/></dt>
        <dd><xsl:apply-templates select="./xsd:annotation"/>
        </dd>
        </xsl:if>
        </xsl:for-each>
      </dl>
      </xsl:if>
      <h4>Description</h4>
      <xsl:apply-templates select="./xsd:annotation"/>
      <xsl:apply-templates select="*" mode="sub"/>
    </div>
  </xsl:template>

  <xsl:template match="*"  mode="sub">
    <xsl:apply-templates select="*" mode="sub"/>
  </xsl:template>

  <!-- utility templates -->
  <!-- 
      pretty print transformation for copying selectively parts of the schema into the result.
      Don't change the formatting of this part, because the <pre> thing will be wrong otherwise !
      Everything should stick to the left border.
    -->
<xsl:template match="xsd:annotation" mode="pretty"/>

<xsl:template match="node()" mode="pretty">
<xsl:param name="indent" select="1"/>
<xsl:value-of select="substring($whitespace,1,$indent)"/>
<!-- Start Tag -->
<xsl:text>&lt;</xsl:text>      
<xsl:value-of select="local-name(.)"/>
<!-- Attributes -->
<xsl:for-each select="./@*">
<xsl:text> </xsl:text>
<xsl:value-of select="name(.)"/>
<xsl:text>="</xsl:text>
<xsl:value-of select="."/>
<xsl:text>"</xsl:text>
</xsl:for-each>
		
<xsl:choose>
<xsl:when test="./* | ./text()">
<!-- Close Start Tag -->
<xsl:text>> 
</xsl:text>
<!-- Content -->
<xsl:apply-templates select="./* | ./text()" mode="pretty">
  <xsl:with-param name="indent" select="$indent + 2"/>
</xsl:apply-templates>
<!-- End Tag -->
<xsl:value-of select="substring($whitespace,1,$indent)"/>
<xsl:text>&lt;/</xsl:text>
<xsl:value-of select="local-name(.)"/>
<xsl:text>>
</xsl:text>
</xsl:when>
<xsl:otherwise>
<!-- Close Start Tag -->
<xsl:text>/> 
</xsl:text>
</xsl:otherwise>
</xsl:choose>
</xsl:template>
  
  <!-- 
      Annotation templates. They are matched against all annotation data and mainly provide the formatting
      for it. They copy all unknown xml structures verbatim allowing to directly use html markup in the 
      annotations. xsd:annotation and xsd:documentation are not put out. They also support a number of
      special markups for our own documentation format :
      
      <title></title> specified in the xsd:schema/xsd:annotation section will set the title of the document.
      <note></note> generates a note box that can be expanded.
      <question></question> generates a question box that can be expanded
      
      all other text is framed by a <div class="description"></div> markup.
    -->
    <xsl:template match="note">
      <div class="note">
        <xsl:apply-templates/>
      </div>
    </xsl:template>
  
    <xsl:template match="question">
      <div class="question">
        <xsl:apply-templates/>
      </div>
    </xsl:template>
    
    <xsl:template match="text()">
      <div class="description">
        <xsl:value-of select="."/>
      </div>
    </xsl:template>
        
    <xsl:template match="title" />
    
    <xsl:template match="xsd:documentation|xsd:annotation">
      <xsl:apply-templates/>
    </xsl:template>
    
    <xsl:template match="@*|node()">
      <xsl:copy>
        <xsl:apply-templates select="@*|node()"/>
      </xsl:copy>
    </xsl:template>
    
  <!-- 
      htmlheader produces the header part of the html document. It avoids having huge stuff at the 
      beginning of the style sheet. It is now completely self contained.
    -->
  <xsl:template name="htmlheader">
    <head>
      <title>Test Schema Documenter</title>
      <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"/>
      <style type="text/css">
        <xsl:comment>
.note {
	background-color: #f0fff0;
	border: 1px solid #bbeebb;
	margin-top: 2px;
	margin-bottom: 2px;
}

.notebutton{
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 9px;
	background-color: #f0fff0;
	border: 1px solid #bbeebb;
	padding: 2px;
	margin-top: 8px;
	margin-bottom: 3px;	
/*	cursor: pointer;
	cursor: hand;
*/
}

.notebutton a {
	color: inherit;
	text-decoration: none;
}

.notebutton a:hover {
	text-decoration: underline;
}

body {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 9pt;
}
.question {
	background-color: #ffcccc;
	border: 1px solid #ff9999;
}

.questionbutton{
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size: 9px;
	background-color: #ffcccc;
	border: 1px solid #ff9999;
	padding: 2px;
	margin-top: 8px;
	margin-bottom: 3px;	
/*	cursor: pointer;
	cursor: hand;
*/
}

.questionbutton a {
	color: inherit;
	text-decoration: none;
}

.questionbutton a:hover {
	text-decoration: underline;
}

h1 {
	font-size: 16pt;
	text-align: center;
}
h2 {
	font-size: 12pt;
	border: 1px solid #dddddd;
	background-color: #f0f0f0;
	margin-top: 80px;
}
h3 {
	font-size: 11pt;
	margin-top: 0px;
}
h4 {
	font-size: 8pt;
}

H2:before {
    content: counter(chapter) ". ";
    counter-increment: chapter;  /* Add 1 to chapter */
    counter-reset: section;      /* Set section to 0 */
}
H3:before {
    content: counter(chapter) "." counter(section) " ";
    counter-increment: section;
}

.syntax {
	background-color: #fafaff;
	border: 1px solid #ddddee;
	font-family: "Courier New", Courier, mono;
	font-size: 10pt;
	line-height: normal;
}

.example {
	background-color: #fffffa;
	border: 1px solid #eeeedd;
	font-family: "Courier New", Courier, mono;
	font-size: 10pt;
	line-height: normal;
}

.elname {
	font-family: "Courier New", Courier, mono;
}

.element {
	border: 1px solid #eeeeee;
	margin-top: 15px;
	margin-bottom: 15px;
	padding: 5px;
}

dl.attribute-list dt {
	font-family: "Courier New", Courier, mono;
}

.attr-type {
	font-family: "Courier New", Courier, mono;
	display:block;
}

.description {
}
.important {
	font-size: 12px;
	font-weight: bold;
	background-color: #FFcccc;
	border: 2px solid #ff0000;
	padding: 5px;
}
</xsl:comment>
      </style>
      <script type="text/javascript">
      <xsl:text disable-output-escaping="yes">
      &lt;!--
function init(){		
		makeAll(document.body);
}

function makeAll(el){

	// cache this now because position in tree might change!
	var next;
	if (el.nextSibling) next = el.nextSibling;
	
	if (el.className == "note") {
		makeNote(el);
	}
	if (el.className == "question") {
		makeNote(el);
	}
	if (el.className == "elname") {
		makeLink(el);
	}
		
	if (el.firstChild) makeAll(el.firstChild);
	if (next) makeAll(next);
}

var notenum = 1;

function makeNote(el){
	if (!el.id) el.id = el.className + "-auto" + (notenum++);
	
	var button = document.createElement("SPAN");
	button.id = "toggle-" + el.id;
	button.className = el.className + "button";
	var name = el.className;
	button.innerHTML = name.substring(0,1).toUpperCase() + name.substring(1,name.length) + " [&lt;a href='#' onclick='toggle(\"" + el.id + "\"); return false;'&gt;toggle&lt;/a&gt;]";
	button.onclick = toggle;
	
	el.parentNode.insertBefore(button, el);
	el.style.display="none";
}

function makeLink(el){
	var linkel = document.getElementById("el-" + el.innerHTML);
	if (linkel &amp;&amp; !isChildOf(el, linkel)){
		el.innerHTML = "&lt;a href='#el-" + el.innerHTML + "'&gt;" + el.innerHTML + "&lt;/a&gt;";
	}
} 

function isChildOf(child, parent){
	if (parent.hasChildNodes){
		var children = parent.childNodes;
		for (var i=0; i&lt;children.length; i++){
			if ((children[i] == child) || (isChildOf(child, children[i]))){
				return true;
			}
		}
	}
	return false;
}	

function toggle(name){
	var el = document.getElementById(name);
	if (el) {
		if (el.style.display == "none") {
			el.style.display = "block";
		}
		else {
			el.style.display = "none";
		}
	}
}			

window.onload = init;
//--&gt; </xsl:text>
     </script>
    </head>
  </xsl:template>
</xsl:stylesheet>
