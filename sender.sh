make
rm log/*

cp data/$1 .
mv $1 test_data_1
cp test_data_1 test_data_2
cp test_data_1 test_data_3
cp test_data_1 test_data_4

echo $2 | ./bin/purenc test_data_1 -d $3:8888 > ./log/remote_enc_1.log &
echo $2 | ./bin/purenc test_data_2 -d $3:8888 > ./log/remote_enc_2.log &
echo $2 | ./bin/purenc test_data_3 -d $3:8888 > ./log/remote_enc_3.log &
echo $2 | ./bin/purenc test_data_4 -d $3:8888 > ./log/remote_enc_4.log &

rm test_data*
make clean
