#!/bin/bash
ftp -n<<!
open $1
user nfs 123*abc
binary
prompt
cd AVLLIB/$2
lcd $3
mget $4
close
bye
!
