/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 08:29:40 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 03:34:46 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** Dispatch the binary to the different handlers: Fat, Archive, Macho
*/

static int print_binary_error(t_file *file)
{
	if (file->error == E_OVERFLOW)
		ft_printf("%s truncated or malformed file\n", file->filename);
	else
		ft_printf("%s The file was not recognized as a valid object file\n", file->filename);
	return (FAILURE);
}

static int start_macho_parsing(t_env *env, t_file *file)
{
	if (env->bin == BIN_OTOOL)
		ft_printf("%s:\n", file->filename);
	if (parse_macho(env, file))
		return (FAILURE);
	if (env->bin == BIN_NM)
		print_mysyms(env, file);
	return (SUCCESS);
}

int start_binary(t_env *env, t_file *file)
{
	uint32_t magic;

	if (check_overflow(file, file->start + sizeof(uint32_t)))
		return (FAILURE);
	magic = *(uint32_t *)(file->start);
	if (!ft_strncmp(file->start, ARMAG, SARMAG))
		return (handle_archive(env, file));
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
	{
		file->swap_bits = magic == FAT_CIGAM ? TRUE : FALSE;
		return (handle_fat(env, file));
	}
	else if (magic == MH_MAGIC || magic == MH_CIGAM || magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		file->arch = (magic == MH_MAGIC || magic == MH_CIGAM) ? ARCH_32 : ARCH_64;
		file->swap_bits = (magic == MH_MAGIC || magic == MH_MAGIC_64) ? FALSE : TRUE;
		return (start_macho_parsing(env, file));
	}
	return (FAILURE);
}

int handle_binary(t_env *env, t_file *file)
{
	if (start_binary(env, file))
		return print_binary_error(file);
	return (SUCCESS);
}
