#!/usr/bin/bash
POINTS=(
        5
        5
        2 2 3 3
        2 3 3
        3 3
        3 3
        2 2 2 3
        2 2 2 3
        2 2 3
        2 2 3
        2 2 3
        2 2 2 2 2 2 2 2 
        )
TESTS=${#POINTS[@]}
TOTAL=100
TOTALTESTS=0 
VALGRINDPOINTS=0

GREEN=$(tput setaf 2)
RED=$(tput setaf 1)
RESET=$(tput sgr0)

function readme() {
    if test -f "README" || test -f "README.md"
        then
            echo "README: 5/5"
            let TOTALTESTS+=5
    else
        echo "README: 0/5"
    fi
}

function checkExe() {
    if test -f "tema1"
        then
            main
    else
        echo "The executable file needs to be named 'tema1'"
    fi
}

function valgrindTest() {   
    local file_name="$1"

    valgrind --leak-check=full --track-origins=yes -q --log-file=rezultat_valgrind.txt ./tema1 < tema1.in > /dev/null

    if [ ! -s rezultat_valgrind.txt ]; then
        echo "  Valgrind ${GREEN}PASSED${RESET}"
        VALGRIND_PER_TEST=$(echo "scale=4; 20.0 / $TESTS" | bc)
        VALGRINDPOINTS=$(echo "$VALGRINDPOINTS + $VALGRIND_PER_TEST" | bc)
    else
        echo "  Valgrind ${RED}FAILED${RESET}"
fi
}

function testInput() {
    local file="$1"
    local test_index="$2"
    local file_name
    file_name=$(basename "$file")
    local ref_file="tests/ref/${file_name%.in}.ref"

    cat "$file" > tema1.in
    timeout 10s ./tema1 < tema1.in > tema1.out

    if [ "$?" -eq 139 ]; then
        echo "$file_name: segmentation fault"
        return
    fi

    if ! diff -w -B "$ref_file" tema1.out > /dev/null 2>&1; then
        echo "$file_name: 0/${POINTS[$test_index-1]}"
    else
        echo "$file_name: ${POINTS[$test_index-1]}/${POINTS[$test_index-1]}"
        let TOTALTESTS+=${POINTS[$test_index-1]}
        valgrindTest "$file_name"
    fi
}

function makeCommand() {
    make 1> /dev/null
    if [ "$?" -ne 0 ]
        then
            exit 1
    fi
}

function main() {
    echo " -= TEMA 1 SDA =-"
    echo

    touch tema1.in
    touch tema1.out

    readme

    local test_index=1
    for file in $(ls tests/input/*.in | sort); do
        testInput "$file" "$test_index"
        ((test_index++))
    done
    
    echo
    echo "Total: $TOTALTESTS/$TOTAL"

    VALGRINDPOINTS_ROUNDED=$(printf "%.0f" "$VALGRINDPOINTS")
    echo "Valgrind: $VALGRINDPOINTS_ROUNDED/20"
}
makeCommand
checkExe