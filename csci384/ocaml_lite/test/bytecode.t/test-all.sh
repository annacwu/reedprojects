#!/bin/sh

count=0
cd "$1"
for f in *.ol; do
    exp_out="${f%.ol}.out"
    ocaml_lite compile $f
    ../../../vm/ovm "${f%.ol}.bc" > comp_out
    diff "$exp_out" comp_out
    if [ $? -ne 0 ]; then
        echo "FAILURE: $f"
        ((count++))
    fi
done
echo "$count failures"
