echo "compile target : $1, test target : $2 param cnt : $#"
cmake  -S . -B build
cmake --build build --target $1 --config Debug

echo "`n=============================================`n"
echo "begin execute target : $1"
echo "`n=============================================`n"
if [ $# -eq 1 ]; then
    ./bin/Debug/$1.exe
else
    ./bin/Debug/$1.exe --gtest_filter=*.$2
fi