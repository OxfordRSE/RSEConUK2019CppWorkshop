#!/bin/bash

checkpoint=$1

n=10
FILE=$checkpoint/${checkpoint}
if [ ! -f "$FILE" ]; then
    echo "Error: Could not find file $FILE."
    exit 1
fi

cd $checkpoint
rm -i timings.csv
echo "read_data,reduce,transform_reduce,nth_element,any_of,mismatch" >> timings.csv
for i in $(seq 1 $n)
do
    ./$checkpoint
done
cd ../
	 
	 
	 
	 
