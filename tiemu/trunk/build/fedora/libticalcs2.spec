# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name	libticalcs2
%define version %(date +%%Y%%m%%d)
%define release 1
%define my_opt_flags -Os -s -fno-exceptions -fomit-frame-pointer

Name:		%{name}
Version:	%{version}
Release:	%{release}
Vendor:		LPG (http://lpg.ticalc.org)
Packager:	Kevin Kofler <Kevin@tigcc.ticalc.org>
Source:         %{name}-%{version}.tar.bz2
Group:		System Environment/Libraries
License:	LGPL
BuildRequires:	libticables2 = %{version}, libtifiles2 = %{version}
Requires:	libticables2 = %{version}, libtifiles2 = %{version}
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
strip $RPM_BUILD_ROOT/usr/lib/libticalcs2.so.*.*.*

%post
/sbin/ldconfig
%postun
/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/include/tilp2/export3.h
/usr/include/tilp2/keys73.h
/usr/include/tilp2/keys83p.h
/usr/include/tilp2/keys89.h
/usr/include/tilp2/keys92p.h
/usr/include/tilp2/ticalcs.h
/usr/include/tilp2/tnode.h
/usr/lib/libticalcs2.la
/usr/lib/libticalcs2.so*
/usr/lib/pkgconfig/ticalcs2.pc

%defattr(-,root,root)
%changelog
* Sun Jan 29 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Strip library because -s gets ignored somehow.

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Vendor to LPG.

* Wed Sep 14 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update to libticalcs-2: new package name, updated file list and dependencies.

* Sun Jun 19 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Copyright to License.

* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
