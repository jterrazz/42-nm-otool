/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 08:40:53 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void init_file(t_file *file, char const *name, uint64_t size, void *start)
{
	ft_bzero(file, sizeof(t_file));
	file->filename = name;
	file->filesize = size; // announced filesize
	file->start = start;
	file->error = E_NULL;
	file->swap_bits = FALSE;
	file->end = file->start + file->filesize; // end also checking for larger data
}

void init_virtual_file(t_file *file, t_file *old_file, char *virtualname)
{
	file->end = file->end < old_file->end ? file->end : old_file->end;
	file->virtualname = virtualname;
}
