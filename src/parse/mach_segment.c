/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_segment.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 23:01:29 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 09:24:08 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "nm_otool.h"

// Probably secure this too
static void hexdump_segment_content(t_file *file, void *sect) // Section not segment
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
	if (!ft_strcmp(SECT_TEXT, name)) { // Add bonus for otool
		ft_printf("Contents of (__TEXT,__text) section\n");
		ft_hexdump(file->start + offset, size, segstart, file->arch);
	}
}

static int file_add_mysection(t_file *file, void *sect)
{
	t_mysection mysect;

	if (check_overflow(file, sect + (file->arch == ARCH_32 ? sizeof(t_section) : sizeof(t_section_64))))
		return FAILURE;
	mysect.name = file->arch == ARCH_32 ? ((t_section *)sect)->sectname : ((t_section_64 *)sect)->sectname;
	mysect.index =  file->nsects;

	// malloc secure
	// free all items
	ft_lstadd(&file->mysects, ft_lstnew(&mysect, sizeof(t_mysection)));
	return SUCCESS;
}

/*
** Parse the segment, supports for :
** - hexdumping a segment content
** - setting the file->mysects
*/

int	parse_mach_segment(t_env *env, t_file *file, void *segment_command) {
	uint32_t nsects;
	void *section;

	section = segment_command + ((file->arch == ARCH_32) ? sizeof(t_segment_command) : sizeof(t_segment_command_64));
	if (check_overflow(file, section))
		return FAILURE;
	nsects = (file->arch == ARCH_32) ? ((t_segment_command *) segment_command)->nsects : ((t_segment_command_64 *) segment_command)->nsects;
	nsects = swapif_u32(file, nsects);

	while (nsects--) {
		if (check_overflow(file, section))
			return FAILURE;
		// ft_printf("Start of section %d\n", nsects);
		file->nsects++;
		if (env->bin == BIN_OTOOL) {
			hexdump_segment_content(file, section); // reverse
		} else {
			if (file_add_mysection(file, section))
				return FAILURE;
		}
		section += ((file->arch == ARCH_32) ? sizeof(t_section) : sizeof(t_section_64));
		// ft_printf("End of section %d\n", nsects);
	}

	return SUCCESS;
}
