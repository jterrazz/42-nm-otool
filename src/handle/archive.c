/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 12:20:55 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 00:36:06 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int handle_archive(t_env *env, t_file *file)
{
	t_ar_hdr *ar_header;
	uint64_t ar_size;
	uint64_t name_size;
	uint64_t i;
	t_file virtual_file;
	void *ptr;

	i = 0;
	ptr = file->start + SARMAG;

	while (ptr < file->start + file->filesize)
	{
		ar_header = ptr;
		ptr += sizeof(t_ar_hdr);
		ar_size = ft_atoi(ar_header->ar_size);

		if (check_overflow(file, ptr) || ar_size == 0 || check_overflow(file, ptr + ar_size))
			return FAILURE;
		if (i) {
			name_size = !ft_strncmp(ar_header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) ? ft_atoi(ar_header->ar_name + ft_strlen(AR_EFMT1)) : 0;
			if (check_overflow(file, ptr + name_size))
				return FAILURE;
			create_file(&virtual_file, file->filename, ar_size, ptr + name_size);
			if (create_virtual_file(&virtual_file, file, ptr))
				return FAILURE;
			handle_binary(env, &virtual_file);
			destroy_file(&virtual_file);
		}
		ptr += ar_size;
		i++;
	}
	return (SUCCESS);
}
