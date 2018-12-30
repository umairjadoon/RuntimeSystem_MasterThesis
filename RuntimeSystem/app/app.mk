CXX_FLAGS += -I${LIBDIR}

LD_FLAGS += -L${BINDIR}
LD_FLAGS += -lruntime

SRCS := ${wildcard *.cpp}
OBJS := ${patsubst %.cpp, ${BUILDDIR}/%.o, ${notdir ${SRCS}}}
DEPS := ${OBJS:.o=.d}

-include ${DEPS}

all: ${BINDIR}/${APP}

${BINDIR}/${APP}: ${OBJS} ${BINDIR}/${LIB}
	@echo "=============================================="
	@echo "Building target: ${notdir $@}"
	@echo "=============================================="
	${CXX} -MMD -o ${BINDIR}/${APP} ${OBJS} ${LD_FLAGS}

# Compile every cpp file to an object
${BUILDDIR}/%.o: %.cpp
	@mkdir -p ${@D}
	@echo "Building file: $<"
	@${CXX} -MMD ${DEBUG_FLAGS} $(INCLUDE) -c ${CXX_FLAGS} -o ${BUILDDIR}/${notdir $@} $<
