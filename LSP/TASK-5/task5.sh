#!/bin/bash

word="example"
file="sample.txt"

grep -i "$word" "$file" | sort

