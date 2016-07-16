#!/bin/sh
#install Netpbm first
#使用方法： （脚本名） （ 待处理的JPG图片名） （输出文件名）
    jpegtopnm $1 | ppmquant 31 | ppmtobmp -bpp 8 > $2
