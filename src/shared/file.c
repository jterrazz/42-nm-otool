/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 19:03:26 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void destroy_file(t_file *file)
{
	if (file->mysyms)
		ft_lstdel(&file->mysyms, NULL);
	if (file->mysects)
		ft_lstdel(&file->mysects, NULL);
}

void init_file(t_file *file, char const *name, uint64_t size, void *start)
{
	ft_bzero(file, sizeof(t_file));
	file->filename = name;
	file->filesize = size;
	file->start = start;
	file->error = E_NULL;
	file->swap_bits = FALSE;
	file->end = file->start + file->filesize;
}

void init_virtual_file(t_file *file, t_file *old_file, char *virtualname)
{
	file->end = file->end < old_file->end ? file->end : old_file->end; // probably delete
	file->virtualname = virtualname;
	file->is_virtual = TRUE;
}
