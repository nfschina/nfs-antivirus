%global debug_package %{nil}
Name:		nfs-antivirus		
Version:	0.1	
Release:	1%{?dist}
Summary:	antivirus of NFSchina

Group:		Application
License:	GPL
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires:	clamav-devel
BuildRequires:	clamav-lib
BuildRequires:  gtk3-devel
Requires: clamav-filesystem clamav libprelude clamav-lib clamav-data inotify-tools

%description
antivirus of NFSchina

%prep
%setup -q


%build
make


%install
mkdir -p 777 %{buildroot}/etc/init.d/
mkdir -p 777 %{buildroot}/etc/rc2.d/
mkdir -p 777 %{buildroot}/usr/bin/
mkdir -p 777 %{buildroot}/usr/libexec/
mkdir -p 777 %{buildroot}/usr/lib/systemd/system/
mkdir -p 777 %{buildroot}/usr/share/applications/
mkdir -p 777 %{buildroot}/usr/share/pixmaps/
mkdir -p 777 %{buildroot}/usr/share/locale/zh_CN/LC_MESSAGES/
mkdir -p 777 %{buildroot}/usr/share/antivirus/log/
mkdir -p 777 %{buildroot}/usr/share/antivirus/log/
mkdir -p 777 %{buildroot}/usr/share/antivirus/temp/
mkdir -p 777 %{buildroot}/usr/share/antivirus/images/
mkdir -p 777 %{buildroot}/usr/share/antivirus/config/
mkdir -p 777 %{buildroot}/usr/share/antivirus/css/
mkdir -p 777 %{buildroot}/usr/share/antivirus/bin/auto_defense/
mkdir -p 777 %{buildroot}/usr/share/antivirus/bin/timeantivirus/
mkdir -p 777 %{buildroot}/usr/share/antivirus/bin/upgrade/
mkdir -p 777 %{buildroot}/usr/share/antivirus/bin/white_list/
mkdir -p 777 %{buildroot}/usr/share/antivirus/auto_defense/
mkdir -p 777 %{buildroot}/usr/share/antivirus/bin/auto_handle/



