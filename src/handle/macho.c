/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 08:39:12 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 08:46:39 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

void handle_macho(t_env *env, t_file *file, uint32_t magic) // Stop in case of error with return
{
	file->arch = (magic == MH_MAGIC || magic == MH_CIGAM) ? ARCH_32 : ARCH_64;
	file->swap_bits = (magic == MH_MAGIC || magic == MH_MAGIC_64) ? FALSE : TRUE;
	// check with sizeof
	if (parse_macho(env, file) && file->error == E_OVERFLOW) // Make different error is no virtualname
		ft_printf("%s truncated or malformed archive (offset to next archive member past the end of the archive after member %s)\n", file->filename, file->virtualname);

	else if (env->bin == BIN_NM)
		print_mysyms(env, file);
}
