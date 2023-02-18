echo "PtUpdater Compile."

rm -rf out

make clean
echo "Building PtUpdater for ARM."
cc_exec="~/buildroot/output/host/bin/arm-linux-gcc"
lflags=" -L/~/buildroot/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/lib"
libs=" -lxml2 -lm -ldl -lssl -lcrypto -lb64 -lz"
includes=" -I/~/buildroot/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/include"
if ! make all CC="$cc_exec" LFLAGS="$lflags" LIBS="$libs" INCLUDES="$includes"; then
    echo "ERROR: PtUpdater build for ARM failed." >&2
    exit 1
fi

echo "Copying PtTools binary for ARM to the 'out' directory."
mkdir -p out/arm
cp bin/ptupdater out/arm/.

make clean
echo "Building PtTools for Intel (x86-64)."
cc_exec="gcc"
lflags=" --static"
libs=" -lxml2 -lm -ldl -lssl -lcrypto -lb64 -lz -llzma"
includes=""
if ! make all CC="$cc_exec" LFLAGS="$lflags" LIBS="$libs" INCLUDES="$includes"; then
    echo "ERROR: PtUpdater build for Intel failed." >&2
    exit 1
fi

echo "Copying the PtUpdater binary for Intel to the 'out' directory."
mkdir -p out/intel
cp bin/ptupdater out/intel/.
