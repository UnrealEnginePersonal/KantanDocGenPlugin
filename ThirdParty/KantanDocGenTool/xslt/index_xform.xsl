<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/TR/REC-html40" version="2.0">
	<xsl:output method="html"/>
	<!-- Root template -->
	<xsl:template match="/">
		<html>
			<head>
				<title>
					<xsl:value-of select="/root/display_name"/>
				</title>
				<link rel="stylesheet" type="text/css" href="./css/bpdoc.css"/>
			</head>
			<body>
				<div id="content_container">
					<xsl:apply-templates/>
				</div>
			</body>
		</html>
	</xsl:template>
	<!-- Templates to match specific elements in the input xml -->
	<xsl:template match="/root">
		<a class="navbar_style">
			<xsl:value-of select="display_name"/>
		</a>
		<h1 class="title_style">
			<xsl:value-of select="display_name"/>
		</h1>
		<xsl:apply-templates select="classes"/>
	</xsl:template>
	<xsl:template match="classes">
		<h2 class="title_style">Classes</h2>
		<table>
			<tbody>
				<xsl:apply-templates select="class">
					<xsl:sort select="type" order="descending"/>
					<xsl:sort select="display_name"/>
				</xsl:apply-templates>
			</tbody>
		</table>
	</xsl:template>
	<xsl:template match="class">
		<tr>
			<td width="5%">
				<div class="param_name title_style">
					<xsl:apply-templates select="type"/>
				</div>
			</td>
			<td width="25%">
				<a>
					<xsl:attribute name="href">./<xsl:value-of select="id"/>/<xsl:value-of select="id"/>.html</xsl:attribute>
					<xsl:apply-templates select="display_name"/>
				</a>
				<div class="group_type">
					<xsl:apply-templates select="group"/>
				</div>
			</td>
			<td width="70%">
				<xsl:apply-templates select="description"/>
			</td>
		</tr>
	</xsl:template>
</xsl:stylesheet>
