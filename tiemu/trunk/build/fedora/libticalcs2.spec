%define name	libticalcs2
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
BuildRequires:	libticables2 = %{version}, libticonv = %{version}, libtifiles2 = %{version}, glib2-devel >= 2.10.1
Requires:	libticables2 = %{version}, libticonv = %{version}, libtifiles2 = %{version}, glib2 >= 2.10.1
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Summary:	Library for handling TI calculators through a common API
%description
Library for handling TI calculators through a common API

%prep
%setup -n libticalcs

%build
CFLAGS="$RPM_OPT_FLAGS" ./configure --prefix=%{_prefix} --disable-nls
make

%install
if [ -d $RPM_BUILD_ROOT ]; then rm -rf $RPM_BUILD_ROOT; fi
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
rm -f $RPM_BUILD_ROOT/usr/lib/libticalcs2.la

%post
/sbin/ldconfig
%postun
/sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root)
/usr/include/tilp2/export3.h
/usr/include/tilp2/keys73.h
/usr/include/tilp2/keys83p.h
/usr/include/tilp2/keys89.h
/usr/include/tilp2/keys92p.h
/usr/include/tilp2/ticalcs.h
/usr/include/tilp2/tnode.h
/usr/lib/libticalcs2.so*
/usr/lib/pkgconfig/ticalcs2.pc

%defattr(-,root,root)
%changelog
* Wed May 24 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Don't package .la file (not needed under Fedora).
Make sure permissions are set correctly when building as non-root.

* Mon May 22 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build debuginfo RPM.
Use the system-wide default RPM_OPT_FLAGS instead of my own.
Use BuildRoot recommended by the Fedora packaging guidelines.

* Sun May 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
License now GPL.
Add missing glib2 BuildRequires/Requires.
Now requires libticonv.

* Sat Feb 11 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update setup -n to use the new directory name (libticalcs, not libticalcs2).

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
