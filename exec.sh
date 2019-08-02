#!/bin/sh

make -B

fn="./test/simple3"

rm -f "$fn.bb"

time ./rlebwt -m $fn ./index ana
#./rlebwt -m $fn ./index ana

#time valgrind --tool=massif --pages-as-heap=yes  ./rlebwt  -m $fn  ./index  "in"


if [[ "$fn" =~ "simple" ]]
then
   xxd -b "$fn.b"

   xxd -b "$fn.bb"

   xxd -b "$fn.bb1"
fi

diff "$fn.bb" "$fn.bb1"