# contrib/abc/Makefile

MODULE_big	= complex
OBJS		= complex.o $(WIN32RES)

EXTENSION = complex
DATA = complex--1.0.sql
PGFILEDESC = "complex - Complex numbers"

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/complex
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
