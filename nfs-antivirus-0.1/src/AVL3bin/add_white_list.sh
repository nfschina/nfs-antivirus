#!/bin/bash
findWhiteListLine=0
cat ./white_list.txt |cut -d "," -f1 |  while read line
do
        if [ "${line}" == "${1}" ];
        then
            findWhiteListLine=`expr $findWhiteListLine + 1`
            echo $findWhiteListLine
            break
        
        fi
done
