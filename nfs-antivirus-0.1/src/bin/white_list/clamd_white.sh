#!/bin/bash
#echo "$CLAM_VIRUSEVENT_FILENAME,$CLAM_VIRUSEVENT_VIRUSNAME,">>/usr/share/antivirus/log/clamd.log
white_list_dir=/usr/share/antivirus/config/white_list.txt
sed  -i  "/(deleted)/d" /usr/share/antivirus/log/clamd.log
#  >>/usr/share/antivirus/log/clamd.log
#        rm -f /usr/share/antivirus/log/clamd1.log
if [ -s /usr/share/antivirus/log/clamd.log ];then
cat /usr/share/antivirus/log/clamd.log | cut -d "," -f1  |while read line
do
#	sed -i '/(deleted)/d'  /usr/share/antivirus/log/clamd.log
    if [ -s $white_list_dir ];then
		cat $white_list_dir | cut -d "," -f1 | while read event
    		do
	        if  echo "$line" |grep -q "$event"   ;then
		echo $line >/usr/share/antivirus/log/b.txt
		sed  -i "/$(sed -r 's/\//\\\//g'  /usr/share/antivirus/log/b.txt)/d" /usr/share/antivirus/log/clamd.log 
		rm /usr/share/antivirus/log/b.txt
		fi	

		done
#	 done
    fi
    if [ -s $white_list_dir ];then
                cat $white_list_dir | cut -d ";" -f2 | while read event1
                do
                if [ "$line" = """$event1" ]   ;then
		echo "----------------------------------------------$event--------------------------"
                echo $line >/usr/share/antivirus/log/b.txt
                sed  -i "/$(sed -r 's/\//\\\//g'  /usr/share/antivirus/log/b.txt)/d" /usr/share/antivirus/log/clamd.log
                rm /usr/share/antivirus/log/b.txt
                fi
                done
    fi
#	rm -f /usr/share/antivirus/log/clamd1.log
        sed -i "/(deleted)/d"  /usr/share/antivirus/log/clamd.log
#fi
done
fi
