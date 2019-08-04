#!/bin/sh

make -B

fn="./test/simple1"

rm -f "$fn.bb"

word=ana

if test "$#" -ge 1
then
    word=$1
fi


#time ./rlebwt -m $fn ./index ana
./rlebwt -m $fn ./index "$word"

./rlebwt_test -m "$fn" index "$word" >rt


#time valgrind --tool=massif --pages-as-heap=yes  ./rlebwt  -m $fn  ./index  "in"


if [[ "$fn" =~ "simple" ]]
then
   xxd -b "$fn.b"

   #xxd -b "$fn.bb"

   xxd -b "$fn.bb1"
fi

diff "$fn.bb" "$fn.bb1"