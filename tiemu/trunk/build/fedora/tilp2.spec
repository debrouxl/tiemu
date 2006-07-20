%define name	tilp2
%define version %(date +%%Y%%m%%d)
%define release 1

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		LPG (http://lpg.ticalc.org)
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:		%{name}-%{version}.tar.bz2
Group:		Applications/Communications
License:	GPL
BuildRequires:	libticables2-devel = %{version}, libticonv-devel = %{version}, libtifiles2-devel = %{version}, libticalcs2-devel = %{version}, glib2-devel >= 2.6.4, gtk2-devel >= 2.6.7, libglade2-devel >= 2.5.1, zlib-devel >= 1.2.2.2, desktop-file-utils >= 0.10
Requires:	libticables2 = %{version}, libticonv = %{version}, libtifiles2 = %{version}, libticalcs2 = %{version}, glib2 >= 2.6.4, gtk2 >= 2.6.7, libglade2 >= 2.5.1, zlib >= 1.2.2.2
Requires(post):	shared-mime-info, desktop-file-utils >= 0.9
Requires(postun): shared-mime-info, desktop-file-utils >= 0.9
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Summary:	TiLP is a TI<->PC linking program
Obsoletes:  tilp < 20050828
%description
TiLP is a TI<->PC linking program

%prep
%setup -n tilp

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} --libdir=%{_libdir} --mandir=%{_mandir} --disable-nls
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
mkdir -p ${RPM_BUILD_ROOT}/usr/share/applications
cat >${RPM_BUILD_ROOT}/usr/share/applications/tilp.desktop <<EOF
[Desktop Entry]
Name=TiLP
Name[fr_FR]=TiLP
Comment=Ti Linking Program
Comment[fr_FR]=Programme de connexion pour calculatrices TI
GenericName=TI Linking Program
GenericName[fr_FR]=Connexion calculatrices TI
Encoding=UTF-8
Version=1.0
Type=Application
Exec=/usr/bin/tilp
Icon=/usr/share/tilp2/pixmaps/icon.xpm
Terminal=false
Categories=Utility;X-KDE-Utilities-Peripherals;
MimeType=application/x-tilp
EOF
desktop-file-install --delete-original --vendor lpg     \
  --dir ${RPM_BUILD_ROOT}%{_datadir}/applications          \
  ${RPM_BUILD_ROOT}/usr/share/applications/tilp.desktop
mkdir -p ${RPM_BUILD_ROOT}%{_datadir}/mime/packages
cat >${RPM_BUILD_ROOT}%{_datadir}/mime/packages/tilp.xml <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<mime-info xmlns="http://www.freedesktop.org/standards/shared-mime-info">
    <mime-type type="application/x-tilp">
        <comment xml:lang="en">TI File</comment>
        <comment xml:lang="fr">Fichier TI</comment>
        <glob pattern="*.73?" />
        <glob pattern="*.82?" />
        <glob pattern="*.8[xX]?" />
        <glob pattern="*.85?" />
        <glob pattern="*.86?" />
        <glob pattern="*.89?" />
        <glob pattern="*.92?" />
        <glob pattern="*.9[xX]?" />
        <glob pattern="*.[vV]2?" />
  </mime-type>
</mime-info>
EOF
cd ${RPM_BUILD_ROOT}/usr/bin && ln -s tilp-2 tilp
cd ${RPM_BUILD_ROOT}%{_mandir}/man1 && ln -s tilp-2.1 tilp.1

%post
update-mime-database %{_datadir}/mime > /dev/null 2>&1 || :
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%postun
update-mime-database %{_datadir}/mime > /dev/null 2>&1 || :
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
/usr/bin/tilp
/usr/bin/tilp-2
%{_mandir}/man1/tilp*
/usr/share/tilp2
%{_datadir}/applications/lpg-tilp.desktop
%{_datadir}/mime/packages/tilp.xml

%changelog
* Thu Jul 20 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Libdir fixes for lib64 platforms.
Use libtifoo-devel instead of libtifoo in BuildRequires.
Pass mandir explicitly so the manfile directory doesn't depend on the autoconf
version used to generate configure.

* Fri Jun 16 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Remove redundant %%defattr at the end of %%files.

* Wed May 24 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Make sure permissions are set correctly when building as non-root.
--enable-exit-homedir not needed anymore (now default).
Allow manpages to be gzipped (done by rpm when using redhat-rpm-config).
Add X-KDE-Utilities-Peripherals category to .desktop file.

* Mon May 22 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build debuginfo RPM.
Use the system-wide default RPM_OPT_FLAGS instead of my own.
Use BuildRoot recommended by the Fedora packaging guidelines.

* Sun May 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Now requires libticonv.

* Sat Feb 11 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update setup -n to use the new directory name (tilp, not tilp2).

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Vendor to LPG.

* Wed Sep 14 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update to TiLP 2: new package name, new dependencies, obsoletes the old package,
some files moved, some compatibility symlinks added, .desktop file updated for
new icon location, dropped obsolete
tilp-no-obsolete-desktop-and-mime-entries.diff patch.

* Mon Jun 20 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Revert -Wl,--export-dynamic addition (now upstream).

* Sun Jun 19 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Bump version requirements for FC4.
Change Copyright to License.
Add missing -Wl,--export-dynamic.

* Fri May 27 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Add Requires on zlib and BuildRequires on zlib-devel.

* Fri May 6 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Menu entry and MIME type support.

* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
