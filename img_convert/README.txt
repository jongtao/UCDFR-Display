NAME
	png2hex - converts png to C style hex constant

SYNOPSIS
	png2hex [file] [pathname]

DESCRIPTION
png2hex converts a png image into a C style hex, comma separated C constant.
A copy of the png is written to the present working directory for verification
that the PNG file has been successfully read before converting to hex.

png2hex is intended for converting images into a format that is to be written
into an embedded system's memory via C. In practical use, one would convert the
png image, then copy the hex ouput into a C program as a C constant.

Png images should have horizontal dimension in multiples of 8 pixels.
