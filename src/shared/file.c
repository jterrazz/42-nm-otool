/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 12:36:22 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static void	destroy_my_sects(void *content, size_t content_size)
{
	free(content);
	(void)content_size;
}

static void	destroy_my_syms(void *content, size_t content_size)
{
	t_mysymbol *sym;

	sym = content;
	if (sym->name)
		free(sym->name);
	sym->name = NULL;
	free(content);
	(void)content_size;
}

void		destroy_file(t_file *file)
{
	if (file->mysyms)
		ft_lstdel(&file->mysyms, &destroy_my_syms);
	if (file->mysects)
		ft_lstdel(&file->mysects, &destroy_my_sects);
}

void		create_file(t_file *file, char const *name,
	uint64_t size, void *start)
{
	ft_bzero(file, sizeof(t_file));
	file->filename = name;
	file->filesize = size;
	file->start = start;
	file->error = E_NULL;
	file->swap_bits = FALSE;
	file->end = file->start + file->filesize;
}

int			create_virtual_file(t_file *file,
	t_file *old_file, char const *virtualname, t_file_type type)
{
	file->virtualname = virtualname;
	file->virtualtype = type;
	if (check_overflow(old_file, file->start)
		|| check_overflow(old_file, file->end))
	{
		file->error = E_OVERFLOW;
		return (FAILURE);
	}
	return (SUCCESS);
}
