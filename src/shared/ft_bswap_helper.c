/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bswap_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/14 11:01:22 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/14 11:36:45 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

uint32_t swapif_u32(t_file *file, uint32_t x)
{
	return file->swap_bits ? ft_bswap_uint32(x) : x;
}

uint64_t swapif_u64(t_file *file, uint64_t x)
{
	return file->swap_bits ? ft_bswap_uint64(x) : x;
}
