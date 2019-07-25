/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 00:58:59 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 10:33:09 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

/*
** Print the hex value of each byte from the `start` pointer, and for `size`
** bytes. `start_address` is the printed address from the value at *start.
*/

void	ft_hexdump_line_values(char *start, int64_t len, t_file *file)
{
	int8_t i;

	i = 0;
	while (i < len)
	{
		if (!file->swap_bits && file->arch == ARCH_32)
		{
			if (i + 3 - 2 * (i % 4) < len)
				ft_printf("%02x", (uint8_t)start[i + 3 - 2 * (i % 4)]);
		}
		else
			ft_printf("%02x", (uint8_t)start[i]);
		if (file->arch == ARCH_64)
			ft_printf(" ");
		else if (i % 4 == 3)
			ft_printf(" ");
		i++;
	}
}

void	ft_hexdump(void *start, int64_t size,
	int64_t start_address, t_file *file)
{
	int64_t len;

	len = size > 16 ? 16 : size;
	while (len > 0)
	{
		ft_printf("%0*llx%s", (file->arch == ARCH_32)
			? 8 : 16, start_address, "\t");
		ft_hexdump_line_values(start, len, file);
		ft_printf("\n");
		start += 16;
		start_address += 16;
		size -= 16;
		len = size > 16 ? 16 : size;
	}
}
