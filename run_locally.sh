#!/bin/sh

# Ищем в стандартных местах
for prefix in /usr /usr/local; do
    DATA_FILE="$prefix/share/AB_NYC_2019.csv"
    if [ -f "$DATA_FILE" ]; then
        break
    fi
done


DATA_FILE=/usr/local/share/AB_NYC_2019.csv

cat ${DATA_FILE} | mapper | sort -k1 | reducer1 > output1
echo -n "average:"
cat output1

cat ${DATA_FILE} | mapper | sort -k1 | reducer2 > output2
echo -n "dispersion:"
cat output2
