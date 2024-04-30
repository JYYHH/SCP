make
echo $2 | ./bin/purdec 8888

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
make clean
