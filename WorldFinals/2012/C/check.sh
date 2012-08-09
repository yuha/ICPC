#!/bin/zsh

for i in bustour/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
