# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name tiemu-tigcc-debugging
%define version %(date +%%Y%%m%%d)
%define release 1
%define my_opt_flags -Os -s -fno-exceptions -fomit-frame-pointer

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		Romain Liévin 
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:         %{name}-%{version}.tar.bz2
Group:		Applications/Emulators
Copyright:	GPL
BuildRequires:	libticables = %{version}, libtifiles = %{version}, libticalcs = %{version}, glib2-devel >= 2.4.0, gtk2-devel >= 2.4.0, libglade2-devel >= 2.4.0, zlib-devel >= 1.2.1.2, kdelibs-devel >= 3.3.0, xorg-x11-devel >= 6.8.1, ncurses-devel >= 5.4, desktop-file-utils >= 0.9
Requires:	libticables = %{version}, libtifiles = %{version}, libticalcs = %{version}, glib2 >= 2.4.0, gtk2 >= 2.4.0, libglade2 >= 2.4.0, zlib >= 1.2.1.2, kdelibs >= 3.3.0, xorg-x11 >= 6.8.1, ncurses >= 5.4, tcl >= 8.4, tk >= 8.4
Requires(post):	desktop-file-utils >= 0.9
Requires(postun): desktop-file-utils >= 0.9
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Obsoletes:	tiemu < 2.00
Conflicts:	tiemu >= 2.00, insight, itcl, itk
Provides:	tiemu = ${version}
Summary: TiEmu is a TI89(Ti)/92(+)/V200 emulator.
%description
TiEmu is a TI89(Ti)/92(+)/V200 emulator. This version supports graphical debugging using Insight GDB.

%prep
%setup -n %{name}

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls --with-kde --enable-shared-tcl-tk
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install-without-tcl-tk DESTDIR=$RPM_BUILD_ROOT
mkdir -p ${RPM_BUILD_ROOT}/usr/share/applications
cat >${RPM_BUILD_ROOT}/usr/share/applications/%{name}.desktop <<EOF
[Desktop Entry]
Name=TiEmu
Comment=TI89(Ti)/92(+)/V200 emulator
GenericName=TI89(Ti)/92(+)/V200 emulator
Encoding=UTF-8
Version=1.0
Type=Application
Exec=/usr/bin/tiemu
Icon=/usr/share/tiemu/pixmaps/icon.xpm
Terminal=false
Categories=Development;
EOF
desktop-file-install --delete-original --vendor lpg     \
  --dir ${RPM_BUILD_ROOT}%{_datadir}/applications          \
  ${RPM_BUILD_ROOT}/usr/share/applications/%{name}.desktop

%post
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%postun
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/bin/tiemu
/usr/include/itclDecls.h
/usr/include/itcl.h
/usr/include/itclIntDecls.h
/usr/include/itclInt.h
/usr/include/itkDecls.h
/usr/include/itk.h
/usr/lib/insight*
/usr/lib/itcl*
/usr/lib/itk*
/usr/lib/libitcl*.a
/usr/lib/libitk*.a
/usr/man/man1/tiemu.1
/usr/man/mann/*.n
/usr/share/insight*
/usr/share/itcl*
/usr/share/itk*
/usr/share/iwidgets*
/usr/share/redhat/gui
/usr/share/tiemu
%{_datadir}/applications/lpg-%{name}.desktop

%defattr(-,root,root)
%changelog
* Sat Jun 11 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Use Jaime Fernando Meza Meza's icon.

* Fri May 27 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Add Requires on zlib and BuildRequires on zlib-devel.

* Thu May 15 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update description, file list, Requires, BuildReq, Provides and Conflicts.
Use --enable-shared-tcl-tk.

* Thu May 12 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Use make install-without-tcl-tk.

* Mon May 9 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Use now working DESTDIR support.
Don't need terminal anymore.

* Fri May 6 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Menu entry support.

* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
