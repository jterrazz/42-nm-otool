/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overflow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 09:20:31 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 20:12:05 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

t_bool check_overflow(t_file *file, void *ptr)
{
	if ((ptr > file->start + file->filesize) || (ptr > file->end) || (ptr < file->start)) {
		file->error = E_OVERFLOW;
		return TRUE;
	}
	return FALSE;
}

char	*ft_strdup_overflow(t_file *file, char *s1, char c, t_bool inc_c, int *failed)
{
	char	*str;
	size_t		size;
	size_t		i;

	i = 0;
	size = 0;
	while (!check_overflow(file, s1 + size) && s1[size] && s1[size] != c)
		size++;
	if (check_overflow(file, s1 + size)) {
		*failed = 1;
		return ft_strdup("bad string index");
	}
	if ((str = (char *)malloc(sizeof(*str) * (size + 1 + inc_c))) == NULL)
		return (NULL);
	while (s1[i] && s1[i] != c)
	{
		str[i] = s1[i];
		i++;
	}
	if (inc_c && s1[size] == c){
		str[i] = c;
		str[i + 1] = 0;
	} else
		str[i] = 0;
	return (str);
}
