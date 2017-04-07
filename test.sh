#!/bin/bash

for function in "sort" "scan" "focus"; do
	echo $function
	for algorithm in "rand" "fifo" "custom"; do
		echo $algorithm
		for nframes in $(seq 3 100); do
			./virtmem 100 $nframes $algorithm $function >> "$function-$algorithm.dat"
		done
	done
done
