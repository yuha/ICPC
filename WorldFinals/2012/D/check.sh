#!/bin/zsh

for i in fibonacci/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
