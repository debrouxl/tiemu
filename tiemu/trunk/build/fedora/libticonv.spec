# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name	libticonv
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
License:	GPL
BuildRequires:	libticables2 = %{version}, glib2-devel >= 2.10.1
Requires:	libticables2 = %{version}, glib2 >= 2.10.1
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Summary:	Library for handling TI link cables
%description
Library for handling TI link cables

%prep
%setup -n libticonv

%build
CFLAGS="%{my_opt_flags}" ./configure --prefix=%{_prefix} --disable-nls
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
strip $RPM_BUILD_ROOT/usr/lib/libticonv.so.*.*.*
# Kill duplicated header (installed by libticables-2 already)
rm -f $RPM_BUILD_ROOT/usr/include/tilp2/stdints.h

%post
/sbin/ldconfig
%postun
/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/include/tilp2/charset.h
/usr/include/tilp2/export4.h
/usr/include/tilp2/ticonv.h
/usr/lib/libticonv.la
/usr/lib/libticonv.so*
/usr/lib/pkgconfig/ticonv.pc

%defattr(-,root,root)
%changelog
* Sun May 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
