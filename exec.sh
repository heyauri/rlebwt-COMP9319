#!/bin/sh

make -B

fn="./test/dblp"

#rm -f "$fn.bb"

word=ana

if test "$#" -ge 1
then
    word=$1
fi


time ./rlebwt -a $fn ./index "\"$word\""
#./rlebwt -m $fn ./index "$word"

#time ./rlebwt_test -m "$fn" index "$word" >rt

#cat "$fn.txt"
egrep -o "$word" "$fn.txt" |wc -w

#time valgrind --tool=massif --pages-as-heap=yes  ./rlebwt  -m $fn  ./index  "in"


if [[ "$fn" =~ "simple" ]]
then
   xxd -b "$fn.b"

   #xxd -b "$fn.bb"

   xxd -b "$fn.bb1"
fi

diff "$fn.bb" "$fn.bb1"