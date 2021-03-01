# Author: Marco Marasco 834482.
# Description: Makefile for COMP90077 Assignment 2.

CC = g++
CFLAGS  = -O3


construction: ConstructionExperiment.o
	$(CC) $(CFLAGS) -o construction ConstructionExperiment.o

construction.o: ConstructionExperiment.cpp Generator.h RangeTreeOrg.h
	$(CC) $(CFLAGS) -c ConstructionExperiment.cpp

query1: QueryExperiment1.o
	$(CC) $(CFLAGS) -o query1 QueryExperiment1.o

query1.o: QueryExperiment1.cpp Generator.h RangeTreeOrg.h RangeTreeFC.h
	$(CC) $(CFLAGS) -c QueryExperiment1.cpp

query2: QueryExperiment2.o
	$(CC) $(CFLAGS) -o query2 QueryExperiment2.o

query2.o: QueryExperiment2.cpp Generator.h RangeTreeOrg.h RangeTreeFC.h
	$(CC) $(CFLAGS) -c QueryExperiment2.cpp

