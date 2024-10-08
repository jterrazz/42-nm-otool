/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 10:11:19 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 17:52:54 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** CPU Informations is defined at <.../arch.c>
** https://github.com/opensource-apple/cctools/blob/master/libstuff/arch.c
*/

static t_arch_flag	g_arch_flags[] = {
	{ "any", CPU_TYPE_ANY, CPU_SUBTYPE_MULTIPLE },
	{ "little", CPU_TYPE_ANY, CPU_SUBTYPE_LITTLE_ENDIAN },
	{ "big", CPU_TYPE_ANY, CPU_SUBTYPE_BIG_ENDIAN },
	{ "ppc601", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_601 },
	{ "ppc603", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603 },
	{ "ppc603e", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603e },
	{ "ppc603ev", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_603ev },
	{ "ppc604", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_604 },
	{ "ppc604e", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_604e },
	{ "ppc750", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_750 },
	{ "ppc7400", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_7400 },
	{ "ppc7450", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_7450 },
	{ "ppc970", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_970 },
	{ "i486", CPU_TYPE_I386, CPU_SUBTYPE_486 },
	{ "i486SX", CPU_TYPE_I386, CPU_SUBTYPE_486SX },
	{ "pentium", CPU_TYPE_I386, CPU_SUBTYPE_PENT },
	{ "i586", CPU_TYPE_I386, CPU_SUBTYPE_586 },
	{ "pentpro", CPU_TYPE_I386, CPU_SUBTYPE_PENTPRO },
	{ "i686", CPU_TYPE_I386, CPU_SUBTYPE_PENTPRO },
	{ "pentIIm3", CPU_TYPE_I386, CPU_SUBTYPE_PENTII_M3 },
	{ "pentIIm5", CPU_TYPE_I386, CPU_SUBTYPE_PENTII_M5 },
	{ "pentium4", CPU_TYPE_I386, CPU_SUBTYPE_PENTIUM_4 },
	{ "m68030", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC68030_ONLY },
	{ "m68040", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC68040 },
	{ "hppa7100LC", CPU_TYPE_HPPA, CPU_SUBTYPE_HPPA_7100LC },
	{ "armv4t", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V4T},
	{ "armv5", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V5TEJ},
	{ "xscale", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_XSCALE},
	{ "armv6", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V6 },
	{ "armv6m", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V6M },
	{ "armv7", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7 },
	{ "armv7f", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7F },
	{ "armv7s", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S },
	{ "armv7k", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K },
	{ "armv7m", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7M },
	{ "armv7em", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7EM },
	{ "arm64v8", CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_V8 },
	{ "ppc64", CPU_TYPE_POWERPC64, CPU_SUBTYPE_POWERPC_ALL },
	{ "x86_64", CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL },
	{ "x86_64h", CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_H },
	{ "arm64", CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL },
	{ "ppc970-64", CPU_TYPE_POWERPC64, CPU_SUBTYPE_POWERPC_970 },
	{ "ppc", CPU_TYPE_POWERPC, CPU_SUBTYPE_POWERPC_ALL },
	{ "i386", CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL },
	{ "m68k", CPU_TYPE_MC680x0, CPU_SUBTYPE_MC680x0_ALL },
	{ "hppa", CPU_TYPE_HPPA, CPU_SUBTYPE_HPPA_ALL },
	{ "sparc", CPU_TYPE_SPARC, CPU_SUBTYPE_SPARC_ALL },
	{ "m88k", CPU_TYPE_MC88000, CPU_SUBTYPE_MC88000_ALL },
	{ "i860", CPU_TYPE_I860, CPU_SUBTYPE_I860_ALL },
	{ "arm", CPU_TYPE_ARM, CPU_SUBTYPE_ARM_ALL },
	{ NULL, 0, 0 }
};

char				*get_cpu_string(cpu_type_t cputype,
	cpu_subtype_t cpusubtype)
{
	int i;

	i = 0;
	while (g_arch_flags[i].name)
	{
		if (cputype == g_arch_flags[i].cputype
			&& cpusubtype == g_arch_flags[i].cpusubtype)
			return (g_arch_flags[i].name);
		i++;
	}
	return (NULL);
}