install -m 0777 src/po/antivirus.mo %{buildroot}/usr/share/locale/zh_CN/LC_MESSAGES/
install -m 0777 src/images/*  %{buildroot}/usr/share/antivirus/images/
install -m 0777 src/images/NFSantivirus.png  %{buildroot}/usr/share/pixmaps/
install -m 0777 src/config/version.txt  %{buildroot}/usr/share/antivirus/config/
install -m 0777 src/AVL3bin/antivirus %{buildroot}/usr/share/antivirus/bin/NFS-antivirus
install -m 0777 src/bin/auto_defense/real_protect_dialog  %{buildroot}/usr/share/antivirus/bin/auto_defense/
install -m 0777 src/bin/antivirus %{buildroot}/usr/bin/
install -m 0777 src/bin/autostart-antivirus %{buildroot}/usr/bin/
install -m 0644 src/bin/antivirusCron.service  %{buildroot}/usr/lib/systemd/system/
install -m 0644 src/bin/detect_usb/detect_usb.service  %{buildroot}/usr/lib/systemd/system/
install -m 0777 src/desktop/cdos-antivirus.desktop %{buildroot}/usr/share/applications/
install -m 0777 src/auto_defense/cdos-antivirus.desktop-autostart  %{buildroot}/usr/share/antivirus/auto_defense/
install -m 0777 src/bin/upgrade/upgrade %{buildroot}/usr/share/antivirus/bin/upgrade/
install -m 0777 src/bin/upgrade/start.sh %{buildroot}/usr/share/antivirus/bin/upgrade/
install -m 0777 src/auto_defense/detect_usb.desktop %{buildroot}/usr/share/antivirus/bin/auto_defense/
install -m 0777 src/bin/detect_usb/detect_usb %{buildroot}/usr/bin/
install -m 0777 src/bin/detect_usb/autorun-detectUsb %{buildroot}/usr/bin/
install -m 0777 src/bin/detect_usb/autorunDetectUsb %{buildroot}/usr/bin/
install -m 0777 src/bin/auto_defense/deal_firefox_download.sh %{buildroot}/usr/share/antivirus/bin/auto_defense/
install -m 0777 src/bin/timeantivirus/all_timeantivirus.sh %{buildroot}/usr/share/antivirus/bin/timeantivirus/
install -m 0777 src/bin/timeantivirus/quick_timeantivirus.sh %{buildroot}/usr/share/antivirus/bin/timeantivirus/
install -m 0777 src/bin/auto_handle/auto_handle.sh %{buildroot}/usr/share/antivirus/bin/auto_handle/
install -m 0777 src/bin/auto_handle/recovery.sh %{buildroot}/usr/share/antivirus/bin/auto_handle/
install -m 0777 src/config/sys_package_list %{buildroot}/usr/share/antivirus/config/
install -m 0777 src/bin/auto_defense/boot_start.sh  %{buildroot}/usr/share/antivirus/bin/auto_defense/
install -m 0755 src/bin/timeantivirus/antivirusCron %{buildroot}/usr/libexec/
install -m 0755 src/bin/detect_usb/detect_usb %{buildroot}/usr/libexec/

install -m 0777 src/bin/white_list/add_white_list.sh %{buildroot}/usr/share/antivirus/bin/white_list/
install -m 0777 src/bin/white_list/delete_result_inWhiteList.sh %{buildroot}/usr/share/antivirus/bin/white_list/
install -m 0777 src/bin/white_list/regular_expression_match.sh %{buildroot}/usr/share/antivirus/bin/white_list/
#install -m 0777 src/bin/antifeedback  %{buildroot}/usr/share/antivirus/bin/
#install -m 0777 cosfeedback/zh_CN.qm  %{buildroot}/usr/share/antivirus/bin/
install -m 0777 src/css/*  %{buildroot}/usr/share/antivirus/css/
install -m 0777 src/config/antivirus-use.conf %{buildroot}/usr/share/antivirus/config/
install -m 0777 src/config/antivirus-un-use.conf %{buildroot}/usr/share/antivirus/config/
install -m 0777 src/bin/white_list/clamd_white.sh %{buildroot}/usr/share/antivirus/bin/white_list/
install -m 0777 src/bin/delete_log_30daysago.sh %{buildroot}/usr/share/antivirus/bin/
#install -m 0777 src/bin/auto_handle/auto_deal_warn_dialog %{buildroot}/usr/share/antivirus/bin/auto_handle/

%post
find /home -type d -name .config > /usr/share/antivirus/temp/user.txt
#sed -i '/\/home\/\.config/d' /usr/share/antivirus/temp/user.txt
cat /usr/share/antivirus/temp/user.txt  | while read myline
do
    usr_auth=`echo $myline | cut -d '/' -f3`
    if [ -d $myline/autostart ]
    then
        rm -f $myline/autostart/cdos-antivirus.desktop*
    else
        mkdir -p  $myline/autostart
    fi
done
rm -f /root/.config/autostart/cdos-antivirus.desktop*
rm -f /root/.config/autostart/detect_usb.desktop
/usr/bin/autorun-detectUsb
sudo_line=`grep -w "ALL ALL=(ALL) NOPASSWD:/usr/share/antivirus/bin/NFS-antivirus" /etc/sudoers | wc -l`
if [ $sudo_line = "0" ];then
    echo "ALL ALL=(ALL) NOPASSWD:/usr/share/antivirus/bin/NFS-antivirus" >> /etc/sudoers
fi
if [ -f "/var/spool/cron/crontabs/root" ];then
    sed -i '/antivirus/d' /var/spool/cron/crontabs/root
    echo "0 12 * * * sh /usr/share/antivirus/bin/upgrade/start.sh nfs" >> /var/spool/cron/crontabs/root
fi
if [ ! -f "/usr/share/antivirus/log/avlUpgradeDate.txt" ];then
        date "+%Y/%m/%d %H:%M:%S" > /usr/share/antivirus/log/avlUpgradeDate.txt
fi
chmod -R 777 /usr/share/antivirus/
chkconfig detect_usb on

%files
/usr/bin/*
/usr/share/antivirus/*
/usr/share/applications/*
/usr/share/locale/zh_CN/LC_MESSAGES/antivirus.mo
/usr/share/pixmaps/NFSantivirus.png
/usr/lib/systemd/system/antivirusCron.service
/usr/libexec/antivirusCron
/usr/lib/systemd/system/detect_usb.service
/usr/libexec/detect_usb

#/etc/rc2.d/S99autorun-detectUsb
#/etc/init.d/autorunDetectUsb


%changelog

