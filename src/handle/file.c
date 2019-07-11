/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 12:19:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/15 15:34:16 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../shared.h"

// Also try with archive in archive (mayb go overflow with the size)
// Check with files that don't start with this BSD thing AR_EFMT1
// https://en.wikipedia.org/wiki/Ar_(Unix)           BSD Variant !
// https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h

// make && ./ft_nm /bin/bash

void init_file(t_file *file, char const *name, uint64_t size, void *start)
{
	ft_bzero(file, sizeof(t_file));
	file->filename = name;
	file->filesize = size; // announced filesize
	file->start = start;
	file->error = E_NULL;
	file->swap_bits = FALSE;
	file->end = file->start + file->filesize; // end also checking for larger data
}

void init_virtual_file(t_file *file, t_file *old_file, char *virtualname)
{
	file->end = file->end < old_file->end ? file->end : old_file->end;
	file->virtualname = virtualname;
}

void handle_mach(t_env *env, t_file *file, uint32_t magic) // Stop in case of error with return
{
	file->arch = (magic == MH_MAGIC || magic == MH_CIGAM) ? ARCH_32 : ARCH_64;
	file->swap_bits = (magic == MH_MAGIC || magic == MH_MAGIC_64) ? FALSE : TRUE; // Delete is not used
	// check with sizeof
	if (parse_mach(env, file) && file->error == E_OVERFLOW) // Make different error is no virtualname
		ft_printf("%s truncated or malformed archive (offset to next archive member past the end of the archive after member %s)\n", file->filename, file->virtualname);

	else if (env->bin == BIN_NM)
		print_mysyms(file);
}

static int handle_file_error(t_file *file)
{
	if (file->virtualname)
		ft_printf("Mach-O universal file: %s for architecture x86_64 is not a Mach-O file or an archive file.\n", file->filename);
	else
		ft_printf("%s The file was not recognized as a valid object file\n", file->filename);
	return FAILURE;
}

int handle_file(t_env *env, t_file *file)
{
	uint32_t magic;
	// uint32_t filetype;
	if (check_over(file, file->start + sizeof(uint32_t)))
		return handle_file_error(file);
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
		handle_mach(env, file, magic);
	} else {
		return handle_file_error(file);
	}
	return SUCCESS;
}
