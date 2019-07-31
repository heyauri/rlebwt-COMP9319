#!/bin/sh

make -B

fn="./test/simple3"

rm -f "$fn.bb"

./rlebwt -m $fn ./index ana

 xxd -b "$fn.b"

 xxd -b "$fn.bb1"