#!/bin/bash

soft_file=/usr/lib/antivirus/AVL3bin/scan_result_deal.txt
repair_pkg_num=0
delete_file_num=0
apt_flag=0
isolation_label=file-into-deivide
isolation_flag=`grep -w $isolation_label /usr/share/antivirus/setting.txt  | cut -d "=" -f2`
#isolation_flag=`cat deal_style.txt | awk '{print $2}'`
#isolation_dir=/usr/share/antivirus/isolation/
isolation_dir=$1/
authority_file=
isolation_backup=/usr/lib/antivirus/AVL3bin/isolation_temp.txt
system_pkg_list=/usr/lib/antivirus/AVL3bin/list.txt
rep_tmp_file=/usr/lib/antivirus/AVL3bin/rep_temp.txt
rename_flag=
isolation_file_null=
#echo $isolation_flag

###set new name
pkg_new_name=
pkg_new_name_line=
#isolation_file_null_flag=`cat isolation_temp.txt |wc -l`
#if [[ ${isolation_file_null_flag} == "0" ]];then
#	pkg_new_name=0
#else
#	pkg_new_name=`tail -n 1 isolation_temp.txt | awk '{print $1}'`
#fi
###########
deal_date=
threat_file_name=

###0821
mkfifo pkg_num_pipe;
mkfifo del_num_pipe;
mkfifo apt_flag_pipe;
exec 3<>pkg_num_pipe;
exec 4<>del_num_pipe;
exec 5<>apt_flag_pipe;
echo $repair_pkg_num >&3
echo $delete_file_num >&4
echo $apt_flag >&5
###0821
if [ ! -d "$isolation_dir" ];then
        mkdir $isolation_dir
fi


cat $soft_file | cut -d ";" -f2 |while read threatFileName
do
echo -e "$threatFileName" > /home/nfs/11
exit_file="./exit_auto_deal.txt"
if [ -a "$exit_file" ];then
	flag=`cat $exit_file`
	if [ $flag="1" ];then
		rm -f $exit_file
		break
	fi
fi
	file_package_name=`dpkg -S "$threatFileName" |cut -f 1 -d :`
    if [[ $file_package_name == "" ]];then
	read -u4 delete_file_num
	if [ $isolation_flag = "1" -a -f "$threatFileName" ];then
		binary=`ls -ld "$threatFileName" |awk '{print $1}' | sed 's/^[a-zA-Z]//'|tr 'x|r|w' '1'|tr '-' '0'`
		authority_file=`echo "obase=8;ibase=2;$binary" |bc`
		pkg_new_name_line=`echo "$threatFileName" |base64 -i |wc -l`	
		pkg_new_name=`echo "$threatFileName" |base64 -i|awk 'NR%'${pkg_new_name_line}'{printf "%s",$0;next};1'`
		deal_date=`date "+%Y-%m-%d %H:%M:%S"`
#####set new name
		isolation_file_null=`cat $isolation_backup | wc -l `
		rename_flag=`grep "$isolation_dir$pkg_new_name,"$threatFileName",$authority_file," $isolation_backup`
		if [ -z "$rename_flag" -o $isolation_file_null = "0" ] ;then
			cp "$threatFileName" $isolation_dir$pkg_new_name
                	chmod 444 $isolation_dir$pkg_new_name
			#echo "$threatFileName"
			echo "$isolation_dir$pkg_new_name,$threatFileName,$authority_file,$deal_date,"  >> $isolation_backup
		fi
	fi
	rm -rf "$threatFileName"
#	read -u4 delete_file_num
	delete_file_num=`expr $delete_file_num + 1 `
	echo $delete_file_num >&4
