#!/bin/bash
TEMP_FILE=/usr/share/antivirus/log/log_temp
#NOW_TIME=`date +"%Y/%m/%d"`
NOW_TIME=`date  "+%Y/%m/%d" -d "1 days"`
echo $NOW_TIME
SYSTEM_NOW_TIME_30_AGAO=`date  "+%Y/%m/%d" -d "-30 days"`
echo $SYSTEM_NOW_TIME_30_AGAO


#awk -F,   '($1 >= "'"$SYSTEM_NOW_TIME_30_AGAO"'" ) && ($1  <= "'"$NOW_TIME"'" ) {print $0}' $1  >$TEMP_FILE
awk -F,   '($1 >= "'"$SYSTEM_NOW_TIME_30_AGAO"'" )  {print $0}' $1  >$TEMP_FILE
#awk -F,   '( $1> "'"$SYSTEM_NOW_TIME_30_AGAO"'" ) && ( $1< "'"$NOW_TIME"'" )'  $1  > $TEMP_FILE
#awk -F,   '( $1> "'"$SYSTEM_NOW_TIME_30_AGAO"'" ) && ( $1< "'"$NOW_TIME"'" )'  $1  
#mv $TEMP_FILE $1
if [  -s $TEMP_FILE ]; then
        mv $TEMP_FILE $1
fi
