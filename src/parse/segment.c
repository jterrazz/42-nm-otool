/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 23:01:29 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 02:14:19 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "../ft_nm.h"

void hexdump_segment_content(t_env *env, t_file *file, void *section, t_arch arch) { // 64
	uint64_t segstart;
	uint64_t size;
	uint32_t offset;
	char *name;

	if (arch == ARCH_32) {
		segstart = ((t_section *)section)->addr;
		size = ((t_section *)section)->size;
		offset = ((t_section *)section)->offset;
		name = ((t_section *)section)->sectname;
	} else {
		segstart = ((t_section_64 *)section)->addr;
		size = ((t_section_64 *)section)->size;
		offset = ((t_section_64 *)section)->offset;
		name = ((t_section_64 *)section)->sectname;
	}
	if (!ft_strcmp(SECT_TEXT, name)) {
		ft_printf("Contents of (__TEXT,__text) section\n");
		ft_hexdump(file->start + offset, size, segstart, arch);
	}
	(void)env;
}

// Test to make a malloc fails (by limiting memory ?)
int	parse_segment(t_env *env, t_file *file, void *segment_command, t_arch arch) {
	// Check seg ?
	size_t nsects;
	void *section; // t_section_64

	if (arch == ARCH_32) {
		nsects = ((t_segment_command *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command);
	} else {
		nsects = ((t_segment_command_64 *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command_64);
	}

	while (nsects--) {
		if (env->bin == BIN_OTOOL) {
			hexdump_segment_content(env, file, section, arch);
		} else {
			// ft_printf("%s: %s\n", section->sectname, section->segname);
			// Copy data to list here
		}
		section = (arch == ARCH_32)
			? section + sizeof(t_section)
			: section + sizeof(t_section_64);
	}

	return SUCCESS;
}
