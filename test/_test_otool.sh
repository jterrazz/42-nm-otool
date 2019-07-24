#!/bin/sh
mkdir /tmp/__otool_tests 2> /dev/null
for it in $@
do
	../ft_otool $it | cat -e > /tmp/__otool_tests/our
	otool -t $it | cat -e > /tmp/__otool_tests/their
	diff /tmp/__otool_tests/our /tmp/__otool_tests/their > /tmp/__otool_tests/"$it"_diff
	if [[ -s /tmp/__otool_tests/"$it"_diff ]] ;
	then echo "$it: FAILURE" ;
	else echo "$it: SUCCESS" ;
	fi
done
