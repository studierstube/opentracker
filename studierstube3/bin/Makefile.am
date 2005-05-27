## Makefile.am -- Process this file with automake to produce Makefile.in

SUBDIRS = workspace

bin_SCRIPTS = stb-config

noinst_PROGRAMS = stb-config

stb-config$(EXEEXT): $(top_builddir)/config.status stb-config.in
	@$(SHELL) $(top_builddir)/config.status --file=$@:stb-config.in
	@chmod 755 stb-config
