#!/bin/sh 

myFile="/var /log/httpd/access.log" 
 if [ ! -e "$myFile" ]; then 
     touch "$myFile" 
 fi 

-e和-f的区别是，-f代表常规文件（regular file），-e代表所有任何类型文件

参考如下：

-e filename 如果 filename存在，则为真
-d filename 如果 filename为目录，则为真 
-f filename 如果 filename为常规文件，则为真
-L filename 如果 filename为符号链接，则为真
-r filename 如果 filename可读，则为真 
-w filename 如果 filename可写，则为真 
-x filename 如果 filename可执行，则为真
-s filename 如果文件长度不为0，则为真
-h filename 如果文件是软链接，则为真
# remove previous repository
[ -d imx6-android-4-4-3 ] && rm -rf imx6-android-4-4-3
