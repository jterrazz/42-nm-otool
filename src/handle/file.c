/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/11 12:27:22 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

// Also try with archive in archive (mayb go overflow with the size)
// Check with files that don't start with this BSD thing AR_EFMT1
// https://en.wikipedia.org/wiki/Ar_(Unix)           BSD Variant !

void init_file(t_file *file, char const *name, uint64_t size, void *start)
{
	ft_bzero(file, sizeof(t_file));
	file->filename = name;
	file->filesize = size;
	file->start = start;
}

void handle_file(t_env *env, t_file *file)
{
	uint32_t magic;
	// uint32_t filetype;
	// FILE IS PROBABLY DEFINED HERE (BECAUSE FILE IS THE  EXECUTABLE AND NOT ARCHIVE)
	magic = *(uint32_t *)(file->start);
	// filetype = ((t_mach_header *)ptr)->filetype; // 64bits

	if (!ft_strncmp(file->start, ARMAG, SARMAG)) { // or magic == *(uint32_t *)ARMAG
		handle_archive(env, file);
	}

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
