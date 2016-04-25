#!/bin/sh

BASEDIR=$(dirname $0)
gcc $BASEDIR/main.c $BASEDIR/bplus_tree.c $BASEDIR/bplus_tree_io.c -o main.o
./main.o