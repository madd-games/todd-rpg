#! /usr/bin/python
import sys, os

srcdir = sys.argv[1]

for name in os.listdir("%s/maps" % srcdir):
	if name.endswith(".tmx"):
		mapname = name[:-4]
		print ">Make map %s" % mapname
		if sys.platform.startswith("win"):
			os.system("convertmap.py \"%s/maps/%s.tmx\" \"%s/data/scenes/%s.scn\"" % (srcdir, mapname, srcdir, mapname))
		else:
			os.system("python \"%s/convertmap.py\" \"%s/maps/%s.tmx\" \"%s/data/scenes/%s.scn\"" % (srcdir, srcdir, mapname, srcdir, mapname))
