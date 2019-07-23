/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 08:29:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 18:25:39 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

// Also try with archive in archive (mayb go overflow with the size)
// Check with files that don't start with this BSD thing AR_EFMT1
// https://en.wikipedia.org/wiki/Ar_(Unix)           BSD Variant !
// https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h

// make && ./ft_nm /bin/bash

static int print_binary_error(t_file *file)
{
	if (file->error == E_OVERFLOW)
		ft_printf("%s truncated or malformed fat file (cputype (0) cpusubtype (0) offset 0 overlaps universal headers)", file->filename); // Replace data
	else if (file->virtualname)
		ft_printf("Mach-O universal file: %s for architecture x86_64 is not a Mach-O file or an archive file.\n", file->filename);
	else
		ft_printf("%s The file was not recognized as a valid object file\n", file->filename);
	return (FAILURE);
}

static int start_macho_parsing(t_env *env, t_file *file, uint32_t magic)
{
	file->arch = (magic == MH_MAGIC || magic == MH_CIGAM) ? ARCH_32 : ARCH_64;
	file->swap_bits = (magic == MH_MAGIC || magic == MH_MAGIC_64) ? FALSE : TRUE;

	if (parse_macho(env, file))
		return (FAILURE);
	if (env->bin == BIN_NM)
		print_mysyms(env, file);
	return (SUCCESS);
}

int handle_binary(t_env *env, t_file *file)
{
	uint32_t magic;

	if (check_overflow(file, file->start + sizeof(uint32_t)))
		return print_binary_error(file);
	magic = *(uint32_t *)(file->start);
	if (!ft_strncmp(file->start, ARMAG, SARMAG))
		return (handle_archive(env, file));
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
	{
		file->swap_bits = magic == FAT_CIGAM ? TRUE : FALSE;
		if (handle_fat(env, file))
			return print_binary_error(file);
	}
	else if (magic == MH_MAGIC || magic == MH_CIGAM || magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		return (start_macho_parsing(env, file, magic));
	return print_binary_error(file);
}
