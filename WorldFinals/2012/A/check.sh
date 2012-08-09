#!/bin/zsh

for i in asteroids/*.in; do
    echo $i
    ./a.out < $i | diff - ${i%.in}.ans
done
