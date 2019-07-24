#!/bin/sh
mkdir /tmp/__nm_tests 2> /dev/null
for it in $@
do
	../ft_nm $it | cat -e > /tmp/__nm_tests/our
	nm $it | cat -e > /tmp/__nm_tests/their
	diff /tmp/__nm_tests/our /tmp/__nm_tests/their > /tmp/__nm_tests/"$it"_diff
	if [[ -s /tmp/__nm_tests/"$it"_diff ]] ;
	then echo "$it: FAILURE" ;
	else echo "$it: SUCCESS" ;
	fi
done
