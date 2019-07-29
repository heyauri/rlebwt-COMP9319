#!/bin/sh

make -B

fn="./test/simple1"

rm -f "$fn.bb"

./rlebwt -m $fn ./index ana

 xxd -b "$fn.bb"

 xxd -b "$fn.bb1"