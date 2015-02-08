#! /usr/bin/python
import sys, os
import xml.etree.ElementTree as ElementTree

if len(sys.argv) != 3:
	print "USAGE: convertmap <tmx> <scn>"
	print "Converts a TMX map into an SCN file for Todd."
	sys.exit(1)

# This must match the list in SceneView.cpp, except the active tiles
# should actually be mapped to their icons (e.g. the save crystal)
tileToSprite = {
	1:	1,
	2:	2,
	3:	3,
	4:	4,
	5:	6,
	6:	7,
	7:	8,
	8:	11,
	9:	11,
	10:	13,
	11:	33,
	12:	29,
	13:	30,
	14:	12,
	15:	31,
	16:	32,
}

spriteToTile = {}
for key, value in tileToSprite.items():
	spriteToTile[value] = key
spriteToTile[0] = 0

tree = ElementTree.parse(sys.argv[1])
root = tree.getroot()

if root.tag != "map":
	print "invalid TMX file"
	sys.exit(1)

width = int(root.attrib["width"])
height = int(root.attrib["height"])

#print "Creating a %dx%d scene file" % (width, height)

layers = {}

allowedNames = ("bg", "hard", "overlay")

nextFreeSymbol = 33
tileSymbols = {}
escape = "#."
objects = []

def getTileSymbol(tileID):
	global nextFreeSymbol
	if not tileSymbols.has_key(tileID):
		c = chr(nextFreeSymbol)
		nextFreeSymbol += 1
		while c in escape:
			c = chr(nextFreeSymbol)
			nextFreeSymbol += 1
		tileSymbols[tileID] = c
	return tileSymbols[tileID]

def parseLayer(layer):
	name = layer.attrib["name"]
	if name not in allowedNames:
		print "unexpected layer: %s" % name
		sys.exit(1)
	if layers.has_key(name):
		print "duplicate layer: %s" % name
		sys.exit(1)
	dataTag = layer[0]
	if dataTag.tag != "data":
		print "invalid <layer> tag (%s), doesn't have <data>" % name
		sys.exit(1)
	if dataTag.attrib["encoding"] != "csv":
		print "<data> encoding is not csv for layer %s" % name
		sys.exit(1)

	csv = dataTag.text
	outdata = ""
	for line in csv.splitlines():
		for tile in line.split(","):
			if tile != "":
				spriteID = int(tile)
				if spriteID != 0:
					spriteID -= 1
				tileID = spriteToTile[spriteID]
				outdata += getTileSymbol(tileID)
		outdata += "\n"

	while outdata.startswith("\n"):
		outdata = outdata[1:]
	layers[name] = outdata

def praseObjects(objectgroup):
	for child in objectgroup:
		if child.tag == "object":
			kind = child.attrib["type"]
			x = int(child.attrib["x"])/48
			y = int(child.attrib["y"])/48
			props = {}
			for prop in child[0]:
				props[prop.attrib["name"]] = prop.attrib["value"]

			if kind == "Warp":
				objects.append(".warp %d,%d:%s" % (x, y, props.get("target", "0")))

for child in root:
	if child.tag == "layer":
		parseLayer(child)
	elif child.tag == "objectgroup":
		praseObjects(child)

if len(layers) != 3:
	print "Not all layers present! (bg, hard, overlay)"
	sys.exit(1)

#print "Writing tilemap..."
f = open(sys.argv[2], "wb")
for key, value in tileSymbols.items():
	f.write(".tile %s%d\n" % (value, key))
f.write("\n")

f.write("\n".join(objects))
f.write("\n\n")

for key, value in layers.items():
	#print "Writing layer %s..." % key
	f.write(".%s\n%s\n" % (key, value))

f.close()