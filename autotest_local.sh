make
cd bin
./purenc purenc -l
cd ..
mv ./bin/purenc.pur .
./bin/purdec -l purenc.pur
diff purenc ./bin/purenc
rm pur*
make clean