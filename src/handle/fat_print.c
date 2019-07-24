/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/24 17:51:18 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 17:52:16 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int			print_fat_header(t_file *file, t_fat_header *fat_header,
	unsigned long nfat_arch, t_fat_arch *fat_arch)
{
	unsigned long	i;
	size_t			power;

	i = 0;
	ft_printf("Fat headers\nfat_magic 0x%08x\n", fat_header->magic);
	ft_printf("nfat_arch %d\n", swapif_u32(file, fat_header->nfat_arch));
	while (i < nfat_arch)
	{
		if (check_overflow(file, fat_arch + 1))
			return (-1);
		ft_printf("architecture\n", i);
		ft_printf("    cputype %d\n", swapif_u32(file, fat_arch->cputype));
		ft_printf("    cpusubtype %d\n", swapif_u32(file,
			fat_arch->cpusubtype) & ~CPU_SUBTYPE_MASK);
		ft_printf("    capabilities 0x%02x\n", (swapif_u32(file,
			(fat_arch->cpusubtype)) & CPU_SUBTYPE_MASK) >> 24);
		ft_printf("    offset %d\n", swapif_u32(file, fat_arch->offset));
		ft_printf("    size %d\n", swapif_u32(file, fat_arch->size));
		power = swapif_u32(file, fat_arch->align);
		ft_printf("    align 2^%d (%lld)\n", power, ft_pow(2, power));
		fat_arch = (void *)fat_arch + sizeof(t_fat_arch);
		i++;
	}
	return (SUCCESS);
}
