<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes"/>
  <!--<xsl:strip-space elements="*"/> -->

  <!-- Recursive loop to diplay levels -->
  <xsl:template name="level-loop">
    <xsl:param name="count"/>
    <xsl:if test="$count > 0">
      
      <xsl:element name="Level">
	<xsl:attribute name="depth"> <xsl:value-of select="$count"/> </xsl:attribute>
	<xsl:attribute name="coherence_protocol">MESI</xsl:attribute>
	<xsl:attribute name="is_inclusive">true</xsl:attribute>
      </xsl:element>

      <xsl:call-template name="level-loop">
	<xsl:with-param name="count" select="$count - 1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <!-- begin applying changes -->
  <xsl:template match="/">

    <xsl:element name="Architecture">
      <xsl:attribute name="name"> <xsl:value-of select="//info[@name='Architecture']/@value"/> </xsl:attribute>
      <xsl:attribute name="CPU_name"> <xsl:value-of select="//info[@name='CPUModel']/@value"/> </xsl:attribute>
      <xsl:attribute name="number_levels"> <xsl:value-of select="//object[@type='Cache'][1]/@depth"/> </xsl:attribute>

      <xsl:call-template name="level-loop">
	<xsl:with-param name="count" select="//object[@type='Cache'][1]/@depth"/>
      </xsl:call-template>

      <xsl:apply-templates select="//object[@type='Cache']"/>

    </xsl:element>
  </xsl:template>


  <!-- Recursive loop to display caches -->
  <xsl:template match="//object[@type='Cache']">
    <xsl:element name="Cache">

      <xsl:if test="./Cache">
	<xsl:apply-templates select="./Cache"/>
      </xsl:if>

      <xsl:attribute name="depth">  <xsl:value-of select="@depth"/> </xsl:attribute>
      <xsl:attribute name="cache_size">  <xsl:value-of select="@cache_size"/> </xsl:attribute>
      <xsl:attribute name="cache_linesize">  <xsl:value-of select="@cache_linesize"/> </xsl:attribute>
      <xsl:attribute name="cache_associativity">  <xsl:value-of select="@cache_associativity"/> </xsl:attribute>
      <xsl:attribute name="replacement_protocol">LRU</xsl:attribute>
    </xsl:element>
  </xsl:template>

</xsl:stylesheet>