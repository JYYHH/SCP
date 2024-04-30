make
rm log/*
echo $2 | ./bin/purdec 8888 > log/dec.log &
sleep 0.4
cd data
echo $2 | ./../bin/purenc $1 -d 127.0.0.1:8888 > ../log/enc.log
cd ..
sleep 1
diff $1 ./data/$1
if [ $? -eq 0 ]; then
    echo '--------------- You pass the test! ------------'
else
    echo '---Something is going wrong... please check it!---'
fi
rm $1*
sleep 0.3
kill -2 $(ps | grep purdec | awk '{print $1;}') # send SIGINT to the server running in the background
make clean