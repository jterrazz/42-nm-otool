/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bswap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 23:31:01 by jterrazz          #+#    #+#             */
/*   Updated: 2019/06/12 21:03:03 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int32_t ft_bswap_int32(int32_t x)
{
    x = ((x << 8) & 0xFF00FF00 ) | ((x >> 8) & 0xFF00FF );
    return (x << 16) | ((x >> 16)& 0xFFFF);
}

uint32_t ft_bswap_uint32(uint32_t x)
{
    x = ((x << 8) & 0xFF00FF00 ) | ((x >> 8) & 0xFF00FF );
    return (x << 16) | (x >> 16);
}

uint64_t ft_bswap_uint64(uint64_t x)
{
    x = ((x << 8) & 0xFF00FF00FF00FF00ULL ) | ((x >> 8) & 0x00FF00FF00FF00FFULL );
    x = ((x << 16) & 0xFFFF0000FFFF0000ULL ) | ((x >> 16) & 0x0000FFFF0000FFFFULL );
    return (x << 32) | (x >> 32);
}
