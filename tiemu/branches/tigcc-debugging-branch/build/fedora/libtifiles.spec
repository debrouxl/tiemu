# Don't run strip, our binaries are already stripped
%define __spec_install_post :
%define debug_package %{nil}

%define name libtifiles
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
Copyright:	LGPL
BuildRequires:	libticables = ${version}
Requires:	libticables = ${version}
BuildRoot:	/usr/src/redhat/BUILD/buildroot
Summary: Ti File Format management
%description
Ti File Format management

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
/usr/include/tilp/file_def.h
/usr/include/tilp/file_err.h
/usr/include/tilp/file_int.h
/usr/include/tilp/file_ver.h
/usr/include/tilp/files8x.h
/usr/include/tilp/files9x.h
/usr/include/tilp/filesxx.h
/usr/include/tilp/grp_ops.h
/usr/include/tilp/macros.h
/usr/include/tilp/stdints.h
/usr/include/tilp/tifiles.h
/usr/include/tilp/file_ver.h
/usr/include/tilp/types73.h
/usr/include/tilp/types82.h
/usr/include/tilp/types83.h
/usr/include/tilp/types83p.h
/usr/include/tilp/types85.h
/usr/include/tilp/types86.h
/usr/include/tilp/types89.h
/usr/include/tilp/types92.h
/usr/include/tilp/types92p.h
/usr/include/tilp/typesv2.h
/usr/include/tilp/typesxx.h
/usr/lib/libtifiles.la
/usr/lib/libtifiles.so*
/usr/lib/pkgconfig/tifiles.pc

%defattr(-,root,root)
%changelog
* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
