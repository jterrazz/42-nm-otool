nm "$@" > tmp_official
./ft_nm "$@" > tmp_mine
diff tmp_official tmp_mine
