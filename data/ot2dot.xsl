<?xml version="1.0" encoding="UTF-8"?>
<!--
OpenTracker Configuration to dot style sheet
Copyright (C) 2001  Vienna University of Technology

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
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
<xsl:output method="text"/>

<!-- The basic part of a dot file -->
<xsl:template match="OpenTracker">
digraph "<xsl:value-of select=".//NetworkSinkConfig/@name"/>" 
{ 
 	orientation = "landscape";
<xsl:apply-templates/>
	{ rank = same; <xsl:for-each select="*[position()>1]"> <xsl:number count="*" format="a" level="any"/>;</xsl:for-each> }
} 
</xsl:template>

<!-- template removing configuration part -->
<xsl:template match="configuration" priority="2"/>
<!-- template counts elements to provide unique node names and passes the current node name as parent to its children 
     after the node definition it calls the templates with mode node for any special definition -->
<xsl:template match="*">
	<xsl:param name="parent"/>
	<xsl:number count="*" format="a" level="any"/>
	<xsl:apply-templates select="." mode="node"/>; 		
	<xsl:if test="not(name(..) = 'OpenTracker')">
  	   <xsl:number count="*" format="a" level="any"/> -> <xsl:copy-of select="$parent"/>; 
	</xsl:if>
	<!-- if the current element is a Ref element, add also an edge from the referenced element to the current -->
	<xsl:if test="name(.) = 'Ref'">
		<xsl:for-each select="//*[@DEF=current()/@USE]"><xsl:number count="*" format="a" level="any"/></xsl:for-each> -> <xsl:number count="*" format="a" level="any"/>;
	</xsl:if>
	<xsl:apply-templates select="." mode="navigate"/>
</xsl:template>
	
	<!-- templates for customization of nodes, these have the mode node -->
	<xsl:template match="NetworkSink" name="NetworkSink" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@name"/> - <xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="ARToolKitSource" name="ARToolKitSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@tag-file"/>"]</xsl:template>
	<xsl:template match="ConsoleSink" name="ConsoleSink" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@comment"/>"]</xsl:template>
	<xsl:template match="NetworkSource" name="NetworkSource" mode="node">[label="<xsl:value-of select="name(.)"/>\n<xsl:value-of select="@number"/>"]</xsl:template>
	<xsl:template match="TestSource" name="TestSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="ConsoleSource" name="ConsoleSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="InterTraxSource" name="InterTraxSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>	
	<xsl:template match="InterSenseSource" name="InterSenseSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="StbSink" name="StbSink" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	
	<xsl:template match="EventDynamicTransform|QueueDynamicTransform|TimeDynamicTransform" mode="node">[shape=record,label="{{&lt;data&gt; Data | &lt;base&gt; Base } | &lt;bottom&gt; DynamicTransformation}"]</xsl:template>	
	<xsl:template match="WacomGraphireSource" name="WacomGraphireSource" mode="node">[label="<xsl:value-of select="name(.)"/>"]</xsl:template>
	<xsl:template match="Ref" name="Ref" mode="node"> [ label="<xsl:value-of select="name(.)"/>" , shape=box , height=0.3 , width=.45 ]</xsl:template>
	<xsl:template match="Selection" mode="node">[shape=record,label="{{&lt;select&gt; Select | &lt;default&gt; Default } |&lt;bottom&gt; Selection}"]</xsl:template>

	<!-- merge sub node templates for defining node ports -->
	<xsl:template match="Merge" name="Merge" mode="node">[shape=record, label="{{<xsl:apply-templates mode="merge"/>} | &lt;bottom&gt; <xsl:value-of select="name(.)"/>} "]</xsl:template>
	<xsl:template match="MergeDefault" mode="merge">&lt;default&gt; Default <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergePosition" mode="merge">&lt;position&gt; Position <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergeOrientation" mode="merge">&lt;orientation&gt; Orientation <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergeButton" mode="merge">&lt;button&gt; Button <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergeConfidence" mode="merge">&lt;confidence&gt; Confidence <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>

	<!-- default node style -->
	<xsl:template match="*" mode="node"> [ label="<xsl:value-of select="name(.)"/>", shape=box ]</xsl:template>
	
	<!-- templates for going down the children tree, this is needed to use dot node ports, these have mode navigate -->
	<!-- Merge children tree templates -->
	<xsl:template match="Merge" mode="navigate">
		<xsl:apply-templates mode="merge-navigate">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeDefault" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:default</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergePosition" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:position</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeOrientation" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:orientation</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeButton" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:button</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="MergeConfidence" mode="merge-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:confidence</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	
	<!-- DynamicTransformation navigation behaviour -->
	<xsl:template match="EventDynamicTransform|QueueDynamicTransform|TimeDynamicTransform" mode="navigate">
		<xsl:apply-templates mode="dynamic-navigate">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>	
	<xsl:template match="TransformBase" mode="dynamic-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:base</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="*" mode="dynamic-navigate">
		<xsl:param name="parent"/>
		<xsl:apply-templates select=".">			
			<xsl:with-param name="parent">	<xsl:copy-of select="$parent"/>:data</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template> 
	
	<!-- Selection navigation behaviour -->
	<xsl:template match="Selection" mode="navigate">
		<xsl:apply-templates mode="dynamic-navigate-selection">
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>	
	<xsl:template match="Select" mode="dynamic-navigate-selection">
		<xsl:param name="parent"/>
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:copy-of select="$parent"/>:select</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	<xsl:template match="*" mode="dynamic-navigate-selection">
		<xsl:param name="parent"/>
		<xsl:apply-templates select=".">			
			<xsl:with-param name="parent">	<xsl:copy-of select="$parent"/>:default</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
	
	<!-- default navigate behaviour -->
	<xsl:template match="*" mode="navigate">
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
</xsl:stylesheet>
