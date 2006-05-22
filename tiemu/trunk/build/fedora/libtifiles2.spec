%define name libtifiles2
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
BuildRequires:	libticables2 = %{version}, libticonv = %{version}, zlib-devel >= 1.2.2.2, glib2-devel >= 2.10.1
Requires:	libticables2 = %{version}, libticonv = %{version}, zlib >= 1.2.2.2, glib2 >= 2.10.1
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
Summary: Ti File Format management
%description
Ti File Format management

%prep
%setup -n libtifiles

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
/usr/include/tilp2/export2.h
/usr/include/tilp2/files8x.h
/usr/include/tilp2/files9x.h
/usr/include/tilp2/macros.h
/usr/include/tilp2/tifiles.h
/usr/include/tilp2/types*.h
/usr/lib/libtifiles2.la
/usr/lib/libtifiles2.so*
/usr/lib/pkgconfig/tifiles2.pc

%defattr(-,root,root)
%changelog
* Mon May 22 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Build debuginfo RPM.
Use the system-wide default RPM_OPT_FLAGS instead of my own.
Use BuildRoot recommended by the Fedora packaging guidelines.

* Sun May 7 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
License now GPL.
Add missing glib2 BuildRequires/Requires.
Now requires libticonv.

* Sat Feb 11 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update setup -n to use the new directory name (libtifiles, not libtifiles2).

* Sun Jan 29 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
BuildRequire zlib-devel, Require zlib.
Strip library because -s gets ignored somehow.

* Wed Jan 4 2006 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Vendor to LPG.

* Wed Sep 14 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Update to libtifiles-2: new package name, updated file list and dependencies.

* Sun Jun 19 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
Change Copyright to License.
Don't list file_ver.h twice.

* Mon May 2 2005 Kevin Kofler <Kevin@tigcc.ticalc.org>
First Fedora RPM.
