/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 10:11:19 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/14 11:34:15 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

static char *get_cpu_string(cpu_type_t cputype)
{
	if (cputype == CPU_TYPE_I386)
		return "i386";
	return "";
}
// TODO Use common function
static void fallback_handle_all_archs(t_env *env, t_file *file)
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
		cputype = (file->swap_bits) ? ft_bswap_int32(fat_arch->cputype) : fat_arch->cputype;
		ft_printf("\n%s (for architecture %s):\n", file->filename, get_cpu_string(cputype));
		offset = (file->swap_bits)
			? ft_bswap_uint32(fat_arch->offset)
			: fat_arch->offset;
		init_file(&virtual_file, file->filename, (file->swap_bits)
			? ft_bswap_uint32(fat_arch->size)
			: fat_arch->size, file->start + offset);
		handle_file(env, &virtual_file);
		fat_arch = (void *) fat_arch + sizeof(t_fat_arch);
	}
}

void handle_fat(t_env *env, t_file *file, uint32_t magic)
{
	uint32_t offset;
	uint32_t nfat_arch;
	t_fat_arch *fat_arch;
	t_file virtual_file;
	cpu_type_t cputype;

	file->swap_bits = magic == FAT_CIGAM ? TRUE : FALSE;
	nfat_arch = ((t_fat_header *)file->start)->nfat_arch;
	nfat_arch = swapif_u32(file, nfat_arch);
	fat_arch = file->start + sizeof(t_fat_header);

	while (nfat_arch-- > 0) {
		// Check for offset forbidden values ?
		cputype = (file->swap_bits) ? ft_bswap_int32(fat_arch->cputype) : fat_arch->cputype;
		if (env->cputype == cputype) {
			offset = (file->swap_bits)
				? ft_bswap_uint32(fat_arch->offset)
				: fat_arch->offset;
			init_file(&virtual_file, file->filename, (file->swap_bits)
				? ft_bswap_uint32(fat_arch->size)
				: fat_arch->size, file->start + offset);
			return handle_file(env, &virtual_file);
		}
		fat_arch = (void *) fat_arch + sizeof(t_fat_arch);
	}
	fallback_handle_all_archs(env, file);
}
