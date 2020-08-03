#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


void main()
{
    if(access("/tmp/cancelscan.txt",F_OK ) ==0 )
    {
        system("sed -i '/timeantivirus/d'  /var/spool/cron/root 2>&1 ");
	system("rm -rf /tmp/cancelscan.txt");
    }
    else
    {
        if(access("/tmp/setupdatecron.txt", F_OK)==0)
        {
            system("sed -i '/upgrade/d'  /var/spool/cron/root 2>&1 ");
            system("cat /tmp/setupdatecron.txt >> /var/spool/cron/root");
        }

    }
    if(access("/tmp/cancelupdate.txt",F_OK) == 0)
    {
       system("sed -i '/upgrade/d'  /var/spool/cron/root 2>&1 ");
       system("rm -rf /tmp/cancelupdate.txt");
    }
    if(access("/tmp/setcron.txt", F_OK)==0) 
    {
        system("sed -i '/timeantivirus/d'  /var/spool/cron/root 2>&1 ");
        system("cat /tmp/setcron.txt >> /var/spool/cron/root");
    }
}
