Name: tiemu3
Epoch: 1
Version: 3.03
%if 0%{?fedora} == 8
Release: 0.1.20081230svn2798
%define system_tcl 1
%else
Release: 0.1.20081230svn2798.1
%endif
Vendor: LPG (http://lpg.ticalc.org)
Packager: Kevin Kofler <Kevin@tigcc.ticalc.org>
Source: tiemu-%{version}-svn2798.tar.bz2
Group: Applications/Emulators
License: GPLv2+
BuildRequires: libticables2-devel >= 1:1.0.0, libticonv-devel >= 1:1.0.4, libtifiles2-devel >= 1:1.0.7, libticalcs2-devel >= 1:1.0.7, glib2-devel >= 2.6.0, gtk2-devel >= 2.6.0, libglade2-devel >= 2.4.0, zlib-devel, kdelibs3-devel, libX11-devel, libXext-devel, ncurses-devel, desktop-file-utils >= 0.10, bison >= 1.28, flex >= 2.5.4, texinfo >= 4.4, dbus-devel >= 0.60, dbus-glib-devel >= 0.60, SDL-devel >= 1.2.0, groff
%if 0%{?system_tcl}
Requires: tcl >= 8.4, tk >= 8.4, itcl >= 3.3-0.11.RC1, itk >= 3.3-0.8.RC1, iwidgets >= 4.0.1
%else
Conflicts: tcl < 1:8.5, tk < 1:8.5, itcl < 3.3, itk < 3.3, iwidgets < 4.0.2
%endif
Requires: xdg-utils >= 1.0.0
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Obsoletes: tiemu < %{version}, tiemu-tigcc-debugging < 20050828
Conflicts: tiemu > %{version}
Provides: tiemu = %{version}
Summary: TiEmu is a TI89(Ti)/92(+)/V200 emulator
%description
TiEmu is a TI89(Ti)/92(+)/V200 emulator. This version supports graphical debugging using Insight GDB.

%prep
%setup -n tiemu-%{version}-svn2798

%build
source /etc/profile.d/qt.sh
%if 0%{?system_tcl}
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itcl/configure.in
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itcl/configure
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itk/configure.in
sed -i 's/MINOR_VERSION=2/MINOR_VERSION=3/g;s/PATCHLEVEL=\.1/PATCHLEVEL=\.0/g' src/gdb/itcl/itk/configure
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} --libdir=%{_libdir} --mandir=%{_mandir} --disable-nls --enable-shared-tcl-tk --enable-shared-itcl --with-dbus
%else
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} --libdir=%{_libdir} --mandir=%{_mandir} --disable-nls --with-dbus
%endif
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
%if 0%{?system_tcl}
make install-without-tcl-tk-itcl DESTDIR=$RPM_BUILD_ROOT
%else
make install DESTDIR=$RPM_BUILD_ROOT
%endif
# don't package unneeded empty directory
rmdir $RPM_BUILD_ROOT%{_libdir}/insight1.0
%if !0%{?system_tcl}
# don't package Tcl/Tk stuff which conflicts with the system versions
rm -rf $RPM_BUILD_ROOT%{_includedir}
rm -f $RPM_BUILD_ROOT%{_mandir}/man1/tclsh.1* $RPM_BUILD_ROOT%{_mandir}/man1/wish.1*
rm -rf $RPM_BUILD_ROOT%{_mandir}/man3
rm -rf $RPM_BUILD_ROOT%{_mandir}/mann
rm -rf $RPM_BUILD_ROOT/usr/man/mann
rm -f $RPM_BUILD_ROOT%{_libdir}/tclConfig.sh $RPM_BUILD_ROOT%{_libdir}/tkConfig.sh
# don't package these either, they won't conflict, but they aren't useful either
rm -f $RPM_BUILD_ROOT%{_bindir}/tclsh8.4 $RPM_BUILD_ROOT%{_bindir}/wish8.4
rm -f $RPM_BUILD_ROOT%{_libdir}/*.a
%endif

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
%if !0%{?system_tcl}
%{_datadir}/tcl8.4
%{_datadir}/tk8.4
%{_datadir}/itcl3.2
%{_datadir}/itk3.2
%{_datadir}/iwidgets4.0.1
%{_libdir}/itcl3.2
%{_libdir}/itk3.2
%endif

%changelog
* Tue Dec 30 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.03-0.1.20081230svn2798
Update to SVN snapshot (revision 2798).
Conditionalize use of system vs. builtin Tcl/Tk to allow for F8 build.
Drop backported patches.

* Thu May 1 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.02-3
Revert to bundled versions of Tcl/Tk/itcl/itk/iwidgets for Fedora 9.
Conflict with Tcl/Tk/itcl/itk/iwidgets of the same versions as the bundled ones.
Remove Tcl/Tk/itcl/itk/iwidgets files which conflict with the system version.
Update file list to include the bundled Tcl/Tk/itcl/itk/iwidgets files.
Fix build with GCC 4.3.
Add missing BuildRequires: groff.

* Wed Feb 13 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.02-2
Use respun tarball.
Backport cumulative fix for dock-related debugger bugs from SVN.

* Sun Jan 13 2008 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.02-1
Update to 3.02 (release).
BuildRequire kdelibs3-devel instead of kdelibs-devel.
Drop itcl/itk rpath hack, require fixed versions of itcl and itk.

* Sat Dec 15 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.02-0.3.20071215test3
Update from SVN again, some bugfixes.

* Fri Dec 14 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.02-0.2.20071214test2
New test version from SVN, with dock support added by Romain.

* Tue Dec 11 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.02-0.1.20071211test1
Update to test version from Romain.
Drop patches (all obsolete).
Tarball is now named tiemu rather than tiemu3.

* Fri Nov 30 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.01a-2
Backport fix for debugging information relocation for kernel programs from SVN.

* Fri Nov 30 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.01a-1
Specify GPL version in License tag.
Apply 3.01a patch.
Backport SVN patch to drop old GTK+ file selector to fix build with GTK+ 2.12.
Backport fix for the KDE file dialog (dirname+filename concatenation) from SVN.
Backport cumulative fix for 2 more GTK+ 2.12 build issues from SVN.
Backport cumulative fix for GDB issues (makeinfo version check, improper use of
longjmp to exit gtk_main triggering an assert with GLib 2.14) from SVN.

* Wed Jun 27 2007 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update tilibs BuildRequires.

* Sun Jun 24 2007 Kevin Kofler <Kevin@tigcc.ticalc.org> 1:3.01-1
Bump Release.

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
