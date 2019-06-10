/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 11:28:27 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

void parse_load_commands(t_env *env, t_file *file, t_arch arch) { // dedicated file
	uint64_t ncmds;
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
			parse_segment(env, file, lc, arch);
		else if (env->bin == BIN_NM && lc->cmd == LC_SYMTAB) // 64 / If no otool
			parse_symtab(file, (void *)lc, arch);
		lc = (void *)lc + lc->cmdsize;
	}
}

void handle_file(t_env *env, t_file *file) {
	uint32_t magic;
	uint32_t filetype;
	// FILE IS PROBABLY DEFINED HERE (BECAUSE FILE IS THE  EXECUTABLE AND NOT ARCHIVE)
	magic = *(uint32_t *)(file->start);
	filetype = ((t_mach_header *)(file->start))->filetype; // 64bits

	// Handle ar
	// Handle fat
	// Handle not supported
	if (magic == MH_MAGIC || magic == MH_CIGAM) // Check cigam is working
		parse_load_commands(env, file, ARCH_32);// Handle otool - nm repartition here ?
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		parse_load_commands(env, file, ARCH_64);
	else
		ft_printf("Not a valid file"); // Do a better msg ???
	// Handle 64
}
