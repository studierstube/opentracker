Summary:	Open Tracker
Name:		opentracker
Version:	1.3
Release:	1
Copyright:	LGPL
Group:		Development/Tools
Source:		%{name}-%{version}.tar.gz
Vendor:		Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Packager:	Institute for Computer Graphics and Vision, Graz University of Technology, Austria
Requires:	ACE Xerces-c boost
BuildRequires:	ACE-devel Xerces-c-devel boost-devel
Prefix:		/usr
BuildRoot: 	/tmp/buildroot-%{name}-%{version}

%description
none available

%prep
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT
%setup

%build
%ifarch x86_64
export CFLAGS=-DUSE_64_BIT
export CXXFLAGS=-DUSE_64_BIT
%endif
#./configure --prefix=%{_prefix} --libdir=%{_libdir}
%configure
make

%install
DESTDIR=$RPM_BUILD_ROOT make install

%clean
make clean
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%{_libdir}/*
%{_bindir}/opentracker

%package devel
Summary:	Open Tracker header and include files
Group:		Development/Libraries
Requires:	%{name} = %{version}

%description devel
This package contains header files and include files that are needed for development using the OpenTracker library.

%files devel
%defattr(-,root,root)
%{_bindir}/opentracker-config
%{_prefix}/include/OpenTracker
%{_prefix}/share/OpenTracker
%exclude %{_prefix}/share/opentracker.spec
