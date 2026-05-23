#!/bin/sh

filename="$1"

current_filename="$(echo ${0##*/})"
if [ "$filename" = "$current_filename" ]; then
    echo -e "${RED}This is the shell script name!$WHI"
    exit 1
fi
if [ "$filename" = "" ]; then
    echo -e "${RED}There is no filename$WHI"
    exit 1
fi

# Delete old created trashfiles
rm -f "$filename" "$filename.ll"

# Generate LLVM Code
./src/tc --llvm-gc-debug --llvm-gc-runtime-link --llvm-runtime-display --llvm-display tests/gc/"$filename".tig > "$filename".ll
if [ "$(cat $filename.ll)" = "" ]; then
    echo -e "${RED}tc program failed!$WHI"
    rm -f "$filename.ll"
    exit 1
fi

# Generate executable from LLVM with clang
clang "$filename".ll -o"$filename"
if [ ! -f "$filename" ]; then
    echo -e "${RED}Compilation with clang failed!$WHI"
    # Delete created trashfiles
    rm -f "$filename.ll"
    exit 1
fi

# Run executable
./"$filename"

exit 0