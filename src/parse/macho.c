/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:16:24 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 10:27:25 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	print_macho_header(t_file *file, void *header_ptr)
{
	t_mach_header_64	*header_64;
	t_mach_header		*header_32;

	ft_printf("Mach header\n");
	ft_printf("      magic cputype cpusubtype  \
caps    filetype ncmds sizeofcmds      flags\n");
	if (file->arch == ARCH_64)
	{
		header_64 = header_ptr;
		ft_printf(" 0x%08x %8ld %10ld  0x%02x %11d %5d %10d 0x%08x\n",
			header_64->magic, header_64->cputype, header_64->cpusubtype &
			~CPU_SUBTYPE_MASK, (header_64->cpusubtype & CPU_SUBTYPE_MASK)
			>> 24, header_64->filetype, header_64->ncmds,
			header_64->sizeofcmds, header_64->flags);
	}
	else
	{
		header_32 = header_ptr;
		ft_printf(" 0x%08x %8ld %10ld  0x%02x %11d %5d %10d 0x%08x\n",
			header_32->magic, header_32->cputype, header_32->cpusubtype &
			~CPU_SUBTYPE_MASK, (header_32->cpusubtype & CPU_SUBTYPE_MASK)
			>> 24, header_32->filetype, header_32->ncmds,
			header_32->sizeofcmds, header_32->flags);
	}
	return (SUCCESS);
}

static int	parse_load_command_errors(t_env *env, t_file *file,
	t_load_command *lc, uint32_t i)
{
	uint64_t	cmd_size;
	void		*endofcmds;

	cmd_size = lc->cmdsize;
	endofcmds = (t_load_command *)(file->start + ((file->arch == ARCH_32) ?
		sizeof(t_mach_header) : sizeof(t_mach_header_64))) + swapif_u32(file,
		(file->arch == ARCH_32) ? ((t_mach_header *)(file->start))->sizeofcmds
		: ((t_mach_header_64 *)(file->start))->sizeofcmds);
	if (i == 0 && cmd_size % 8)
	{
		ft_printf("%s: %s truncated or malformed object (load command %d \
cmdsize not a multiple of 8)\n", env->execname, file->filename, i);
		file->error = E_WAS_PRINTED;
		return (FAILURE);
	}
	if ((void *)lc + (cmd_size || 1) > endofcmds)
	{
		ft_printf("%s: %s truncated or malformed object (load command %d exten\
ds past the end all load commands in the file)\n", env->execname,
file->filename, i);
		file->error = E_WAS_PRINTED;
		return (FAILURE);
	}
	return (SUCCESS);
}

/*
** Otool will print directly when reading the segment
*/

static int	parse_load_command(t_env *env, t_file *file,
	t_load_command *lc, uint32_t i)
{
	uint32_t	cmd;

	cmd = swapif_u32(file, lc->cmd);
	if (parse_load_command_errors(env, file, lc, i))
		return (FAILURE);
	if (cmd == LC_SEGMENT || cmd == LC_SEGMENT_64)
		return (parse_macho_segment(env, file, lc));
	else if (env->bin == BIN_NM && cmd == LC_SYMTAB)
		return (parse_macho_symtab(file, (void *)lc));
	return (SUCCESS);
}

int			parse_macho(t_env *env, t_file *file)
{
	t_load_command	*lc;
	uint32_t		ncmds;
	uint32_t		i;

	lc = (t_load_command *)(file->start + ((file->arch == ARCH_32) ?
		sizeof(t_mach_header) : sizeof(t_mach_header_64)));
	if (check_overflow(file, lc))
		return (FAILURE);
	if (env->flags & FLAG_M && env->bin == BIN_OTOOL)
		return (print_macho_header(file, file->start));
	ncmds = swapif_u32(file, (file->arch == ARCH_32)
		? ((t_mach_header *)(file->start))->ncmds :
		((t_mach_header_64 *)(file->start))->ncmds);
	i = 0;
	while (i < ncmds)
	{
		if (check_overflow(file, lc + 1)
			|| parse_load_command(env, file, lc, i++))
			return (FAILURE);
		lc = (void *)lc + swapif_u32(file, lc->cmdsize);
	}
	return (SUCCESS);
}
