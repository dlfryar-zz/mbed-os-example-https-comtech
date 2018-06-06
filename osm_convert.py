#!/usr/bin/python

import sys, argparse
import math

def main():
  parser = argparse.ArgumentParser(prog='convert.py', usage='python %(prog)s\
  --latitude 30.2436111 --longitude -97.847069 --zoom 15\n\
  see: https://wiki.openstreetmap.org/wiki/Slippy_map_tilenames#X_and_Y')

  parser.add_argument('-a', '--latitude', help='required e.g. 30.2436111')
  parser.add_argument('-o', '--longitude', help='required e.g. -97.847069')
  parser.add_argument('-z', '--zoom', help='optional 0-19 defaults to 15 if not specified')

  if len(sys.argv)==1:
    parser.print_help(sys.stderr)
    sys.exit(1)
  else:
    args = parser.parse_args(sys.argv[1:])

  if not args.latitude or not args.longitude:
    parser.print_help(sys.stderr)
    sys.exit(2)
  elif not args.zoom:
    zoom = 15
  elif int(args.zoom) < 0 or int(args.zoom) > 19:
    print '---> zoom must be between 0 and 19'
    print '       see: https://wiki.openstreetmap.org/wiki/Zoom_levels'
    print ''
    parser.print_help(sys.stderr)
    sys.exit(2)    

  lat = args.latitude
  lon = args.longitude
  zoom = args.zoom if args.zoom else 15
  
  xtile, ytile = deg2num(float(lat), float(lon), float(zoom))
  print 'lat is', lat, 'lon is', lon, 'zoom is', zoom
  print 'xtile is', xtile, 'ytile is', ytile, 'zoom is', zoom
 
def deg2num(lat_deg, lon_deg, zoom):
  lat_rad = math.radians(lat_deg)
  n = 2.0 ** zoom
  xtile = int((lon_deg + 180.0) / 360.0 * n)
  ytile = int((1.0 - math.log(math.tan(lat_rad) + (1 / math.cos(lat_rad))) / math.pi) / 2.0 * n)
  return (xtile, ytile)

if __name__ == "__main__":
  main()

