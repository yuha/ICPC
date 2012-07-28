#!/bin/zsh

for i in 001 002 003 004 005 006 007 008 009 010 011 012; do
    echo $i
    ./a.out < ./infiltration/infiltration-$i.in | diff - ./infiltration/infiltration-$i.ans;
done
