Summary:	Open Tracker
Name:		opentracker
Version:	2.0.0
Release:	1
License:	LGPL
Group:		Development/Tools
Source:		%{name}-%{version}.tar.bz2
Vendor:		Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Requires:	ACE Xerces-c boost ncurses
BuildRequires:	ACE-devel Xerces-c-devel boost-devel ncurses-devel
Prefix:		/usr
BuildRoot: 	%{_tmppath}/buildroot-%{name}-%{version}

%description
none available

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup
sed -i 's/curses/ncurses/' SConstruct

%build
%ifarch x86_64
export CFLAGS=-DUSE_64_BIT
export CXXFLAGS=-DUSE_64_BIT
%endif
scons

%install
scons --cache-disable DESTDIR=$RPM_BUILD_ROOT PREFIX=%{_prefix} LIBDIR=%{_libdir} install

%clean
scons -c
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*.so*
%{_bindir}/*

%package devel
Summary:	Open Tracker header and include files
Group:		Development/Libraries
Requires:	%{name} = %{version}

%description devel
This package contains header files and include files that are needed for development using the OpenTracker library.

%files devel
%defattr(-,root,root)
%{_libdir}/pkgconfig/*
%{_prefix}/include/OpenTracker
