/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:16:24 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/11 12:12:29 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_nm.h"

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

	// TODO Put this in dedicated function (load_command)
	// + Do clearer way to separate otool/nm
	while (ncmds--) {
		if (lc->cmd == LC_SEGMENT || lc->cmd == LC_SEGMENT_64)
			parse_segment(env, file, lc, arch);
		else if (env->bin == BIN_NM && lc->cmd == LC_SYMTAB) // 64 / If no otool
			parse_symtab(file, (void *)lc, arch);
		lc = (void *)lc + lc->cmdsize;
	}
}
