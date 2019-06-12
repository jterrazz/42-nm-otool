/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_segment.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 23:01:29 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/12 20:28:31 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "../ft_nm.h"

static void hexdump_segment_content(t_file *file, void *sect)
{
	uint64_t segstart;
	uint64_t size;
	uint32_t offset;
	char *name;

	if (file->arch == ARCH_32) {
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
		ft_hexdump(file->start + offset, size, segstart, file->arch);
	}
}

static void file_add_mysection(t_file *file, void *sect)
{
	t_mysection mysect;

	if (file->arch == ARCH_32)
		mysect.name = ((t_section *)sect)->sectname;
	else
		mysect.name = ((t_section_64 *)sect)->sectname;
	mysect.index =  file->nsects;

	// malloc secure
	ft_lstadd(&file->mysects, ft_lstnew(&mysect, sizeof(t_mysection)));
}

/*
** Parse the segment, supports for :
** - hexdumping a segment content
** - setting the file->mysects
*/

int	parse_mach_segment(t_env *env, t_file *file, void *segment_command) {
	uint64_t nsects;
	void *section;

	if (file->arch == ARCH_32) {
		nsects = ((t_segment_command *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command);
	} else {
		nsects = ((t_segment_command_64 *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command_64);
	}

	while (nsects--) {
		file->nsects++;
		if (env->bin == BIN_OTOOL) {
			hexdump_segment_content(file, section);
		} else {
			file_add_mysection(file, section);
		}
		section = (file->arch == ARCH_32)
			? section + sizeof(t_section)
			: section + sizeof(t_section_64);
	}

	return SUCCESS;
}
