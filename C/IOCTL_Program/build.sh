cls() {
    echo "clear!"
    cd build
    rm -rf ./*
    rmmod ko_test.ko
    dmesg -c
}

compile() {
    echo "begin compile"
    cd build
    cmake ..
    make
    insmod ko_test.ko
    echo "end compile"
}

if [ $1 = "cls" ]; then
    cls
elif [ $1 = "compile" ]; then
    compile
fi