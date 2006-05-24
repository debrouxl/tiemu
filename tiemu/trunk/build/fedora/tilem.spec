%define name tilem
%define version 0.973
%define release 3

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		LPG (http://lpg.ticalc.org)
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:         %{name}.tar.bz2
Patch0:         tilem-ti85-keypad-fix.diff
Group:		Applications/Emulators
License:	LGPL, z80em License
BuildRequires:	libticables2 >= 20050915, glib2-devel, gtk2-devel >= 2.4.0, imlib-devel, desktop-file-utils >= 0.10
Requires(post):	desktop-file-utils >= 0.10
Requires(postun): desktop-file-utils >= 0.10
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Summary: TilEm is a TI-73/82/83/83+(SE)/84+(SE)/85/86 emulator
%description
TilEm is a TI-73/82/83/83+(SE)/84+(SE)/85/86 emulator.

%prep
%setup -n %{name}-%{version}
sed 's;0.0.3;0.0.1;g' <configure.ac >configure.ac.fixed
mv -f configure.ac.fixed configure.ac
sed 's;0.0.3;0.0.1;g' <configure >configure.fixed
mv -f configure.fixed configure
chmod +x configure
patch -l src/tilem/keypad.c <%{PATCH0}

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} --with-ticables
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
%defattr(-, root, root)
/usr/bin/tilem
/usr/share/tilem
%{_datadir}/applications/lpg-tilem.desktop

%defattr(-,root,root)
%changelog
* Wed May 24 2006 Kevin Kofler <Kevin@tigcc.ticalc.org> 0.973-3
Make sure permissions are set correctly when building as non-root.
Don't hardcode Requires and versions of BuildRequires where not needed.

* Mon May 22 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build debuginfo RPM.
Use the system-wide default RPM_OPT_FLAGS instead of my own.
No period at end of summary.
Use BuildRoot recommended by the Fedora packaging guidelines.

* Wed Jan 5 2006 Kevin Kofler <Kevin@tigcc.ticalc.org> 0.973-2
Apply TI-85 keymap fix by Benjamin Moody.

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org> 0.973-1
Remove unneeded makefile patch.

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Don't require libticables 0.0.3, even the current SVN claims to be only 0.0.2
(bug reported to Romain, it should be 0.0.3).

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
