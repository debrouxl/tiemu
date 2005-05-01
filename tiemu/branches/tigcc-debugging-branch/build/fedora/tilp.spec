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
Source:         %{name}-%{version}.tar.bz2
Group:		Application/Generic
Copyright:	GPL
BuildRequires:	libticables = ${version}, libtifiles = ${version}, libticalcs = ${version}
Requires:	libticables = ${version}, libtifiles = ${version}, libticalcs = ${version}
BuildRoot:	/usr/src/redhat/BUILD
Summary:	TiLP is a TI<->PC linking program
%description
TiLP is a TI<->PC linking program

%prep
%setup -n %{name}

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls --enable-exit-homedir
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/bin/tilp
/usr/include/tilp/tilp_plugin.h
/usr/man/man1/tiemu.1
/usr/share/tilp

%defattr(-,root,root)
%changelog
* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
