#!/bin/zsh

for i in safe/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
