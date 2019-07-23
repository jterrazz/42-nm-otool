/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macho_segment.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 23:01:29 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/24 00:58:16 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int print_section(t_env *env, t_file *file, void *sect)
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
	if ((env->flags & FLAG_M) || (env->flags & FLAG_F))
		return (SUCCESS);
	if (!ft_strcmp(SECT_TEXT, name) && (env->flags & FLAG_T || !(env->flags & FLAG_D)))
	{
		ft_printf("Contents of (__TEXT,__text) section\n");
		ft_hexdump(file->start + offset, size, segstart, file->arch);
	}
	if (env->flags & FLAG_D)
	{
		if (!ft_strcmp(SECT_DATA, name)) {
			ft_printf("Contents of (__DATA,__data) section\n");
			ft_hexdump(file->start + offset, size, segstart, file->arch);
		}
	}
	return (SUCCESS);
}

static int create_mysects(t_file *file, void *sect)
{
	t_mysection mysect;
	t_list *lst;

	mysect.name = file->arch == ARCH_32 ? ((t_section *)sect)->sectname : ((t_section_64 *)sect)->sectname;
	mysect.index =  file->nsects;
	if (!(lst = ft_lstnew(&mysect, sizeof(t_mysection))))
		return (FAILURE);
	ft_lstadd(&file->mysects, lst);
	return (SUCCESS);
}

/*
** Parse the load command LC_SEGMENT. Otool will print directly the section,
** nm will create a file->mysects list.
*/

int	parse_machoo_segment(t_env *env, t_file *file, void *segment_command) {
	uint32_t nsects;
	void *section;
	uint64_t section_size;

	section_size = ((file->arch == ARCH_32) ? sizeof(t_section) : sizeof(t_section_64));
	section = segment_command + ((file->arch == ARCH_32) ? sizeof(t_segment_command) : sizeof(t_segment_command_64));
	if (check_overflow(file, section))
		return (FAILURE);
	nsects = (file->arch == ARCH_32) ? ((t_segment_command *) segment_command)->nsects : ((t_segment_command_64 *) segment_command)->nsects;
	nsects = swapif_u32(file, nsects);

	while (nsects--) {
		if (check_overflow(file, section) || check_overflow(file, section + section_size))
			return (FAILURE);
		file->nsects++;
		if (env->bin == BIN_OTOOL)
		{
			if (print_section(env, file, section))
				return (FAILURE);
		} else {
			if (create_mysects(file, section))
				return (FAILURE);
		}
		section += section_size;
	}
	return (SUCCESS);
}
