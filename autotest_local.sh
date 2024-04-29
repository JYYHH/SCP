make
./bin/purenc ./data/$1 -l
mv ./data/$1.pur .
./bin/purdec -l $1.pur
diff $1 ./data/$1
if [ $? -eq 0 ]; then
    echo '--------------- You pass the test! ------------'
else
    echo '---Something is going wrong... please check it!---'
fi
rm $1*
make clean