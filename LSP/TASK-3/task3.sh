#!/bin/bash

greeting="Hello, World!"
echo "Enter integer: "
read number

if [ $number -gt 0 ]
then
    echo "$greeting The number is positive."
elif [ $number -eq 0 ]
then
	echo "$greeting The number is zero."
else
    echo "$greeting The number is negative."
fi

