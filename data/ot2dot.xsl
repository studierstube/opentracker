<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
	<xsl:output method="text"/>
	<!-- The basic part of a dot file -->
	<xsl:template match="OpenTracker">
digraph "<xsl:value-of select=".//NetworkSinkConfig/@name"/>" 
{ 
	<xsl:apply-templates/>
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
		<xsl:if test="not(name(..)='OpenTracker')">
			<xsl:number count="*" format="a" level="any"/> -> <xsl:copy-of select="$parent"/>; 
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
	<xsl:template match="EventDynamicTransformation|QueueDynamicTransformation|TimeDynamicTransformation" mode="node">[shape=record,label="{{&lt;data&gt; Data | &lt;base&gt; Base } | &lt;bottom&gt; DynamicTransformation}"]</xsl:template>
	<xsl:template match="Merge" name="Merge" mode="node">[shape=record, label="{{<xsl:apply-templates mode="merge"/>} | &lt;bottom&gt; <xsl:value-of select="name(.)"/>} "]</xsl:template>
	<!-- merge sub node templates for defining node ports -->
	<xsl:template match="MergeDefault" mode="merge">&lt;default&gt; Default <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergePosition" mode="merge">&lt;position&gt; Position <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergeOrientation" mode="merge">&lt;orientation&gt; Orientation <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergeButton" mode="merge">&lt;button&gt; Button <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<xsl:template match="MergeConfidence" mode="merge">&lt;confidence&gt; Confidence <xsl:if test="not(position()=last())">|</xsl:if></xsl:template>
	<!-- default node style -->
	<xsl:template match="*" mode="node">[label="<xsl:value-of select="name(.)"/>",shape=box]</xsl:template>
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
	<xsl:template match="EventDynamicTransformation|QueueDynamicTransformation|TimeDynamicTransformation" mode="navigate">
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
	<!-- default navigate behaviour -->
	<xsl:template match="*" mode="navigate">
		<xsl:apply-templates>
			<xsl:with-param name="parent">
				<xsl:number count="*" format="a" level="any"/>
			</xsl:with-param>
		</xsl:apply-templates>
	</xsl:template>
</xsl:stylesheet>
