make
rm log/*
echo $2 | ./bin/purdec 8888 > log/dec.log &
sleep 0.4
cp data/$1 .
mv $1 test_data_1
cp test_data_1 test_data_2
cp test_data_1 test_data_3
cp test_data_1 test_data_4

echo $2 | ./bin/purenc test_data_1 -d 127.0.0.1:8888 > ./log/enc_1.log &
echo $2 | ./bin/purenc test_data_2 -d 127.0.0.1:8888 > ./log/enc_2.log &
echo $2 | ./bin/purenc test_data_3 -d 127.0.0.1:8888 > ./log/enc_3.log &
echo $2 | ./bin/purenc test_data_4 -d 127.0.0.1:8888 > ./log/enc_4.log &

sleep 1

for i in {1..4}
do
    diff test_data_$i ./data/$1
    if [ $? -eq 0 ]; then
        echo '--------------- You pass the test! ------------'
    else
        echo '---Something is going wrong... please check it!---'
    fi
done

rm test_data*
sleep 0.3
kill -2 $(ps | grep purdec | awk '{print $1;}') # send SIGINT to the server running in the background
make clean