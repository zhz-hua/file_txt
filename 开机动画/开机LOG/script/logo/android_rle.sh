convert -depth 8 $1  rgb:logo.raw
rgb2565 -rle <logo.raw> initlogo.rle
