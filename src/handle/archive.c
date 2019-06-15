/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 12:20:55 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/15 14:31:56 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

// Move this ft
t_bool check_over(t_file *file, void *ptr)
{
	if ((ptr >= file->start + file->filesize) || (ptr >= file->end)) {
		file->error = E_OVERFLOW;
		return TRUE;
	}
	return FALSE;
}

void handle_archive(t_env *env, t_file *file)
{
	t_ar_hdr *header;
	uint64_t name_size;
	uint64_t i;
	t_file virtual_file;
	void *ptr;

	ptr = file->start + SARMAG;

	i = 0;
	while (ptr < file->start + file->filesize) {
		header = ptr;
		ptr += sizeof(t_ar_hdr);

		if (check_over(file, ptr))
			return;

		if (i) {
			name_size = !ft_strncmp(header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) ? ft_atoi(header->ar_name + ft_strlen(AR_EFMT1)) : 0;
			init_file(&virtual_file, file->filename, ft_atoi(header->ar_size), ptr + name_size);
			init_virtual_file(&virtual_file, file, ptr); // Put in fat too ????
			virtual_file.is_virtual = TRUE;
			if (handle_file(env, &virtual_file)) // return in fat too ???
				return;
		}
		ptr += ft_atoi(header->ar_size);

		i++;
	}

	// Free file ?
}
