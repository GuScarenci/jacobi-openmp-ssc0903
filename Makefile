#Trabalho 1 de Computação de Alto Desempenho - SCC00903
#
#  Docente:
#      Paulo Sérgio Lopes de Souza
#  Alunos:
#      Beatriz Lomes da Silva -  12548038
#      Gustavo Moura Scarenci de Carvalho Ferreira -12547792
#      Hugo Hiroyuki Nakamura - 12732037
#      Matheus Henrique Dias Cirillo - 12547750

SHELL := /bin/bash
CC ?= gcc
ZIP ?= zip
PDFLAGS += -march=native
CFLAGS += -Wall -Wextra -Wpedantic -O3
LDFLAGS += -lm -fopenmp
VDFLAGS += --leak-check=full --show-leak-kinds=all -s

SRCDIRS = $(sort $(dir $(wildcard src/*/)))
INC_PARAMS = $(addprefix -I ,$(SRCDIRS))

EXECUTABLE_SEQ ?= build/jacobiseq
EXECUTABLE_PAR ?= build/jacobipar

ZIPFILE    ?= ../zipfile.zip
CFILES      = $(wildcard src/*.c) $(wildcard src/**/*.c)

.PHONY: all clean zip run run_seq run_par test debug_seq debug_par

all: $(EXECUTABLE_SEQ) $(EXECUTABLE_PAR)

$(EXECUTABLE_SEQ): $(CFILES)
	@mkdir -p build
	$(CC) $(CFLAGS) $(INC_PARAMS) -o $@ $^ $(LDFLAGS) $(PDFLAGS)

$(EXECUTABLE_PAR): MACROFLAGS += -DJACOBIPAR #define the macro that changes between sequential and parallel
$(EXECUTABLE_PAR): $(CFILES)
	@mkdir -p build
	$(CC) $(MACROFLAGS) $(CFLAGS) $(INC_PARAMS) -o $@ $^ $(LDFLAGS) $(PDFLAGS)

clean:
	@rm -f $(ZIPFILE)
	@rm -rf build/
	@rm -rf out/*.csv

zip: clean
	$(ZIP) jacobi $(ZIPFILE) ./scripts/* ./src/* ./Makefile ./README.md ./relatorio.pdf

run: $(EXECUTABLE_SEQ) $(EXECUTABLE_PAR)
	@echo "Running Sequential Version:"
	@time ./$(EXECUTABLE_SEQ) $(ARGS)
	@echo ""
	@echo "Running Parallel Version:"
	@time ./$(EXECUTABLE_PAR) $(ARGS)
	@echo ""

run_seq: $(EXECUTABLE_SEQ)
	@time ./$(EXECUTABLE_SEQ) $(ARGS)

run_par: $(EXECUTABLE_PAR)
	@time ./$(EXECUTABLE_PAR) $(ARGS)

#debug versions
debug: CFLAGS += -g
debug: clean
debug: $(EXECUTABLE_SEQ) $(EXECUTABLE_PAR)

debug_seq: CFLAGS += -g
debug_seq: clean
debug_seq: $(EXECUTABLE_SEQ)

debug_par: CFLAGS += -g
debug_par: clean
debug_par: $(EXECUTABLE_PAR)

#run valgrind to check for memory leaks
valgrind_seq: $(EXECUTABLE_SEQ)
	valgrind $(VDFLAGS) ./$(EXECUTABLE_SEQ)

valgrind_par: $(EXECUTABLE_PAR)
	valgrind $(VDFLAGS) ./$(EXECUTABLE_PAR)