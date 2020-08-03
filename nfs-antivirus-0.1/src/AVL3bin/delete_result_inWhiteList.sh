#!/bin/bash
path=${1%/*}
findWhiteListLine=0
fileLength=`expr length $1`
pathLength=`expr length $path`
cat ./white_list.txt |cut -d " " -f1 |  while read line
do
    whitelistLength=`expr length $line`
    if  [ $fileLength -ge $whitelistLength ];
    then
        if [ "${line}" = "${1}" ];
        then
            findWhiteListLine=`expr $findWhiteListLine + 1`
            echo $findWhiteListLine
            break
        
        elif [ $pathLength -ge $whitelistLength ];
        then
            if [ $pathLength -eq $whitelistLength ];
            then
	        if [ "${line}" == "${path}" ];
                then 
                    findWhiteListLine=`expr $findWhiteListLine + 1`
                    echo $findWhiteListLine
                    break
                fi
            elif [ ${path:$whitelistLength:1} == "/" ];
            then
                if [ "${path:0:$whitelistLength}" == "${line}" ];
                then
                    findWhiteListLine=`expr $findWhiteListLine + 1`
                    echo $findWhiteListLine
                    break
                fi

            fi
            
        fi
    fi
done
