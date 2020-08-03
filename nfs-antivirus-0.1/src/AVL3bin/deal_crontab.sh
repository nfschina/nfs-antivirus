#!/bin/bash
cat /usr/share/antivirus/temp/setcron.txt >> /var/spool/cron/root
rm -rf /usr/share/antivirus/temp/setcron.txt
