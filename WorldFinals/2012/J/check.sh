#!/bin/zsh

for i in shortest/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
