/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/09 23:32:35 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

static void build_symbol(char *symname, void *symtab) {
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
			build_symbol(strtab + ((t_nlist *)symtab + i)->n_un.n_strx, (t_nlist *)symtab + i);
		else
			build_symbol(strtab + ((t_nlist_64 *)symtab + i)->n_un.n_strx, (t_nlist_64 *)symtab + i);
		i++;
	}

	return SUCCESS;
}

static void parse_load_cmds(t_env *env, t_file *file, t_arch arch) {
	size_t ncmds;
	t_load_command *lc;
	(void)env;

	file->arch = arch;
	ncmds = (arch == ARCH_32) ?
		((t_mach_header *)(file->start))->ncmds : // inverted
		((t_mach_header_64 *)(file->start))->ncmds; // inverted

	lc = (t_load_command *)(file->start +
		((arch == ARCH_32) ?
		sizeof(t_mach_header) : sizeof(t_mach_header_64))); // secure
	// Section number (not needed i guess)

	while (ncmds--) {
		if (lc->cmd == LC_SEGMENT || lc->cmd == LC_SEGMENT_64)
			parse_segment(lc, arch);
		else if (lc->cmd == LC_SYMTAB)
			parse_symtab(file, (void *)lc, arch);
		lc = (void *)lc + lc->cmdsize;
	}
}

void parse_file(t_env *env, t_file *file) {
	uint32_t magic;
	uint32_t filetype;

	magic = *(uint32_t *)(file->start);
	filetype = ((t_mach_header *)file->start)->filetype; // 64bits

	// Handle ar
	// Handle fat
	// Handle not supported
	if (magic == MH_MAGIC || magic == MH_CIGAM) // Check cigam is working
		parse_load_cmds(env, file, ARCH_32);// Handle otool - nm repartition here ?
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		parse_load_cmds(env, file, ARCH_64);
	else
		ft_printf("Not a valid file"); // Do a better msg ???
	// Handle 64
}
