/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 08:29:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 08:40:55 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

// Also try with archive in archive (mayb go overflow with the size)
// Check with files that don't start with this BSD thing AR_EFMT1
// https://en.wikipedia.org/wiki/Ar_(Unix)           BSD Variant !
// https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h

// make && ./ft_nm /bin/bash

static int handle_binary_error(t_file *file)
{
	if (file->virtualname)
		ft_printf("Mach-O universal file: %s for architecture x86_64 is not a Mach-O file or an archive file.\n", file->filename);
	else
		ft_printf("%s The file was not recognized as a valid object file\n", file->filename);
	return FAILURE;
}

// rename to binary
int handle_binary(t_env *env, t_file *file)
{
	uint32_t magic;
	// uint32_t filetype;
	if (check_overflow(file, file->start + sizeof(uint32_t))) // Check to create with 1 char size
		return handle_binary_error(file);
	magic = *(uint32_t *)(file->start);
	// filetype = ((t_mach_header *)ptr)->filetype; // 64bits

	if (!ft_strncmp(file->start, ARMAG, SARMAG)) { // or magic == *(uint32_t *)ARMAG
		return handle_archive(env, file);
	} else if (magic == FAT_MAGIC || magic == FAT_CIGAM) {
		if (handle_fat(env, file, magic)) {
			ft_printf("%s truncated or malformed fat file (cputype (0) cpusubtype (0) offset 0 overlaps universal headers)", file->filename); // Replace data
			return FAILURE;
		}
	}
	// Handle not supported
	else if (magic == MH_MAGIC || magic == MH_CIGAM
		|| magic == MH_MAGIC_64 || magic == MH_CIGAM_64) { // Check cigam is working
		handle_macho(env, file, magic);
	} else {
		return handle_binary_error(file);
	}
	return SUCCESS;
}
