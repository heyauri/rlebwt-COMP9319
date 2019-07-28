#!/bin/sh

make -B

#rm -f ./test/simple1.bb

./rlebwt -m ./test/simple1 ./index ana

# xxd -b ./test/simple1.bb

# xxd -b ./test/simple1.bb1