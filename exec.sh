#!/bin/sh

make -B

fn="./test/simple2"

rm -f "$fn.bb"

./rlebwt -m $fn ./index ana

#time valgrind --tool=massif --pages-as-heap=yes  ./rlebwt  -m $fn  ./index  "in"

 xxd -b "$fn.bb"

 xxd -b "$fn.bb1"