#!/bin/bash
#username=`who am i|cut -d " " -f1`
#echo "enter aaaa">>/home/sff/test.txt
auto_defense_user=/usr/share/antivirus/auto_defense/user.txt
find /home -name .config > $auto_defense_user
cat $auto_defense_user  | while read username
do
#username=$2
auto_start_dir=$username"/autostart"
old_info=$username"/autostart/cdos-antivirus.desktop"
new_info=$username"/autostart/cdos-antivirus.desktop-autostart"
source_info="/usr/share/antivirus/auto_defense/cdos-antivirus.desktop-autostart"
usr_auth=`echo $username | cut -d '/' -f3`
if [ $1 = "1" ];
then 
    if [ ! -f $old_info ];
    then 
        echo $source_info
	if [ ! -d $auto_start_dir ];then
                mkdir $auto_start_dir
		chown $usr_auth:$usr_auth $auto_start_dir
        fi
        cp $source_info $old_info
    fi
fi
if [ $1 = "0" ];
then
    echo $new_info
    mv $old_info $new_info
fi
done
