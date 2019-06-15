/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:16:24 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/15 14:06:15 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

static void parse_load_command(t_env *env, t_file *file, t_load_command *lc) {
	// + Do clearer way to separate otool/nm
	uint32_t cmd;

	cmd = swapif_u32(file, lc->cmd);

	if (cmd == LC_SEGMENT || cmd == LC_SEGMENT_64)
		parse_mach_segment(env, file, lc);
	else if (env->bin == BIN_NM && cmd == LC_SYMTAB) // 64 / If no otool
		parse_mach_symtab(file, (void *)lc);
}

int parse_mach(t_env *env, t_file *file)
{
	uint32_t ncmds;
	t_load_command *lc;

	ncmds = (file->arch == ARCH_32) ?
		((t_mach_header *)(file->start))->ncmds : // inverted
		((t_mach_header_64 *)(file->start))->ncmds; // inverted
	ncmds = swapif_u32(file, ncmds);

	lc = (t_load_command *)(file->start +
		((file->arch == ARCH_32) ?
		sizeof(t_mach_header) : sizeof(t_mach_header_64))); // secure

	// ft_printf("There are %lld load commands\n", ncmds);
	while (ncmds--) {
		if (check_over(file, lc))
			return FAILURE;
		// ft_printf("Command nb %lld start\n", ncmds);
		parse_load_command(env, file, lc);
		lc = (void *)lc + swapif_u32(file, lc->cmdsize); // Secure for corruption ?
		// ft_printf("Command nb %lld end\n", ncmds);
	}
	// ft_printf("Ending parsing mach files\n");
	return SUCCESS;
}
