#!/bin/bash
set -e
cd /Users/mabrahamsson/src/reverse-as-set/bgpq4

# Compile rasa.c
echo "Compiling rasa.c..."
gcc -I. -I/opt/homebrew/include -g -O2 -c rasa.c -o /tmp/rasa_final.o

# Compile test
echo "Compiling test_rasa_comprehensive.c..."
gcc -I. -I/opt/homebrew/include -g -O2 -c tests/test_rasa_comprehensive.c -o /tmp/test_comp_final.o -Wno-implicit-function-declaration

# Link
echo "Linking..."
gcc /tmp/test_comp_final.o /tmp/rasa_final.o -L/opt/homebrew/lib -ljansson -o test_rasa_comprehensive

echo "Build successful!"
./test_rasa_comprehensive
