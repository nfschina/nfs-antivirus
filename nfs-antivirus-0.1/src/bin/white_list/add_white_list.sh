#!/bin/bash
findWhiteListLine=0
white_list_dir=/usr/share/antivirus/config/white_list.txt
cat $white_list_dir |cut -d "," -f1 |  while read line
do
        if [ "${line}" == "${1}" ];
        then
            findWhiteListLine=`expr $findWhiteListLine + 1`
            echo $findWhiteListLine
            break
        
        fi
done
