## Makefile.am -- Process this file with automake to produce Makefile.in

include $(top_srcdir)/src/Makefile.common

lib_LTLIBRARIES = calibration_stb.la

calibration_stb_la_SOURCES = \
	$(top_srcdir)/src/apps/calibration/powell/brent.cxx \
	$(top_srcdir)/src/apps/calibration/powell/f1dim.cxx \
	$(top_srcdir)/src/apps/calibration/powell/mnbrak.cxx \
	$(top_srcdir)/src/apps/calibration/powell/nrutil.h \
	$(top_srcdir)/src/apps/calibration/powell/linmin.cxx \
	$(top_srcdir)/src/apps/calibration/powell/nrutil.cxx \
	$(top_srcdir)/src/apps/calibration/powell/powell.cxx \
	$(top_srcdir)/src/apps/calibration/powell/powell.h \
	$(top_srcdir)/src/apps/calibration/utils/SoSampler.cxx \
	$(top_srcdir)/src/apps/calibration/utils/SoSampler.h \
	$(top_srcdir)/src/apps/calibration/utils/cameraOptimizer.cxx \
	$(top_srcdir)/src/apps/calibration/utils/cameraOptimizer.h \
	$(top_srcdir)/src/apps/calibration/utils/SoMessageKit.h \
	$(top_srcdir)/src/apps/calibration/utils/SoMessageKit.cxx \
	$(top_srcdir)/src/apps/calibration/utils/So2DMenueKit.h \
	$(top_srcdir)/src/apps/calibration/utils/So2DMenueKit.cxx \
        $(top_srcdir)/src/apps/calibration/hmdCalibration/SoHmdCalibrationKit.cxx \
        $(top_srcdir)/src/apps/calibration/hmdCalibration/SoHmdCalibrationKit.h \
	$(top_srcdir)/src/apps/calibration/projectionCalibration/SoAngularConstraintEngine.h \
	$(top_srcdir)/src/apps/calibration/projectionCalibration/SoAngularConstraintEngine.cxx \
	$(top_srcdir)/src/apps/calibration/projectionCalibration/SoProjectionCalibrationKit.h \
	$(top_srcdir)/src/apps/calibration/projectionCalibration/SoProjectionCalibrationKit.cxx

calibration_stb_la_LDFLAGS = -no-undefined -module -avoid-version
