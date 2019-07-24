/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overflow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 09:20:31 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 18:11:18 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

t_bool check_overflow_wo_error(t_file *file, void *ptr)
{
	if ((ptr > file->start + file->filesize) || (ptr > file->end) || (ptr < file->start))
		return (TRUE);
	return (FALSE);
}

t_bool check_overflow(t_file *file, void *ptr)
{
	if (check_overflow_wo_error(file, ptr) == TRUE) {
		file->error = E_OVERFLOW;
		return (TRUE);
	}
	return (FALSE);
}

char	*ft_strdup_overflow(t_file *file, char *src, char end_char, int *failed)
{
	size_t		size;
	size_t		i;
	char		*str;

	i = 0;
	size = 0;
	while (!check_overflow_wo_error(file, src + size)
		&& src[size] && src[size] != end_char && ft_isprint(src[size]))
		size++;
	if (check_overflow_wo_error(file, src + size))
	{
		*failed = 1;
		return (ft_strdup("bad string index"));
	}
	if ((str = (char *)malloc(sizeof(*str) * (size + 1 + 1))) == NULL)
		return (NULL);
	while (i < size)
	{
		str[i] = src[i];
		i++;
	}
	if (src[size] == end_char)
		str[i++] = end_char;
	str[i] = 0;
	return (str);
}
