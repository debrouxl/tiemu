# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name	tilp
%define version %(date +%%Y%%m%%d)
%define release 1
%define my_opt_flags -Os -s -fno-exceptions -fomit-frame-pointer

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		Romain Liévin 
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:		%{name}-%{version}.tar.bz2
Patch0:		tilp-no-obsolete-desktop-and-mime-entries.diff
Group:		Applications/Communications
License:	GPL
BuildRequires:	libticables = %{version}, libtifiles = %{version}, libticalcs = %{version}, glib2-devel >= 2.6.4, gtk2-devel >= 2.6.7, libglade2-devel >= 2.5.1, zlib-devel >= 1.2.2.2, desktop-file-utils >= 0.10
Requires:	libticables = %{version}, libtifiles = %{version}, libticalcs = %{version}, glib2 >= 2.6.4, gtk2 >= 2.6.7, libglade2 >= 2.5.1, zlib >= 1.2.2.2
Requires(post):	shared-mime-info, desktop-file-utils >= 0.9
Requires(postun): shared-mime-info, desktop-file-utils >= 0.9
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Summary:	TiLP is a TI<->PC linking program
%description
TiLP is a TI<->PC linking program

%prep
%setup -n %{name}
patch desktop/Makefile.in <%{PATCH0}

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls --enable-exit-homedir
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
mkdir -p ${RPM_BUILD_ROOT}/usr/share/applications
cat >${RPM_BUILD_ROOT}/usr/share/applications/%{name}.desktop <<EOF
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
Icon=/usr/share/tilp/pixmaps/icon.xpm
Terminal=false
Categories=Utility;
MimeType=application/x-tilp
EOF
desktop-file-install --delete-original --vendor lpg     \
  --dir ${RPM_BUILD_ROOT}%{_datadir}/applications          \
  ${RPM_BUILD_ROOT}/usr/share/applications/%{name}.desktop
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

%post
update-mime-database %{_datadir}/mime > /dev/null 2>&1 || :
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%postun
update-mime-database %{_datadir}/mime > /dev/null 2>&1 || :
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/bin/tilp
/usr/include/tilp/tilp_plugin.h
/usr/man/man1/tilp.1
/usr/share/tilp
%{_datadir}/applications/lpg-%{name}.desktop
%{_datadir}/mime/packages/tilp.xml

%defattr(-,root,root)
%changelog
* Fri Jun 19 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Bump version requirements for FC4.
Change Copyright to License.

* Fri May 27 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Add Requires on zlib and BuildRequires on zlib-devel.

* Fri May 6 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Menu entry and MIME type support.

* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
