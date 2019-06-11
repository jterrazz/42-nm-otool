/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:16:24 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/11 12:32:59 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_nm.h"

static void parse_load_command(t_env *env, t_file *file, t_load_command *lc) {
	// + Do clearer way to separate otool/nm
	if (lc->cmd == LC_SEGMENT || lc->cmd == LC_SEGMENT_64)
		parse_segment(env, file, lc);
	else if (env->bin == BIN_NM && lc->cmd == LC_SYMTAB) // 64 / If no otool
		parse_symtab(file, (void *)lc);
}

void parse_mach_file(t_env *env, t_file *file, uint32_t magic)
{
	t_arch arch;
	arch = (magic == MH_MAGIC || magic == MH_CIGAM) ? ARCH_32 : ARCH_64;
	uint64_t ncmds;
	t_load_command *lc;

	file->arch = arch;
	ncmds = (arch == ARCH_32) ?
		((t_mach_header *)(file->start))->ncmds : // inverted
		((t_mach_header_64 *)(file->start))->ncmds; // inverted

	lc = (t_load_command *)(file->start +
		((arch == ARCH_32) ?
		sizeof(t_mach_header) : sizeof(t_mach_header_64))); // secure

	while (ncmds--) {
		parse_load_command(env, file, lc);
		lc = (void *)lc + lc->cmdsize; // Secure for corruption ?
	}
}
