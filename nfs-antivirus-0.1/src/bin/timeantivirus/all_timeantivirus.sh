#!/bin/bash
user=$1
path="/tmp/."$user"_time_antivirus.txt"
echo "all-scan" >> $path 
chown $1:$1 $path
