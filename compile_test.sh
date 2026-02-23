#!/bin/bash
cd /Users/mabrahamsson/src/reverse-as-set/bgpq4
gcc -I. -I/opt/homebrew/include -c tests/test_rasa_comprehensive.c -o /tmp/test_comp.o 2>&1
