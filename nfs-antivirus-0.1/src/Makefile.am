CFLAGS2 = `pkg-config --cflags --libs gtk+-3.0`
CUR_DIR=${PWD}
#LIBS2 = -I$(top_srcdir) -I$(top_srcdir)/shared -I$(top_srcdir)/libclamav
LIBS2 = $(top_srcdir)/libclamav/.libs/*.so*
VPATH=.
srcPath=.
DESTDIR=/usr/local
#INSTDIR=/usr/lib/antivirus

all:${srcPath}/bin/upgrade/upgrade ${srcPath}/bin/auto_defense/real_protect_dialog  ${srcPath}/bin/auto_handle/auto_deal_warn_dialog NFS-antivirus

${srcPath}/bin/upgrade/upgrade:   ${srcPath}/upgrade/upgrade.o
	$(CC) -o   ${srcPath}/bin/upgrade/upgrade   ${srcPath}/upgrade/upgrade.o

${srcPath}/upgrade/upgrade.o: ${srcPath}/upgrade/upgrade.c  ${srcPath}/upgrade/upgrade.h
	$(CC) -o   ${srcPath}/upgrade/upgrade.o -c  ${srcPath}/upgrade/upgrade.c

${srcPath}/bin/auto_defense/real_protect_dialog:   ${srcPath}/real_protect/real_protect_dialog.o
	$(CC) -o   ${srcPath}/bin/auto_defense/real_protect_dialog ${srcPath}/real_protect/real_protect_dialog.o $(CFLAGS2)

${srcPath}/real_protect/real_protect_dialog.o: ${srcPath}/real_protect/real_protect_dialog.c  
	$(CC) -o   ${srcPath}/real_protect/real_protect_dialog.o -c  ${srcPath}/real_protect/real_protect_dialog.c $(CFLAGS2)


${srcPath}/bin/auto_handle/auto_deal_warn_dialog:   ${srcPath}/auto_deal_warn_dialog.o
	$(CC) -o   ${srcPath}/bin/auto_handle/auto_deal_warn_dialog ${srcPath}/auto_deal_warn_dialog.o $(CFLAGS2)

${srcPath}/auto_deal_warn_dialog.o: ${srcPath}/auto_deal_warn_dialog.c  
	$(CC) -o   ${srcPath}/auto_deal_warn_dialog.o -c  ${srcPath}/auto_deal_warn_dialog.c $(CFLAGS2)

NFS-antivirus:antivirus.o result.o real_protect_result_page.o resultcallbackFunc.o  result_detail.o  scan.o refresh.o sharememory.o writelog.o  filechooser.o  writeresult.o antivirus.mo upgradeFunction.o auto_handle.o ${srcPath}/upgrade/upgradeself.o ${srcPath}/detect_usb/detect_usb.o function.o ${srcPath}/deal/tray_handle.o ${srcPath}/deal/home_handle.o ${srcPath}/page/upgrade-page.o  ${srcPath}/page/log-page.o ${srcPath}/page/scanprocess-page.o  ${srcPath}/page/about-page.o  ${srcPath}/page/home-page.o ${srcPath}/deal/log_handle.o ${srcPath}/deal/setting_handler.o ${srcPath}/page/setting-page.o ${srcPath}/page/isolation-recovery-page.o ${srcPath}/deal/isolation_handle.o
	$(CC) -o  ${srcPath}/bin/NFS-antivirus antivirus.o real_protect_result_page.o result.o resultcallbackFunc.o result_detail.o scan.o refresh.o sharememory.o  writelog.o  filechooser.o  writeresult.o upgradeFunction.o auto_handle.o ${srcPath}/upgrade/upgradeself.o ${srcPath}/detect_usb/detect_usb.o function.o ${srcPath}/page/log-page.o ${srcPath}/deal/log_handle.o  ${srcPath}/deal/tray_handle.o ${srcPath}/deal/home_handle.o ${srcPath}/page/upgrade-page.o ${srcPath}/page/scanprocess-page.o ${srcPath}/page/about-page.o  ${srcPath}/page/home-page.o ${srcPath}/deal/setting_handler.o  ${srcPath}/page/setting-page.o  ${srcPath}/page/isolation-recovery-page.o ${srcPath}/deal/isolation_handle.o -ldl -lpthread $(LIBS2) $(CFLAGS2)
 
antivirus.o:antivirus.c antivirus.h
	$(CC) -c ${srcPath}/antivirus.c $(LIBS2) $(CFLAGS2)
scan.o:scan.c scan.h  
	$(CC) -c ${srcPath}/scan.c $(CFLAGS2)
result.o:result.c result.h
	$(CC) -c ${srcPath}/result.c $(CFLAGS2)
real_protect_result_page.o:real_protect_result_page.c real_protect_result_page.h
	$(CC) -c ${srcPath}/real_protect_result_page.c  $(CFLAGS2)
resultcallbackFunc.o:resultcallbackFunc.c resultcallbackFunc.h
	$(CC) -c ${srcPath}/resultcallbackFunc.c $(CFLAGS2)
result_detail.o:result_detail.c result_detail.h
	$(CC) -c ${srcPath}/result_detail.c $(CFLAGS2)
refresh.o:refresh.c refresh.h
	$(CC) -c ${srcPath}/refresh.c $(CFLAGS2)
sharememory.o:sharememory.c sharememory.h
	$(CC) -c ${srcPath}/sharememory.c $(CFLAGS2)
writelog.o:writelog.c writelog.h
	$(CC) -c ${srcPath}/writelog.c $(CFLAGS2)
filechooser.o:filechooser.c filechooser.h
	$(CC) -c ${srcPath}/filechooser.c $(CFLAGS2)
writeresult.o:writeresult.c writeresult.h
	$(CC) -c ${srcPath}/writeresult.c $(CFLAGS2)
auto_handle.o:auto_handle.c auto_handle.h
	$(CC) -c ${srcPath}/auto_handle.c $(CFLAGS2)
${srcPath}/page/isolation-recovery-page.o:${srcPath}/page/isolation-recovery-page.c ${srcPath}/page/isolation-recovery-page.h
	$(CC)  -o  ${srcPath}/page/isolation-recovery-page.o -c ${srcPath}/page/isolation-recovery-page.c $(CFLAGS2)
antivirus.mo:po/antivirus.po
	msgfmt ${srcPath}/po/antivirus.po -o ${srcPath}/po/antivirus.mo
upgradeFunction.o:${srcPath}/upgradeFunction.c ${srcPath}/upgradeFunction.h
	$(CC) -c ${srcPath}/upgradeFunction.c $(CFLAGS2)
function.o:${srcPath}/function.c ${srcPath}/function.h
	$(CC) -c ${srcPath}/function.c  $(LIBS2) $(CFLAGS2)
${srcPath}/upgrade/upgradeself.o: ${srcPath}/upgrade/upgradeself.c  ${srcPath}/upgrade/upgradeself.h
	$(CC) -o  ${srcPath}/upgrade/upgradeself.o -c   ${srcPath}/upgrade/upgradeself.c  $(CFLAGS2)
${srcPath}/deal/tray_handle.o:${srcPath}/deal/tray_handle.c ${srcPath}/deal/tray_handle.h
	$(CC) -o  ${srcPath}/deal/tray_handle.o -c   ${srcPath}/deal/tray_handle.c  $(CFLAGS2)
${srcPath}/deal/log_handle.o:${srcPath}/deal/log_handle.c ${srcPath}/deal/log_handle.h
	$(CC) -o  ${srcPath}/deal/log_handle.o -c   ${srcPath}/deal/log_handle.c  $(CFLAGS2)
${srcPath}/deal/home_handle.o:${srcPath}/deal/home_handle.c ${srcPath}/deal/home_handle.h
	$(CC) -o  ${srcPath}/deal/home_handle.o -c   ${srcPath}/deal/home_handle.c  $(CFLAGS2)
${srcPath}/page/upgrade-page.o:${srcPath}/page/upgrade-page.c ${srcPath}/page/upgrade-page.h
	$(CC) -o  ${srcPath}/page/upgrade-page.o -c   ${srcPath}/page/upgrade-page.c  $(CFLAGS2)
${srcPath}/page/scanprocess-page.o:${srcPath}/page/scanprocess-page.c ${srcPath}/page/scanprocess-page.h
	$(CC) -o  ${srcPath}/page/scanprocess-page.o -c   ${srcPath}/page/scanprocess-page.c  $(CFLAGS2)
${srcPath}/page/about-page.o:${srcPath}/page/about-page.c ${srcPath}/page/about-page.h
	$(CC) -o  ${srcPath}/page/about-page.o -c   ${srcPath}/page/about-page.c  $(CFLAGS2)
${srcPath}/deal/setting_handler.o: ${srcPath}/deal/setting_handler.c  ${srcPath}/deal/setting_handler.h
	$(CC) -o  ${srcPath}/deal/setting_handler.o -c   ${srcPath}/deal/setting_handler.c  $(CFLAGS2)
${srcPath}/page/setting-page.o: ${srcPath}/page/setting-page.c  ${srcPath}/page/setting-page.h
	$(CC) -o  ${srcPath}/page/setting-page.o -c   ${srcPath}/page/setting-page.c  $(CFLAGS2)
${srcPath}/page/home-page.o:${srcPath}/page/home-page.c ${srcPath}/page/home-page.h
	$(CC) -o  ${srcPath}/page/home-page.o -c   ${srcPath}/page/home-page.c  $(CFLAGS2)
${srcPath}/detect_usb/detect_usb.o:${srcPath}/detect_usb/detect_usb.c ${srcPath}/detect_usb/detect_usb.h
	$(CC) -o  ${srcPath}/detect_usb/detect_usb.o -c ${srcPath}/detect_usb/detect_usb.c $(CFLAGS2)
${srcPath}/page/log-page.o:${srcPath}/page/log-page.c ${srcPath}/page/log-page.h
	$(CC) -o  ${srcPath}/page/log-page.o -c   ${srcPath}/page/log-page.c  $(CFLAGS2)
${srcPath}/deal/isolation_handle.o:${srcPath}/deal/isolation_handle.c ${srcPath}/deal/isolation_handle.h
	$(CC) -o  ${srcPath}/deal/isolation_handle.o -c   ${srcPath}/deal/isolation_handle.c  $(CFLAGS2)


install:
      
clean:
	rm -fr antivirus.o result.o real_protect_result_page.o result_detail.o scan.o refresh.o sharememory.o writelog.o   filechooser.o  writeresult.o antivirus.mo upgradeFunction.o auto_handle.o resultcallbackFunc.o ${srcPath}/upgrade/upgradeself.o ${srcPath}/detect_usb/detect_usb.o function.o ${srcPath}/deal/tray_handle.o ${srcPath}/deal/home_handle.o ${srcPath}/page/upgrade-page.o ${srcPath}/page/scanprocess-page.o  ${srcPath}/page/about-page.o  ${srcPath}/page/home-page.o ${srcPath}/deal/log_handle.o  ${srcPath}/deal/setting_handler.o ${srcPath}/page/setting-page.o ${srcPath}/page/log-page.o ${srcPath}/page/isolation-recovery-page.o ${srcPath}/deal/isolation_handle.o ${srcPath}/real_protect/real_protect_dialog.o ${srcPath}/auto_defense/real_protect_dialog  ${srcPath}/upgrade/upgrade.o ${srcPath}/auto_deal_warn_dialog.o ${srcPath}/bin/auto_handle/auto_deal_warn_dialog 
