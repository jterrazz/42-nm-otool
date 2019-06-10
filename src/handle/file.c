/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 17:35:27 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

void handle_file(t_env *env, t_file *file, void *ptr)
{
	uint32_t magic;
	// uint32_t filetype;
	// FILE IS PROBABLY DEFINED HERE (BECAUSE FILE IS THE  EXECUTABLE AND NOT ARCHIVE)
	magic = *(uint32_t *)(ptr);
	// filetype = ((t_mach_header *)ptr)->filetype; // 64bits

	if (!ft_strncmp(ptr, ARMAG, SARMAG)) {
		ft_printf("yo\n");
	}

	// Handle ar
	// Handle fat
	// Handle not supported
	else if (magic == MH_MAGIC || magic == MH_CIGAM
		|| magic == MH_MAGIC_64 || magic == MH_CIGAM_64) { // Check cigam is working
		parse_mach_file(env, file, magic);

		if (env->bin == BIN_NM)
			print_mysyms(file);
	} else
		ft_printf("Not a valid file"); // Do a better msg ???
}
