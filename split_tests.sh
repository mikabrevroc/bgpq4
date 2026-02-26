#!/bin/bash
cd /Users/mabrahamsson/src/reverse-as-set/bgpq4

# Split comprehensive test into multiple files
head -90 tests/test_rasa_comprehensive.c > tests/test_rasa_part1.c
sed -n '91,600p' tests/test_rasa_comprehensive.c >> tests/test_rasa_part1.c

echo "Part 1 created: $(wc -l < tests/test_rasa_part1.c) lines"

# Compile and test part 1
gcc -c -I. tests/test_rasa_part1.c -o tests/test_rasa_part1.o 2>&1 | head -10
if [ -f tests/test_rasa_part1.o ]; then
    gcc tests/test_rasa_part1.o rasa.o -L/opt/homebrew/lib -ljansson -o test_rasa_part1 2>&1
    if [ -f test_rasa_part1 ]; then
        ./test_rasa_part1
    else
        echo "Link failed for part 1"
    fi
else
    echo "Compile failed for part 1"
fi
