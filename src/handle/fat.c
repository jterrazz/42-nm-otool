/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 10:11:19 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 18:22:39 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"
#include <mach-o/arch.h> // delete if not using official arr.

// Find global list ? http://web.mit.edu/darwin/src/modules/cctools/libmacho/arch.c
// Put in static var (remove all globals)
char *get_cpu_string(cpu_type_t cputype)
{
	if (cputype == CPU_TYPE_I386)
		return "i386";
	else if (cputype == CPU_TYPE_POWERPC)
		return "ppc";
	return "";
}

// TODO Use common function
static int fallback_handle_all_archs(t_env *env, t_file *file)
{
	t_fat_arch *fat_arch;
	cpu_type_t cputype;
	uint32_t offset;
	uint32_t nfat_arch;
	t_file virtual_file;

	nfat_arch = ((t_fat_header *)file->start)->nfat_arch;
	nfat_arch = swapif_u32(file, nfat_arch);
	fat_arch = file->start + sizeof(t_fat_header);
	while (nfat_arch-- > 0) {
		if (check_overflow(file, fat_arch + sizeof(fat_arch)))
			return FAILURE;
		cputype = (file->swap_bits) ? ft_bswap_int32(fat_arch->cputype) : fat_arch->cputype;
		ft_printf("\n%s (for architecture %s):\n", file->filename, get_cpu_string(cputype));
		offset = (file->swap_bits)
			? ft_bswap_uint32(fat_arch->offset)
			: fat_arch->offset;
		if (offset == 0)
			return FAILURE;
		init_file(&virtual_file, file->filename, (file->swap_bits)
			? ft_bswap_uint32(fat_arch->size)
			: fat_arch->size, file->start + offset);
		handle_binary(env, &virtual_file);
		fat_arch = (void *) fat_arch + sizeof(t_fat_arch);
	}
	return SUCCESS;
}

// Check the checkft all have sizeof()

int handle_fat(t_env *env, t_file *file, uint32_t magic) // Secure the ft up the same
{
	uint32_t offset;
	uint32_t nfat_arch;
	t_fat_arch *fat_arch;
	t_file virtual_file;
	cpu_type_t cputype;

	if (check_overflow(file, file->start + sizeof(t_fat_header)))
		return (FAILURE);
	nfat_arch = ((t_fat_header *)file->start)->nfat_arch;
	nfat_arch = swapif_u32(file, nfat_arch);
	fat_arch = file->start + sizeof(t_fat_header);
	while (nfat_arch-- > 0) {
		if (check_overflow(file, fat_arch + sizeof(fat_arch)))
			return (FAILURE);
		cputype = (file->swap_bits) ? ft_bswap_int32(fat_arch->cputype) : fat_arch->cputype;
		if (env->cputype == cputype) {
			offset = (file->swap_bits)
				? ft_bswap_uint32(fat_arch->offset)
				: fat_arch->offset;
			if (offset == 0)
				return (FAILURE);
			init_file(&virtual_file, file->filename, (file->swap_bits)
				? ft_bswap_uint32(fat_arch->size)
				: fat_arch->size, file->start + offset);
			// handle_binary(env, &virtual_file);
			return SUCCESS;
		}
		fat_arch = (void *) fat_arch + sizeof(t_fat_arch);
	}
	return fallback_handle_all_archs(env, file); // check when is it used ?
	return SUCCESS;
}
