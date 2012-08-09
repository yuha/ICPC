#!/bin/zsh

for i in minflow/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
