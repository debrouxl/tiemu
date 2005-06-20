# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name	libticalcs
%define version %(date +%%Y%%m%%d)
%define release 1
%define my_opt_flags -Os -s -fno-exceptions -fomit-frame-pointer

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		Romain Liévin 
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:         %{name}-%{version}.tar.bz2
Group:		System Environment/Libraries
License:	LGPL
BuildRequires:	libticables = %{version}, libtifiles = %{version}
Requires:	libticables = %{version}, libtifiles = %{version}
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Summary:	Library for handling TI calculators through a common API
%description
Library for handling TI calculators through a common API

%prep
%setup -n %{name}

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%post
/sbin/ldconfig
%postun
/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/include/tilp/calc_def.h
/usr/include/tilp/calc_err.h
/usr/include/tilp/calc_int.h
/usr/include/tilp/calc_ver.h
/usr/include/tilp/headers.h
/usr/include/tilp/keys73.h
/usr/include/tilp/keys83p.h
/usr/include/tilp/keys89.h
/usr/include/tilp/keys92p.h
/usr/include/tilp/pause.h
/usr/include/tilp/ticalcs.h
/usr/include/tilp/tikeys.h
/usr/include/tilp/tnode.h
/usr/include/tilp/update.h
/usr/lib/libticalcs.la
/usr/lib/libticalcs.so*
/usr/lib/pkgconfig/ticalcs.pc

%defattr(-,root,root)
%changelog
* Sun Jun 19 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Copyright to License.

* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
