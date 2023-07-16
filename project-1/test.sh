#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "Usage: ./test.sh <program> <tests directory>"
	exit 1
fi

prog=$1
dir=$2

for file in $dir/*.in; do
	echo -n "Testing $file ..." $'\t'
	valgrind --error-exitcode=123 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --log-fd=3 ./$prog < "$file" 1> $prog.out 2> $prog.err 3> $prog.valgrind
	
	if [[ -f "${file%in}out" ]] && ! diff -b $prog.out "${file%in}out" > /dev/null; then
		echo "Wrong output!"
		break
	fi
	if [[ -f "${file%in}err" ]] && ! diff -b $prog.err "${file%in}err" > /dev/null; then
		echo "Wrong error!"
		break
	fi
	if ! grep -q "0 errors" $prog.valgrind; then
		echo "Memory leak!"
		break
	fi
	
	echo "OK"
done

rm -f $prog.out $prog.err $prog.valgrind
