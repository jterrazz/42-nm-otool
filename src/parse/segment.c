/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 23:01:29 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 13:07:08 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "../ft_nm.h"

static void hexdump_segment_content(t_file *file, void *sect, t_arch arch) { // 64
	uint64_t segstart;
	uint64_t size;
	uint32_t offset;
	char *name;

	if (arch == ARCH_32) {
		segstart = ((t_section *)sect)->addr;
		size = ((t_section *)sect)->size;
		offset = ((t_section *)sect)->offset;
		name = ((t_section *)sect)->sectname;
	} else {
		segstart = ((t_section_64 *)sect)->addr;
		size = ((t_section_64 *)sect)->size;
		offset = ((t_section_64 *)sect)->offset;
		name = ((t_section_64 *)sect)->sectname;
	}
	if (!ft_strcmp(SECT_TEXT, name)) {
		ft_printf("Contents of (__TEXT,__text) section\n");
		ft_hexdump(file->start + offset, size, segstart, arch);
	}
}

// malloc secure
static void file_add_mysection(t_file *file, void *sect, t_arch arch) {
	t_mysection mysect;

	if (arch == ARCH_32)
		mysect.name = ((t_section *)sect)->sectname;
	else
		mysect.name = ((t_section_64 *)sect)->sectname;
	mysect.index =  file->nsects;

	ft_lstadd(&file->mysects, ft_lstnew(&mysect, sizeof(t_mysection)));
}

int	parse_segment(t_env *env, t_file *file, void *segment_command, t_arch arch) {
	// Check seg ?
	uint64_t nsects;
	void *section;

	if (arch == ARCH_32) {
		nsects = ((t_segment_command *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command);
	} else {
		nsects = ((t_segment_command_64 *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command_64);
	}

	while (nsects--) {
		file->nsects++;
		if (env->bin == BIN_OTOOL) {
			hexdump_segment_content(file, section, arch);
		} else {
			file_add_mysection(file, section, arch);
		}
		section = (arch == ARCH_32)
			? section + sizeof(t_section)
			: section + sizeof(t_section_64);
	}

	return SUCCESS;
}
