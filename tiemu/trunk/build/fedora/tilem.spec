# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name tilem
%define version 0.973
%define release 1
%define my_opt_flags -Os -s -fno-exceptions -fomit-frame-pointer

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		LPG (http://lpg.ticalc.org)
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:         %{name}.tar.bz2
Group:		Applications/Emulators
License:	LGPL, z80em License
BuildRequires:	libticables2 >= 20050915, glib2-devel >= 2.6.4, gtk2-devel >= 2.6.7, imlib-devel >= 1.9.13, desktop-file-utils >= 0.10
Requires:	libticables2 >= 20050915, glib2 >= 2.6.4, gtk2 >= 2.6.7, imlib >= 1.9.13
Requires(post):	desktop-file-utils >= 0.10
Requires(postun): desktop-file-utils >= 0.10
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Summary: TilEm is a TI-73/82/83/83+(SE)/84+(SE)/85/86 emulator.
%description
TilEm is a TI-73/82/83/83+(SE)/84+(SE)/85/86 emulator.

%prep
%setup -n %{name}-%{version}
sed 's;0.0.3;0.0.1;g' <configure.ac >configure.ac.fixed
mv -f configure.ac.fixed configure.ac
sed 's;0.0.3;0.0.1;g' <configure >configure.fixed
mv -f configure.fixed configure
chmod +x configure

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --with-ticables
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
cd src/tilem
mkdir -p ${RPM_BUILD_ROOT}/usr/bin
make install datadir=$RPM_BUILD_ROOT%{_prefix}/share bindir=$RPM_BUILD_ROOT%{_prefix}/bin
mkdir -p ${RPM_BUILD_ROOT}/usr/share/applications
cat >${RPM_BUILD_ROOT}/usr/share/applications/tilem.desktop <<EOF
[Desktop Entry]
Name=TilEm
Comment=TI-73/82/83/83+(SE)/84+(SE)/85/86 emulator
GenericName=TI-73/82/83/83+(SE)/84+(SE)/85/86 emulator
Encoding=UTF-8
Version=1.0
Type=Application
Exec=/usr/bin/tilem
#Icon=???
Terminal=false
Categories=Development;
EOF
desktop-file-install --delete-original --vendor lpg     \
  --dir ${RPM_BUILD_ROOT}%{_datadir}/applications          \
  ${RPM_BUILD_ROOT}/usr/share/applications/tilem.desktop

%post
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%postun
update-desktop-database %{_datadir}/applications > /dev/null 2>&1 || :

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/bin/tilem
/usr/share/tilem
%{_datadir}/applications/lpg-tilem.desktop

%defattr(-,root,root)
%changelog
* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Remove unneeded makefile patch.

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Don't require libticables 0.0.3, even the current SVN claims to be only 0.0.2
(bug reported to Romain, it should be 0.0.3).

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
