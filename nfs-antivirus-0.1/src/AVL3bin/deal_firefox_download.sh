#!/bin/bash

ls  /tmp/firefox_download*|while read line
do
    down_info=`cat $line`
    echo $down_info >> /tmp/.firefox_info
done
rm -rf /tmp/firefox_download*
