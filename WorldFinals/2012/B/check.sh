#!/bin/zsh

for i in 005 006 007 008 009 010 011 012 013 014 015 016 017; do
    echo $i
    ./a.out < ./bottle/bottle-$i.in | diff - ./bottle/bottle-$i.ans;
done
