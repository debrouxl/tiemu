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
BuildRequires:	libticables = %{version}, libtifiles = %{version}, libticalcs = %{version}, glib2-devel >= 2.4.0, gtk2-devel >= 2.4.0, libglade2-devel >= 2.4.0, kdelibs-devel >= 3.3.0, ncurses-devel >= 5.4
Requires:	libticables = %{version}, libtifiles = %{version}, libticalcs = %{version}, glib2 >= 2.4.0, gtk2 >= 2.4.0, libglade2 >= 2.4.0, kdelibs >= 3.3.0, ncurses >= 5.4
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Obsoletes:	tiemu < 2.00
Conflicts:	tiemu >= 2.00
Provides:	tiemu = ${version}
Summary: TiEmu is a TI89(Ti)/92(+)/V200 emulator.
%description
TiEmu is a TI89(Ti)/92(+)/V200 emulator. This version supports debugging using GDB.

%prep
%setup -n %{name}

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls --with-kde
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install prefix=$RPM_BUILD_ROOT%{_prefix}

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/bin/tiemu
/usr/man/man1/tiemu.1
/usr/share/tiemu

%defattr(-,root,root)
%changelog
* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
