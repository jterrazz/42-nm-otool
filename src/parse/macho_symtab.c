/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_symtab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 00:03:36 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 20:08:56 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static t_list *create_mysym(t_file *file, char *strtab, void *sym)
{
	t_mysymbol mysym;
	char *symname;

	symname = strtab + swapif_u32(file, (file->arch == ARCH_32) ? ((t_nlist *)sym)->n_un.n_strx : ((t_nlist_64 *)sym)->n_un.n_strx);
	if (!(init_mysym(file, &mysym, symname, (t_nlist *)sym)))
		return (NULL);
	fill_mysym(file, &mysym);

	return (ft_lstnew(&mysym, sizeof(t_mysymbol)));
}

/*
** Parse the load command LC_SYMTAB.
*/

int parse_macho_symtab(t_file *file, t_symtab_command *symtab_command)
{
	void *sym;
	uint64_t sym_size;
	void *strtab;
	uint64_t nsyms;
	t_list *lst;

	if (check_overflow(file, (void *)symtab_command + sizeof(t_symtab_command *)))
		return (FAILURE);
	strtab = (void *) file->start + swapif_u32(file, symtab_command->stroff);
	sym = (void *) file->start + swapif_u32(file, symtab_command->symoff);
	nsyms = swapif_u32(file, symtab_command->nsyms);
	sym_size = ((file->arch == ARCH_32) ? sizeof(t_nlist) : sizeof(t_nlist_64));

	while (nsyms--)
	{
		if (check_overflow(file, strtab) || check_overflow(file, sym))
			return (FAILURE);
		if (!(lst = create_mysym(file, strtab, sym)))
			return (FAILURE);
		ft_lstadd(&file->mysyms, lst);
		sym += sym_size;
	}

	return (SUCCESS);
}
