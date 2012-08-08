#!/bin/zsh

for i in room/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
