convert $1 linuxlogo.png
pngtopnm linuxlogo.png > linuxlogo.pnm
pnmquant 224 linuxlogo.pnm > linuxlogo224.pnm
pnmtoplainpnm linuxlogo224.pnm > logo_linux_clut224.ppm
rm -rf ./linuxlogo.pnm  ./linuxlogo224.pnm ./linuxlogo.png
