/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 12:20:55 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/15 01:01:26 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../ft_nm.h"

// Move this ft
t_bool check_over(t_file *file, void *ptr)
{
	return ptr >= file->start + file->filesize;
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
			ft_printf("\n%s(%.*s):\n", file->filename, name_size, ptr);
			init_file(&virtual_file, ptr, ft_atoi(header->ar_size), ptr + name_size);
			virtual_file.is_virtual = TRUE;
			handle_file(env, &virtual_file);
		}
		ptr += ft_atoi(header->ar_size);

		i++;
	}

	// Free file ?
}
