/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symtab.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 00:03:36 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 13:08:26 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

static void init_mysym(t_symbol *mysym, char *symname, void *sym, t_arch arch) {
	ft_bzero(mysym, sizeof(t_symbol *));
	mysym->type_p = ' ';
	mysym->name = symname;
	if (arch == ARCH_32) {
		mysym->type = ((t_nlist *)sym)->n_type;
		mysym->sect = ((t_nlist *)sym)->n_sect;
		mysym->desc = ((t_nlist *)sym)->n_desc;
		mysym->value = ((t_nlist *)sym)->n_value;
	} else {
		mysym->type = ((t_nlist_64 *)sym)->n_type;
		mysym->sect = ((t_nlist_64 *)sym)->n_sect;
		mysym->desc = ((t_nlist_64 *)sym)->n_desc;
		mysym->value = ((t_nlist_64 *)sym)->n_value;
	}
}

static t_mysection *find_mysection(t_list *lst, uint8_t n_sect) {
	while (lst && lst->content) {
		if (((t_mysection *)lst->content)->index == n_sect) {
			return lst->content;
		}
		lst = lst->next;
	}

	return NULL;
}

static void match_sym_section(t_list *mysect_lst, t_symbol *mysym) {
	t_mysection *mysect;

	if ((mysect = find_mysection(mysect_lst, mysym->sect))) {
		// __data
		// __const
		// __common
		if (!ft_strcmp(mysect->name, SECT_TEXT))
			mysym->type_p = 'T'; // Never found
		else if (!ft_strcmp(mysect->name, SECT_DATA))
			mysym->type_p = 'D';
		else if (!ft_strcmp(mysect->name, SECT_BSS)) // Not sure in print cause never found ?
			mysym->type_p = 'B';
		else
			mysym->type_p = 'S';
	}
}

static void complete_mysym(t_file *file, t_symbol *mysym, t_arch arch) {
	// #define N_ABS 0x2		/* absolute, n_sect == NO_SECT */
	// #define N_SECT 0xe		/* defined in section number n_sect */
	// #define N_PBUD 0xc		/* prebound undefined (defined in a dylib) */
	// #define N_INDR 0xa
	if ((N_TYPE & mysym->type) == N_SECT) {
		match_sym_section(file->mysects, mysym);
	} else if ((N_TYPE & mysym->type) == N_UNDF) {
		mysym->type_p = 'U';
	}
	ft_printf("%0*llx %c %s\n", (arch == ARCH_32) ? 8 : 16, mysym->value, mysym->type_p, mysym->name);
	(void)arch;
}

int parse_symtab(t_file *file, t_symtab_command *symtab_command, t_arch arch) { // No need for 64 ?
	void *symtab;
	void *strtab;
	uint64_t nsyms;
	uint64_t i;
	t_symbol mysym;

	strtab = (void *) file->start + symtab_command->stroff;
	symtab = (void *) file->start + symtab_command->symoff;
	nsyms = symtab_command->nsyms;
	i = 0;
	while (i < nsyms) {
		if (arch == ARCH_32)
			init_mysym(&mysym, strtab + ((t_nlist *)symtab + i)->n_un.n_strx, (t_nlist *)symtab + i, arch);
		else
			init_mysym(&mysym, strtab + ((t_nlist_64 *)symtab + i)->n_un.n_strx, (t_nlist_64 *)symtab + i, arch);
		complete_mysym(file, &mysym, arch);
		i++;
	}

	return SUCCESS;
}
