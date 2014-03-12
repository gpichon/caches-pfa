<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml" indent="yes"/>
  <xsl:strip-space elements="*"/>

  <xsl:variable name="nb_levels"/>

  <!-- Recursive loop to diplay levels -->
  <xsl:template name="level-loop">
    <xsl:param name="count"/>
    <xsl:param name="nb_levels"/>
    <xsl:if test="$count > 0">
      
      <xsl:element name="Level">
	<xsl:attribute name="depth"> <xsl:value-of select="$count"/> </xsl:attribute>
	
	<xsl:choose>
	  <!-- Last level -->
	  <xsl:when test="$count &gt;= $nb_levels">
	    <xsl:attribute name="type">inclusive</xsl:attribute>
	    <xsl:attribute name="directory_manager">false</xsl:attribute>
	  </xsl:when>
	  <!-- L1 -->
	  <xsl:when test="$count=1">
	    <xsl:attribute name="coherence_protocol">MESI</xsl:attribute>
	    <xsl:attribute name="snooping">true</xsl:attribute>
	  </xsl:when>
	  <!-- other levels (L2) -->
	  <xsl:otherwise>
	    <xsl:attribute name="coherence_protocol">MESI</xsl:attribute>
	    <xsl:attribute name="type">nieo</xsl:attribute>
	    <xsl:attribute name="snooping">true</xsl:attribute>
	    <xsl:attribute name="directory_manager">false</xsl:attribute>
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:element>

      <xsl:call-template name="level-loop">
	<xsl:with-param name="count" select="$count - 1"/>
	<xsl:with-param name="nb_levels" select="$nb_levels"/>
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
	<xsl:with-param name="nb_levels" select="//object[@type='Cache'][1]/@depth"/>
      </xsl:call-template>

      <xsl:apply-templates select="//object[@type='Cache'][1]"/>

    </xsl:element>
  </xsl:template>

  <!-- Recursive loop to display caches -->
  <xsl:template match="object[@type='Cache']">
    <xsl:if test="not(@cache_type) or @cache_type &lt;= 1">
      <xsl:element name="Cache">
	<xsl:copy-of select="@depth" />
	<xsl:copy-of select="@cache_size" />
	<xsl:copy-of select="@cache_linesize" />
	<xsl:copy-of select="@cache_associativity" />
	<xsl:attribute name="replacement_protocol">LRU</xsl:attribute>
	
	<!-- if the cache is lower, we create a child -->
	<xsl:if test="./object[@type='Cache'] and object/@depth &lt; ./@depth">
	  <xsl:apply-templates/>
	</xsl:if>

      </xsl:element>
      
      <!-- if the cache is at the same level -->
      <xsl:if test="./object[@type='Cache'] and object/@depth &gt;= ./@depth">
	<xsl:apply-templates/>
      </xsl:if>
      
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>