int					process_arch_init(t_file *file, t_fat_arch *fat_arch,
	uint32_t *offset, cpu_type_t *cputype)
{
	if (check_overflow(file, fat_arch + 1))
		return (FAILURE);
	*cputype = (file->swap_bits) ?
		ft_bswap_int32(fat_arch->cputype) : fat_arch->cputype;
	*offset = (file->swap_bits) ?
		ft_bswap_uint32(fat_arch->offset) : fat_arch->offset;
	if (offset == 0)
		return (FAILURE);
	return (SUCCESS);
}

/*
** Handle nested files for the current cpu_type, or types if
** current is not found. It returns 1 if was printed as expected,
** or 0 if not. In case of an error, returns -1.
*/

int					process_arch(t_env *env, t_file *file,
	t_bool all_cputypes, t_fat_arch *fat_arch)
{
	t_file		virtual_file;
	uint32_t	offset;
	cpu_type_t	cputype;
	cpu_type_t	cpusubtype;
	int			ret;

	if (process_arch_init(file, fat_arch, &offset, &cputype))
		return (-1);
	cpusubtype = (file->swap_bits) ?
		ft_bswap_int32(fat_arch->cpusubtype) : fat_arch->cpusubtype;
	if (all_cputypes || env->cputype == cputype)
	{
		create_file(&virtual_file, file->filename, (file->swap_bits)
	? ft_bswap_uint32(fat_arch->size) : fat_arch->size, file->start + offset);
		if (create_virtual_file(&virtual_file, file, file->filename, FILE_FAT))
			return (-1);
		if (all_cputypes && env->bin == BIN_NM)
			ft_printf("\n%s (for architecture %s):\n",
				file->filename, get_cpu_string(cputype, cpusubtype));
		ret = handle_binary(env, &virtual_file);
		destroy_file(&virtual_file);
		if (!all_cputypes && ret == 0)
			return (1);
	}
	return (!all_cputypes ? 0 : ret);
}

/*
** A fat file starts with a t_fat_arch header. Each fat_arch element
** contains informations about the for this cputype. We first try priting
** the section of our computer cpu_type, else we print all of them.
*/

int					handle_fat_section(t_env *env, t_file *file,
	t_bool print_all_archs, t_fat_arch *fat_arch)
{
	unsigned long	i;
	int				ret;
	unsigned long	nfat_arch;
	t_bool			printed_error;

	i = 0;
	ret = 0;
	printed_error = 0;
	nfat_arch = ((t_fat_header *)file->start)->nfat_arch;
	nfat_arch = swapif_u32(file, nfat_arch);
	while (i < nfat_arch)
	{
		ret = process_arch(env, file, print_all_archs, fat_arch);
		if (!print_all_archs && ret != 0)
			return (ret);
		printed_error |= ret < 0;
		fat_arch = (void *)fat_arch + sizeof(t_fat_arch);
		i++;
	}
	return (printed_error ? 1 : ret);
}

int					handle_fat(t_env *env, t_file *file)
{
	unsigned long	nfat_arch;
	t_fat_arch		*fat_arch;
	int				ret;

	ret = 0;
	nfat_arch = ((t_fat_header *)file->start)->nfat_arch;
	nfat_arch = swapif_u32(file, nfat_arch);
	if (check_overflow(file, file->start + sizeof(t_fat_header)))
		return (FAILURE);
	fat_arch = file->start + sizeof(t_fat_header);
	if (env->flags & FLAG_F && env->bin == BIN_OTOOL)
		return (print_fat_header(file, file->start, nfat_arch, fat_arch));
	ret = handle_fat_section(env, file, FALSE, fat_arch);
	if (ret == 0)
		ret = handle_fat_section(env, file, TRUE, fat_arch);
	return (ret < 0 ? FAILURE : SUCCESS);
}
