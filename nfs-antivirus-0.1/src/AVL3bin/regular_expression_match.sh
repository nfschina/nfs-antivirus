#!/bin/bash
findWhiteListLine=0
cat ./white_list.txt | while read line
do
    fileName=`echo $line | cut -d "," -f1 ` 
    fileType=`echo $line | cut -d "," -f2 `
    if [[ "2" != "$fileType" ]];then
    	continue
    fi
    if [[ "*" == "${fileName:0:1}" ]];then 
        newRegular="[a-zA-Z0-9!@#$%^&*()-]{0,}""${fileName:1}"
    else
	newRegular=$fileName
    fi 
    if [[ $1 =~ $newRegular ]];then
        findWhiteListLine=`expr $findWhiteListLine + 1`
        echo $findWhiteListLine
        break;
    fi 
done