#	let delete_file_num=$delete_file_num +1	
    else
	line=`grep -w "${file_package_name}" $system_pkg_list | wc -l`
	if [[ $line == "0" ]];then 
		read -u4 delete_file_num
		if [ $isolation_flag = "1" -a -f "$threatFileName" ];then
                	binary=`ls -ld "$threatFileName" |awk '{print $1}' | sed 's/^[a-zA-Z]//'|tr 'x|r|w' '1'|tr '-' '0'`
               	 	authority_file=`echo "obase=8;ibase=2;$binary" |bc`
			pkg_new_name_line=`echo "$threatFileName" |base64 -i |wc -l`
	                pkg_new_name=`echo "$threatFileName" |base64 -i|awk 'NR%'${pkg_new_name_line}'{printf "%s",$0;next};1'`
                	deal_date=`date "+%Y-%m-%d %H:%M:%S"`
#####set new name
			isolation_file_null=`cat $isolation_backup | wc -l `
			rename_flag=`grep "$isolation_dir$pkg_new_name,"$threatFileName",$authority_file," $$isolation_backup`
                	if [ -z "$rename_flag" -o $isolation_file_null = "0" ];then
				cp "$threatFileName" $isolation_dir$pkg_new_name
                        	chmod 444 $isolation_dir$pkg_new_name
				#echo "$threatFileName"
                        	echo "$isolation_dir$pkg_new_name,$threatFileName,$authority_file,$deal_date,"  >> $isolation_backup
                	fi
        	fi
		#########0821-end
		rm -rf "$threatFileName"
		delete_file_num=`expr $delete_file_num + 1`
		echo $delete_file_num >&4
#		let delete_file_num=$delete_file_num +1
####        	echo "delete num is ${delete_file_num}"
	else

            grep -w  "${file_package_name}" $system_pkg_list | while read event
            do
		if [ -f $rep_tmp_file ];then
			rep_temp_file_line=`grep -w "$file_package_name\s" $rep_tmp_file | wc -l`
		fi

	        if [ ! -f $rep_tmp_file ] || [ ${event} = ${file_package_name} -a $rep_temp_file_line = "0" ]
		then
		    apt-get --reinstall install $file_package_name --assume-yes --force-yes  >/dev/null 2>&1
#			let repair_pkg_num=$repair_pkg_num + 1
		    if [ $? == 0 ];then
			echo "$file_package_name " >> $rep_tmp_file 2>&1
			read -u3 repair_pkg_num
			read -u5 apt_flag
			repair_pkg_num=`expr $repair_pkg_num + 1`
			apt_flag=0			
			echo $repair_pkg_num >&3
			echo $apt_flag >&5

####			echo "repair num is $repair_pkg_num"
######		    else
# 			echo "OK"
#####			read -u5 apt_flag
#####			if [[ ${apt_flag} == "0" ]];then
#####			
#####         			zenity --info --no-wrap  \
######                			--text="请检查下网络或者apt工具是否安装正确"
#####				apt_flag=1
#####				echo $apt_flag >&5
#####			fi
			 
	    fi
#		    repair_pkg_num=$(expr $repair_pkg_num + 1)
#	        else
#		    echo $event
#		    rm -rf $threatFileName
#		    echo $threatFileName

#		    echo "delete file"
	        fi
	
#    fileLine=`expr $fileLine + 1`
#    grep "${threatFileName}" white_list.txt |  while read event #|cut -d " " -f1 | while read event
#    do
#        findWhiteListLine=`expr $findWhiteListLine + 1`
#        echo $findWhiteListLine
        #sed -i "$fileLine d" scan_results.txt
#	echo " final ${delete_file_num} ${repair_pkg_num}"
            done
        fi
    fi
#echo " ${delete_file_num} ${repair_pkg_num}"
done
read -u3 repair_pkg_num
read -u4 delete_file_num
#echo " out : delete is $delete_file_num  repair is $repair_pkg_num"
echo "$delete_file_num $repair_pkg_num" >> $rep_tmp_file 2>&1
echo "$delete_file_num $repair_pkg_num"
rm -rf $rep_tmp_file
rm  pkg_num_pipe;
rm  del_num_pipe;
rm apt_flag_pipe;
exec 3>&-
exec 4>&-
exec 5>&-
rm -f $soft_file