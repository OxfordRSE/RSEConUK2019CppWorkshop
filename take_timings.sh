#!/bin/bash

builddir=$1
checkpoint=$2

n=100
FILE=$builddir/checkpoint_${checkpoint}_chrono
if [ ! -f "$FILE" ]; then
    echo "Error: Could not find file $FILE."
    exit 1
fi

rm -i timings_$checkpoint.csv
echo "write_data,reduce,transform_reduce,nth_element,any_of,mismatch" >> timings_$checkpoint.csv
for i in $(seq 1 $n)
do
    ./$FILE
done
	 
	 
	 
	 
