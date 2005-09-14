# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name tiemu3
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
License:	GPL
BuildRequires:	libticables2 = %{version}, libtifiles2 = %{version}, libticalcs2 = %{version}, glib2-devel >= 2.6.4, gtk2-devel >= 2.6.7, libglade2-devel >= 2.5.1, zlib-devel >= 1.2.2.2, kdelibs-devel >= 3.4.0, xorg-x11-devel >= 6.8.2, ncurses-devel >= 5.4, desktop-file-utils >= 0.10
Requires:	libticables2 = %{version}, libtifiles2 = %{version}, libticalcs2 = %{version}, glib2 >= 2.6.4, gtk2 >= 2.6.7, libglade2 >= 2.5.1, zlib >= 1.2.2.2, kdelibs >= 3.4.0, xorg-x11 >= 6.8.2, ncurses >= 5.4, tcl >= 8.4, tk >= 8.4
Requires(post):	desktop-file-utils >= 0.10
Requires(postun): desktop-file-utils >= 0.10
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Obsoletes:	tiemu < 3.00, tiemu-tigcc-debugging < 20050828, itcl < 3.3, itk < 3.3, iwidgets < 4.0.2
Conflicts:	tiemu >= 3.00
Provides:	tiemu = ${version}, itcl = 3.2, itk = 3.2, iwidgets = 4.0.1
Summary: TiEmu is a TI89(Ti)/92(+)/V200 emulator.
%description
TiEmu is a TI89(Ti)/92(+)/V200 emulator. This version supports graphical debugging using Insight GDB.

%prep
%setup -n %{name}

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls --with-kde --enable-shared-tcl-tk --enable-shared-itcl
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install-without-tcl-tk DESTDIR=$RPM_BUILD_ROOT
mkdir -p ${RPM_BUILD_ROOT}/usr/share/applications
cat >${RPM_BUILD_ROOT}/usr/share/applications/tiemu.desktop <<EOF
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
  ${RPM_BUILD_ROOT}/usr/share/applications/tiemu.desktop

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
/usr/lib/libitclstub*.a
/usr/lib/libitkstub*.a
/usr/lib/libitcl*.so
/usr/lib/libitk*.so
/usr/man/man1/tiemu.1
/usr/man/mann/*.n
/usr/share/insight*
/usr/share/itcl*
/usr/share/itk*
/usr/share/iwidgets*
/usr/share/redhat/gui
/usr/share/tiemu
%{_datadir}/applications/lpg-tiemu.desktop

%defattr(-,root,root)
%changelog
* Wed Sep 14 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update BuildRequires and Requires for new libti*2.
Desktop file is now just lpg-tiemu.desktop, not lpg-tiemu3.desktop.

* Sun Aug 28 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change name from tiemu-tigcc-debugging to tiemu3 and update Obsoletes/Conflicts.

* Sat Aug 6 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Fix file list for shared ITCL.

* Sat Jun 25 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Drop "Conflicts: insight" because this package gets along just fine with
Gérard Milmeister's Insight RPM.
Add --enable-shared-itcl to build itcl/itk/iwidgets shared and Provide them
rather than conflicting with them.

* Sun Jun 19 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Bump version requirements for FC4.
Change Copyright to License.

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
