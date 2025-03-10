if [ $# != 1 ];
then
    echo "Invalid argument!"
    exit 1
fi

script_dir="$(cd "$(dirname "$0")" && pwd)"
echo "Script is located in: $script_dir"

# if [ -e "lib/gtest/" ] && [ -e "include/gtest" ]; then
#     echo "gtest exist "
# else
#     sh ../Gtest/build.sh $script_dir
# fi

# if [ -e "lib/util/" ] && [ -e "include/util/" ]; then
#     echo "util exist "
# else
#     sh ../Util/build.sh $script_dir
# fi

target=$1

echo "compile target - ${target} "

cmake -Dtarget::STRING=${target} -S . -B build
cmake --build build --target ${target}

echo -e "\n\n==================RUN================="
./bin/${target}
