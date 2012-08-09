#!/bin/zsh

for i in takeover/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
