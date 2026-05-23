#!/bin/sh

RED="\e[0;31m"
GRN="\e[0;32m"
YEL="\e[0;33m"
WHI="\e[0;37m"

filename=$1
# ! WARNING: The following code must not be modified.
# It prevents the user to delete by accident the script file.
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

command1="./src/tc --llvm-gc-debug --llvm-gc-runtime-link --llvm-runtime-display --llvm-display tests/gc/$filename.tig > $filename.ll"
echo "$command1"
eval "$command1"
if [ "$(cat $filename.ll)" = "" ]; then
    echo -e "${RED}tc program failed!$WHI"
    rm -f "$filename.ll"
    exit 1
fi

command2="clang $filename.ll -o$filename"
echo "$command2"
eval "$command2" 2>/dev/null
if [ ! -f "$filename" ]; then
    echo -e "${RED}Compilation with clang failed!$WHI"
    # Delete created trashfiles
    rm -f "$filename.ll"
    exit 1
fi

command3="valgrind --leak-check=full ./$filename"
echo "$command3"
object_freed="$(eval $command3 3> /tmp/tc-debug-objects.txt 2> /tmp/tc-debug-result.txt; cat /tmp/tc-debug-objects.txt; rm /tmp/tc-debug-objects.txt)"
valgrind_result="$(cat /tmp/tc-debug-result.txt; rm /tmp/tc-debug-result.txt)"
leak_result="$(echo $valgrind_result | grep 'All heap blocks were freed -- no leaks are possible')"


# Check if any leaks occured
if [ "$leak_result" = "" ]; then
    echo -e "${RED}Test failed. There is at least one memory leak!$WHI"
    # Delete created trashfiles
    rm -f "$filename" "$filename.ll"
    exit 1
fi

# No freed object
if [ "$object_freed" = "" ]; then
    echo "No freed object or debug line in comment in tiger_gc runtime function"
# Print freed objects
else
    echo
    echo -e "${YEL}======================= Objects freed =======================${WHI}"
    echo "$object_freed"
fi

# Delete created trashfiles
rm -f "$filename" "$filename.ll"

exit 0
