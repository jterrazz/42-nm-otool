otool "$@" > tmp_official
./ft_otool "$@" > tmp_mine
diff tmp_official tmp_mine
