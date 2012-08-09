#!/bin/zsh

for i in stacking/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
