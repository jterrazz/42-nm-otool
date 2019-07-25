#!/bin/bash
i=42
while [ $i -gt 0 ]; do
	echo "Truncating $i lines :"
	head -n $i $@ > bin
	./ft_nm bin
	let "i=i-1"
done
