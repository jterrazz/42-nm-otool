/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_binary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 10:11:19 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/13 14:24:47 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_nm.h"

void handle_fat(t_env *env, t_file *file, uint32_t magic)
{
	uint32_t offset;
	uint32_t nfat_arch;
	t_fat_arch *fat_arch;
	t_file virtual_file;
	cpu_type_t cputype;

	file->swap_bits = magic == FAT_CIGAM ? TRUE : FALSE;
	nfat_arch = ((t_fat_header *)file->start)->nfat_arch;
	nfat_arch = (file->swap_bits) ? ft_bswap_uint32(nfat_arch) : nfat_arch;
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
}
