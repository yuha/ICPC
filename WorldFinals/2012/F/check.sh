#!/bin/zsh

for i in keys/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
