/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overflow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 09:20:31 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 09:39:12 by jterrazz         ###   ########.fr       */
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
