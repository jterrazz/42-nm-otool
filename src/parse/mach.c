/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:16:24 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 09:39:53 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
Otool will print directly when reading the segment
*/

static int parse_load_command(t_env *env, t_file *file, t_load_command *lc) { // return failure ????
	// + Do clearer way to separate otool/nm
	uint32_t cmd;

	cmd = swapif_u32(file, lc->cmd);

	if (cmd == LC_SEGMENT || cmd == LC_SEGMENT_64)
		return parse_mach_segment(env, file, lc);
	else if (env->bin == BIN_NM && cmd == LC_SYMTAB) // 64 / If no otool
		return parse_mach_symtab(file, (void *)lc);
	return SUCCESS;
}

int parse_mach(t_env *env, t_file *file)
{
	uint32_t ncmds;
	t_load_command *lc;

	lc = (t_load_command *)(file->start + ((file->arch == ARCH_32) ?
		sizeof(t_mach_header) : sizeof(t_mach_header_64)));
	if (check_overflow(file, lc))
		return (FAILURE);
	ncmds = (file->arch == ARCH_32) ? ((t_mach_header *)(file->start))->ncmds :
		((t_mach_header_64 *)(file->start))->ncmds;
	ncmds = swapif_u32(file, ncmds);

	while (ncmds--)
	{
		if (check_overflow(file, lc + sizeof(t_load_command)))
			return (FAILURE);
		if (parse_load_command(env, file, lc))
			return (FAILURE);
		lc = (void *)lc + swapif_u32(file, lc->cmdsize); // Secure for corruption ?
	}
	return SUCCESS;
}
