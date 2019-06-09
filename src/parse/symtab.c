/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 00:03:36 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 00:04:25 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

static void build_symbol(char *symname, void *symtab, t_arch arch) {
	t_symbol symbol;

	symbol.name = symname;
	if (arch == ARCH_32) {
		symbol.type = ((t_nlist *)symtab)->n_type;
		symbol.sect = ((t_nlist *)symtab)->n_sect;
		symbol.desc = ((t_nlist *)symtab)->n_desc;
		symbol.value = ((t_nlist *)symtab)->n_value;
	} else {
		symbol.type = ((t_nlist_64 *)symtab)->n_type;
		symbol.sect = ((t_nlist_64 *)symtab)->n_sect;
		symbol.desc = ((t_nlist_64 *)symtab)->n_desc;
		symbol.value = ((t_nlist_64 *)symtab)->n_value;
	}
	ft_printf("%s\n", symname);
	(void) symtab;
}

int parse_symtab(t_file *file, t_symtab_command *symtab_command, t_arch arch) { // No need for 64 ?
	void *symtab;
	void *strtab;
	size_t nsyms;
	size_t i;

	strtab = (void *) file->start + symtab_command->stroff;
	symtab = (void *) file->start + symtab_command->symoff;
	nsyms = symtab_command->nsyms;
	i = 0;
	while (i < nsyms) {
		if (arch == ARCH_32)
			build_symbol(strtab + ((t_nlist *)symtab + i)->n_un.n_strx, (t_nlist *)symtab + i, arch);
		else
			build_symbol(strtab + ((t_nlist_64 *)symtab + i)->n_un.n_strx, (t_nlist_64 *)symtab + i, arch);
		i++;
	}

	return SUCCESS;
}
