#!/bin/sh
INPUT=/usr/local/share/AB_NYC_2019.csv

cat ${INPUT} | mapper | sort -k1 | reducer1 > output1
echo -n "average:"
cat output1

cat ${INPUT} | mapper | sort -k1 | reducer2 > output2
echo -n "dispersion:"
cat output2
