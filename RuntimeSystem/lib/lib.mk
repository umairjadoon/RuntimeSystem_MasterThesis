SRCS := ${shell find . -name '*.cpp'}
SRCS_AS :=${shell find . -name '*.S'}
OBJS := ${patsubst %.cpp, ${BUILDDIR}/%.o, ${notdir ${SRCS}}}
OBJS_AS := ${patsubst %.S, ${BUILDDIR}/%.o, ${notdir ${SRCS_AS}}}
DEPS := ${OBJS:.o=.d}

FILES += ${shell find . -type f -name '*.*' ! -name '*.mk'}

CXX_FLAGS += -I${LIBDIR}

VPATH := ${LIBDIR}/src/WorkerThread
VPATH += ${LIBDIR}/src/WorkPackage
VPATH += ${LIBDIR}/src/WorkPackageManagement
VPATH += ${LIBDIR}/src/WorkPackageQueue
VPATH += ${LIBDIR}/src/WorkPackageSynchronization
VPATH += ${LIBDIR}/src/MemoryManagement
VPATH += ${LIBDIR}/src/FiberKey
VPATH += ${LIBDIR}/include

-include ${DEPS}

all: ${BINDIR}/${LIB}

${BINDIR}/${LIB}: $(OBJS) $(OBJS_AS)
	@mkdir -p ${BINDIR}
	@echo "=============================================="
	@echo "Building target: ${notdir $@}"
	@echo "=============================================="
	@${AR} rcs $@ $^
	
${BUILDDIR}/%.o: %.cpp
	@mkdir -p ${BUILDDIR}
	@echo "Building file:  ${notdir $<}"
	@${CXX} -MMD ${GCOV_FLAGS} ${DEBUG_FLAGS} $(INCLUDE) -c ${CXX_FLAGS} -o $@ $<
	
${BUILDDIR}/%.o: %.S
	@mkdir -p ${BUILDDIR}
	@echo "Building file:  ${notdir $<}"
	@${CXX} ${DEBUG_FLAGS} $(INCLUDE) -c ${CXX_FLAGS} $< -o $@


.PHONY: tidy

tidy: ${FILES}
	clang-tidy -checks=modernize-*,clang-analyzer-*,cppcoreguidelines-*,cert-*,performance-* ${FILES} -- -I. -std=c++11 #
	
format: ${FILES}
	clang-format -i -style=${STYLE} ${FILES}
