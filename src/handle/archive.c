/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 12:20:55 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 16:52:18 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int parse_archive_section(t_env *env, t_file *file, t_ar_hdr *ar_header, uint64_t ar_size)
{
	uint64_t name_size;
	t_file virtual_file;
	void *ptr;

	ptr = (void *)ar_header + sizeof(t_ar_hdr);
	name_size = !ft_strncmp(ar_header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) ? ft_atoi(ar_header->ar_name + ft_strlen(AR_EFMT1)) : 0;
	if (check_overflow(file, ptr + name_size))
		return FAILURE;
	create_file(&virtual_file, file->filename, ar_size - sizeof(ar_header), ptr + name_size);
	create_virtual_file(&virtual_file, file, ptr);
	if (env->bin == BIN_NM)
		ft_printf("\n");
	ft_printf("%s(%s):\n", virtual_file.filename, virtual_file.virtualname);
	handle_binary(env, &virtual_file); // Handle
	destroy_file(&virtual_file);
	return SUCCESS;
}

int handle_archive(t_env *env, t_file *file)
{
	t_ar_hdr *ar_header;
	uint64_t ar_size;
	uint64_t i;
	void *ptr;

	i = 0;
	ptr = file->start + SARMAG;

	if (env->bin == BIN_OTOOL)
		ft_printf("Archive: %s\n", file->filename);

	while (ptr < file->start + file->filesize)
	{
		ar_header = ptr;
		ptr += sizeof(t_ar_hdr);
		ar_size = ft_atoi(ar_header->ar_size);
		if (check_overflow(file, ptr) || ar_size == 0 || check_overflow(file, ptr + ar_size))
			return (FAILURE);
		if (i) {
			if (parse_archive_section(env, file, ar_header, ar_size))
				return (FAILURE);
		}
		ptr += ar_size;
		i++;
	}
	return (SUCCESS);
}
