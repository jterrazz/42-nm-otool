/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 12:20:55 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/15 15:18:29 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../shared.h"

// Move this ft
t_bool check_over(t_file *file, void *ptr)
{
	// Also check for before
	if ((ptr >= file->start + file->filesize) || (ptr >= file->end)) {
		file->error = E_OVERFLOW;
		return TRUE;
	}
	return FALSE;
}

int handle_archive(t_env *env, t_file *file)
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

		if (check_over(file, ptr) || ft_atoi(header->ar_size) == 0)
			return FAILURE;
		if (i) {
			name_size = !ft_strncmp(header->ar_name, AR_EFMT1, ft_strlen(AR_EFMT1)) ? ft_atoi(header->ar_name + ft_strlen(AR_EFMT1)) : 0;
			if (check_over(file, ptr + name_size))
				return FAILURE;
			init_file(&virtual_file, file->filename, ft_atoi(header->ar_size), ptr + name_size);
			init_virtual_file(&virtual_file, file, ptr); // Put in fat too ????
			virtual_file.is_virtual = TRUE;
			return handle_file(env, &virtual_file); // return in fat too ???
		}
		ptr += ft_atoi(header->ar_size);

		i++;
	}
		return SUCCESS;
	// Free file ?
}
