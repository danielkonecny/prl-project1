# Project: 		Pipeline Merge Sort
# File: 		Makefile
# Version: 		1.0
# Course: 		PRL - Parallel and Distributed Algorithms
# Organisation:	Brno University of Technology - Faculty of Information Technology
# Author: 		Daniel Konecny (xkonec75)
# Date: 		09. 04. 2021

# Macros
MPI = mpic++
SUFFIX = cpp
PFLAGS = -Wall -Wextra -pedantic
PROJECT = pms
LOGIN = xkonec75
TEST_FILE = test.sh
NUMBER_FILE = numbers
NUMBER_COUNT = 16
PROCESS_COUNT = 5

all: $(PROJECT)

run: $(PROJECT)
	dd if=/dev/random bs=1 count=$(NUMBER_COUNT) of=$(NUMBER_FILE) 1>/dev/null 2>/dev/null
	mpirun --use-hwthread-cpus --oversubscribe -np $(PROCESS_COUNT) $^
	rm -f $(NUMBER_FILE)

test:
	sh $(TEST_FILE)
	
clean:
	rm $(PROJECT) $(NUMBER_FILE)

pack:
	zip -r $(LOGIN).zip $(PROJECT).$(SUFFIX) $(PROJECT).h $(LOGIN).pdf $(TEST_FILE)

$(PROJECT): $(PROJECT).$(SUFFIX) $(PROJECT).h
	$(MPI) -o $@ $<
