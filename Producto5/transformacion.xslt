<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output method="html" indent="yes" encoding="UTF-8"/>

	<xsl:template match="/">
		<html>
			<head>
				<title>Configuración de Red</title>
				<style>
					body {
					font-family: Arial, sans-serif;
					background-color: #f4f4f4;
					padding: 20px;
					}
					h1, h2 {
					color: #333;
					}
					table {
					border-collapse: collapse;
					width: 100%;
					margin-bottom: 40px;
					}
					th, td {
					border: 1px solid #ccc;
					padding: 8px;
					text-align: left;
					}
					th {
					background-color: #007bff;
					color: white;
					}
				</style>
			</head>
			<body>
				<h1>Resumen de Configuración de Red</h1>
				<table>
					<tr>
						<th>Campo</th>
						<th>Valor</th>
					</tr>
					<tr>
						<td>Adaptador</td>
						<td>
							<xsl:value-of select="ConfiguracionRed/Adaptador"/>
						</td>
					</tr>
					<tr>
						<td>Dirección IP</td>
						<td>
							<xsl:value-of select="ConfiguracionRed/IP"/>
						</td>
					</tr>
					<tr>
						<td>Máscara de subred</td>
						<td>
							<xsl:value-of select="ConfiguracionRed/Mascara"/>
						</td>
					</tr>
					<tr>
						<td>Puerta de enlace</td>
						<td>
							<xsl:value-of select="ConfiguracionRed/Gateway"/>
						</td>
					</tr>
					<tr>
						<td>Servidor DNS</td>
						<td>
							<xsl:value-of select="ConfiguracionRed/DNS"/>
						</td>
					</tr>
					<tr>
						<td>Latencia media</td>
						<td>
							<xsl:value-of select="ConfiguracionRed/LatenciaMedia"/> ms
						</td>
					</tr>
				</table>

				<h2>Saltos hacia el DNS</h2>
				<table>
					<tr>
						<th>#</th>
						<th>Tiempo 1</th>
						<th>Tiempo 2</th>
						<th>Tiempo 3</th>
						<th>IP o mensaje</th>
					</tr>
					<xsl:for-each select="ConfiguracionRed/Saltos/Salto">
						<tr>
							<td>
								<xsl:value-of select="position()"/>
							</td>
							<xsl:choose>
								<xsl:when test="contains(., 'Tiempo de espera')">
									<td>*</td>
									<td>*</td>
									<td>*</td>
									<td>
										<xsl:value-of select="normalize-space(.)"/>
									</td>
								</xsl:when>
								<xsl:otherwise>
									<!-- Eliminar número de salto: busca segunda palabra -->
									<xsl:variable name="sinNumero" select="normalize-space(substring-after(., ' '))" />
									<xsl:variable name="datos" select="substring-after($sinNumero, ' ')" />

									<!-- Tiempo 1 -->
									<td>
										<xsl:value-of select="concat(substring-before($datos, 'ms'), 'ms')" />
									</td>

									<!-- Tiempo 2 -->
									<xsl:variable name="resto1" select="normalize-space(substring-after($datos, 'ms'))" />
									<td>
										<xsl:value-of select="concat(substring-before($resto1, 'ms'), 'ms')" />
									</td>

									<!-- Tiempo 3 -->
									<xsl:variable name="resto2" select="normalize-space(substring-after($resto1, 'ms'))" />
									<td>
										<xsl:value-of select="concat(substring-before($resto2, 'ms'), 'ms')" />
									</td>

									<!-- IP o mensaje -->
									<xsl:variable name="ip" select="normalize-space(substring-after($resto2, 'ms'))" />
									<td>
										<xsl:value-of select="$ip"/>
									</td>
								</xsl:otherwise>
							</xsl:choose>
						</tr>
					</xsl:for-each>
				</table>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>