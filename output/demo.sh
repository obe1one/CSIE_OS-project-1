#! /usr/bin/env bash

tasks=("FIFO" "SJF" "PSJF" "RR")

for task in ${tasks[@]}; do
	for ((i = 1; i <= 5; i++)); do
		outfile="$task"_"$i"_stdout.txt
		touch $outfile
		infile=../OS**/"$task"_"$i".txt
		sudo ../main <$infile >$outfile
		outfile="$task"_"$i"_dmesg.txt
		touch $outfile
		dmesg | grep Project1 > $outfile
	done
done
