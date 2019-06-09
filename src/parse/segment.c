/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 23:01:29 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/10 00:33:36 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "../ft_nm.h"

void hexdump_segment_content(t_env *env, t_section *section) {
	if (!ft_strcmp(SECT_TEXT, section->sectname)) {
		ft_printf("yo");
	}
	(void)env;
}

// Test to make a malloc fails (by limiting memory ?)
int	parse_segment(t_env *env, void *segment_command, t_arch arch) {
	// Check seg ?
	size_t nsects;
	t_section *section;

	if (arch == ARCH_32) {
		nsects = ((t_segment_command *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command);
	} else {
		nsects = ((t_segment_command_64 *) segment_command)->nsects;
		section = segment_command + sizeof(t_segment_command_64);
	}

	while (nsects--) {
		if (env->bin == BIN_OTOOL) {
			hexdump_segment_content(env, section);
		} else {
			// ft_printf("%s: %s\n", section->sectname, section->segname);
			// Copy data to list here
		}
		section = (void *)section + sizeof(t_section);
	}

	return SUCCESS;
}
