%define name	libticonv
%define version %(date +%%Y%%m%%d)
%define release 1

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
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Summary:	Library for handling TI link cables
%description
Library for handling TI link cables

%prep
%setup -n libticonv

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} --disable-nls
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
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
* Mon May 22 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build debuginfo RPM.
Use the system-wide default RPM_OPT_FLAGS instead of my own.
Use BuildRoot recommended by the Fedora packaging guidelines.

* Sun May 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
