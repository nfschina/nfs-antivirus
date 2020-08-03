#!/bin/bash
user=$1
path="/tmp/."$user"_time_antivirus.txt"
echo "quick-scan" >> $path
chown $1:$1 $path
