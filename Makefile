include Makefile_config

SOURCES := ${shell find ${SRCDIR} -type f -iname '*.${SRCEXT}'}
DEPENDS := ${patsubst ${SRCDIR}/%.${SRCEXT},${DEPDIR}/%.d,${SOURCES}}

target: ${DEPENDS}
	make -f Makefile_main
tests: ${DEPENDS}
	make -f Makefile_main tests
all: ${DEPENDS}
	make -f Makefile_main all
clean:
	rm -rf ${BUILDDIR}/*	\
	       ${BINDIR}/*	\
	       ${TESTSDIR}/*    \
	       $(DEPDIR)/*

${DEPDIR}/%.d: ${SRCDIR}/%.${SRCEXT}
	@mkdir -p ${shell dirname $@} ; \
        $(CC) -MM ${INC} $< > $@ 
