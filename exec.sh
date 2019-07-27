#!/bin/sh

make -B

./rlebwt -m ./test/shopping ./index ana
