#!/bin/bash

#echo $1
encrypted_file=/usr/lib/antivirus/AVL3bin/isolation_temp.txt
isolation_dir=/usr/share/antivirus/isolation/
find_var=$1","

#echo $find_var

dest_row=`grep "$find_var" $encrypted_file`
line_num=`grep -n "$find_var" $encrypted_file | cut -d ":" -f 1`
echo $line_num

echo "$dest_row"

isolation_file_name=`echo "$dest_row" | cut -d "," -f 1`

echo $isolation_file_name
if [ ! -d $2 ];then
	mkdir $2;
fi
real_file_name=`echo "$dest_row" | cut -d "," -f 2`
#real_file_name=${real_file_name_t//\ /\\\ }

real_name=`echo ${real_file_name##*/}`
echo $real_file_name
real_auth=`echo "$dest_row" | cut -d "," -f 3`
echo $real_auth
if [ -f $isolation_file_name ];then
	chmod $real_auth "$isolation_file_name"
	if [[ $2 == "" ]];then
		mv $isolation_file_name "$real_file_name"
	else
		mv $isolation_file_name $2/"$real_name"
	fi
	echo "move"
fi

sed -i ${line_num}'d' ${encrypted_file} 2>&1

