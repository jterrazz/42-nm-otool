/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 20:50:36 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void destroy_file(t_file *file)
{
	if (file->mysyms)
		ft_lstdel(&file->mysyms, NULL);
		// mysym->name
	if (file->mysects)
		ft_lstdel(&file->mysects, NULL);
}

void create_file(t_file *file, char const *name, uint64_t size, void *start)
{
	ft_bzero(file, sizeof(t_file));
	file->filename = name;
	file->filesize = size;
	file->start = start;
	file->error = E_NULL;
	file->swap_bits = FALSE;
	file->end = file->start + file->filesize;
}

void create_virtual_file(t_file *file, char *virtualname)
{
	file->virtualname = virtualname;
	file->is_virtual = TRUE;
}
