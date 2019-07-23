/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 10:11:19 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 19:04:45 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** CPU Informations is defined at <libmacho/arch.c>
** http://web.mit.edu/darwin/src/modules/cctools/libmacho/arch.c
*/

// Put in array static var (remove all globals)
char *get_cpu_string(cpu_type_t cputype)
{
	if (cputype == CPU_TYPE_I386)
		return "i386";
	else if (cputype == CPU_TYPE_POWERPC)
		return "ppc";
	return "";
}

/*
** Handle nested files for the current cpu_type, or types if
** current is not found. It returns 0 if the current cpu_type is found,
** or 1 if not. In case of an error, returns -1.
*/

int process_arch(t_env *env, t_file *file, t_bool all_cputypes, t_fat_arch *fat_arch)
{
	t_file virtual_file;
	uint32_t offset;
	cpu_type_t cputype;

	if (check_overflow(file, fat_arch + sizeof(fat_arch)))
		return (-1);
	cputype = (file->swap_bits) ? ft_bswap_int32(fat_arch->cputype) : fat_arch->cputype;
	if (all_cputypes || env->cputype == cputype)
	{
		offset = (file->swap_bits) ? ft_bswap_uint32(fat_arch->offset) : fat_arch->offset;
		if (offset == 0)
			return (-1);
		init_file(&virtual_file, file->filename, (file->swap_bits)
			? ft_bswap_uint32(fat_arch->size) : fat_arch->size, file->start + offset);
		init_virtual_file(&virtual_file, file, (char *)file->filename);
		handle_binary(env, &virtual_file); // handle return ????????
		destroy_file(&virtual_file);
		if (!all_cputypes)
			return (0);
	}
	return (1);
}

/*
** A fat file starts with a t_fat_arch header. Each fat_arch element
** contains informations about the for this cputype. We first try priting
** the section of our computer cpu_type, else we print all of them.
*/

int handle_fat(t_env *env, t_file *file)
{
	t_fat_arch *fat_arch;
	uint32_t nfat_arch;
	uint32_t i;
	t_bool print_all_archs;

	i = 0;
	print_all_archs = FALSE;
	if (check_overflow(file, file->start + sizeof(t_fat_header)))
		return (FAILURE);
	nfat_arch = ((t_fat_header *)file->start)->nfat_arch;
	nfat_arch = swapif_u32(file, nfat_arch);
	fat_arch = file->start + sizeof(t_fat_header);
	while (i < nfat_arch)
	{
		if (!process_arch(env, file, print_all_archs, fat_arch))
			return (SUCCESS);
		fat_arch = (void *) fat_arch + sizeof(t_fat_arch);
		i++;
		if (i == nfat_arch && !print_all_archs)
		{
			i = 0;
			print_all_archs = TRUE;
		}
	}
	return (SUCCESS);
}
