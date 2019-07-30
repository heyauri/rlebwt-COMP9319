#!/bin/sh

make -B

fn="./test/simple2"

rm -f "$fn.bb"

./rlebwt -m $fn ./index ana

 xxd -b "$fn.bb"

 xxd -b "$fn.bb1"