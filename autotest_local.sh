make
./bin/purenc ./data/$1 -l
mv ./data/$1.pur .
./bin/purdec -l $1.pur
diff $1 ./data/$1
rm $1*
make clean