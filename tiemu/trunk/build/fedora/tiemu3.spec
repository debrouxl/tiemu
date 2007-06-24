Name: tiemu3
Epoch: 1
Version: 3.01
Release: 0.1.svn20070623
Vendor: LPG (http://lpg.ticalc.org)
Packager: Kevin Kofler <Kevin@tigcc.ticalc.org>
Source: %{name}-%{version}.tar.bz2
Group: Applications/Emulators
License: GPL
BuildRequires: libticables2-devel >= 1:1.0.0, libticonv-devel >= 1:1.0.0, libtifiles2-devel >= 1:1.0.3, libticalcs2-devel >= 1:1.0.0, glib2-devel >= 2.6.0, gtk2-devel >= 2.6.0, libglade2-devel >= 2.4.0, zlib-devel, kdelibs-devel >= 6:3.0, libX11-devel, libXext-devel, ncurses-devel, desktop-file-utils >= 0.10, bison >= 1.28, flex >= 2.5.4, texinfo >= 4.4, dbus-devel >= 0.60, dbus-glib-devel >= 0.60, SDL-devel >= 1.2.0
Requires: tcl >= 8.4, tk >= 8.4, itcl >= 3.3, itk >= 3.3, iwidgets >= 4.0.1, xdg-utils >= 1.0.0
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Obsoletes: tiemu < %{version}, tiemu-tigcc-debugging < 20050828
Conflicts: tiemu > %{version}
Provides: tiemu = %{version}
Summary: TiEmu is a TI89(Ti)/92(+)/V200 emulator
%description
TiEmu is a TI89(Ti)/92(+)/V200 emulator. This version supports graphical debugging using Insight GDB.

%prep
%setup -n tiemu-3-sound

%build
source /etc/profile.d/qt.sh
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itcl/configure.in
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itcl/configure
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itk/configure.in
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itk/configure
export extra_ldflags="-Wl,-rpath,%{_libdir}/itcl3.3 -Wl,-rpath,%{_libdir}/itk3.3"
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} --libdir=%{_libdir} --mandir=%{_mandir} --disable-nls --enable-shared-tcl-tk --enable-shared-itcl --with-dbus
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install-without-tcl-tk-itcl DESTDIR=$RPM_BUILD_ROOT
# don't package unneeded empty directory
rmdir $RPM_BUILD_ROOT%{_libdir}/insight1.0
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
%defattr(-, root, root)
/usr/bin/tiemu
%{_mandir}/man1/tiemu*
/usr/share/insight*
/usr/share/redhat/gui
/usr/share/tiemu
%{_datadir}/applications/lpg-tiemu.desktop

%changelog
* Sat Jun 23 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.01-0.1.svn20070623
Add BR SDL-devel.

* Wed May 16 2007 Kevin Kofler <Kevin@tigcc.ticalc.org>
Drop -n tiemu, the tarball uses name-version format now.

* Mon Apr 16 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.00-1
Bump Epoch.
Use real version number instead of date.
Update Obsoletes, Provides and Conflicts.
Also use real version numbers and Epoch 1 for the dependencies.

* Mon Apr 16 2007 Kevin Kofler <Kevin@tigcc.ticalc.org>
Remove redundant explicit Requires.
Don't BuildRequire newer versions than actually needed.
Don't force flex = 2.5.4a, newer versions appear to be supposed to work.
Don't require desktop-file-utils for post and postun (not needed in FC5+).

* Wed Feb 7 2007 Kevin Kofler <Kevin@tigcc.ticalc.org>
Require xdg-utils.
Fix paths for itcl version hacks.

* Fri Nov 3 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Source /etc/profile.d/qt.sh so QTDIR is always set.

* Mon Oct 30 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
BuildRequire dbus-glib-devel instead dbus-glib (FC6 splits it out).

* Tue Oct 10 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build with D-Bus support.

* Sun Jul 23 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Fix rmdir by moving it after make install.

* Fri Jul 21 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Don't package unneeded empty directory.

* Thu Jul 20 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Libdir fixes for lib64 platforms.
Use libtifoo-devel instead of libtifoo in BuildRequires.
Pass mandir explicitly so the manfile directory doesn't depend on the autoconf
version used to generate configure.

* Fri Jun 16 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Remove redundant %%defattr at the end of %%files.

* Wed May 24 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Make sure permissions are set correctly when building as non-root.
Allow manpages to be gzipped (done by rpm when using redhat-rpm-config).

* Mon May 22 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build debuginfo RPM.
Use the system-wide default RPM_OPT_FLAGS instead of my own.
No period at end of summary.
Use BuildRoot recommended by the Fedora packaging guidelines.

* Sun May 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Now requires libticonv.

* Tue Apr 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
BuildRequire exact version of flex because GDB really does want 2.5.4a.

* Mon Apr 3 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
BuildRequire texinfo (the build reportedly fails without it).

* Thu Mar 23 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update dependencies for FC5.

* Sun Jan 29 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build against itcl/itk 3.3 (RC1).
Add Epoch to KDE requirements.
BuildRequire bison and flex.

* Fri Jan 27 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
--with-kde not needed anymore (now default).

* Sat Jan 9 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Add /usr/lib/itcl3.2 and /usr/lib/itk3.2 to the rpath.

* Sat Jan 8 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Use the itcl, itk and iwidgets RPMs being proposed for Extras.

* Sat Jan 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Require kdelibs 3.5.0 from FC4 updates.

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Vendor to LPG.

* Wed Sep 14 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update BuildRequires and Requires for new libti*2.
Desktop file is now just lpg-tiemu.desktop, not lpg-tiemu3.desktop.
Fix setup -n to use the correct directory name (tiemu, not tiemu3).

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
