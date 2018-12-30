export BINDIR := ${CURDIR}/bin
export LIBDIR := ${CURDIR}/lib
export BUILDDIR := ${CURDIR}/build
export TESTDIR := ${CURDIR}/test
export INCDIR:= ../lib/include
export INCLUDE:= -I$(INCDIR)


export APP := app
export LIB := libruntime.a
export TEST := test
export BENCH := bench

export CXX_FLAGS := -Wall -Wextra -std=c++11
export CXX := clang++
export LD_FLAGS := -lpthread -lsqlite3

export GCOV_FLAGS :=
export DEBUG_FLAGS := -g3

all: app 

app: lib
	@${MAKE} all -C app -f app.mk --no-print-directory

lib:
	@${MAKE} all -C lib -f lib.mk --no-print-directory

test: GCOV_FLAGS := -fprofile-instr-generate -fcoverage-mapping -DCODECOVERAGE
test: BUILDDIR := ${BUILDDIR}/test/
test: lib ${BINDIR}/${LIB} ${TEMPLATES}
	@${MAKE} all -C test -f test.mk --no-print-directory

bench: lib ${BINDIR}/${LIB} ${TEMPLATES}
	@${MAKE} all -C benchmark -f bench.mk --no-print-directory

tidy:
	@${MAKE} tidy -C src -f src.mk --no-print-directory

format:
	@${MAKE} format -C src -f src.mk --no-print-directory

run_bench:
	@${MAKE} run_bench -C benchmark -f bench.mk --no-print-directory

.PHONY: clean build test tidy bench lib
clean:
	@cd build && rm -rf *.o *.d test
	@cd bin && rm -rf *
	@rm -rf *.profdata *.profraw
