include Makefile_config

SOURCES := ${shell find ${SRCDIR} -type f -iname '*.${SRCEXT}'}
DEPENDS := ${patsubst ${SRCDIR}/%.${SRCEXT},${DEPDIR}/%.d,${SOURCES}}

target: ${DEPENDS}
	@make -f Makefile_main
tests: ${DEPENDS}
	@make -f Makefile_main tests
all: ${DEPENDS}
	@make -f Makefile_main all
mathlink: ${DEPENDS} mathlink/mathlink.tm mathlink/mathlink.cpp
	@make -f Makefile_main mathlink/mathlink
clean:
	rm -rf ${BUILDDIR}/*	\
	       ${BINDIR}/*	\
	       ${TESTSDIR}/*    \
	       $(DEPDIR)/*;	\
	echo > include/version.h

${DEPDIR}/%.d: ${SRCDIR}/%.${SRCEXT}
	@mkdir -p ${shell dirname $@} ; \
	echo -n "`dirname '${patsubst ${SRCDIR}/%.${SRCEXT},${BUILDDIR}/%.o,$<}'`/" > $@; \
        $(CC) -MM ${INC} $< >> $@ 
.PHONY: target tests all clean
