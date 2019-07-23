/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:45:04 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/23 17:50:19 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

int cmd_start(t_env *env, char const *filename)
{
    struct stat buf;
    t_file file;
    char *ptr;
    int fd;

    ft_bzero(&file, sizeof(t_file));
    if ((fd = open(filename, O_RDONLY)) < 0)
    {
        ft_printf("error: %s: Open error on file %s\n", env->execname, filename);
        return (FAILURE);
    }
    if (fstat(fd, &buf) < 0)
    {
        ft_printf("error: %s: Fstat error on file %s\n", env->execname, filename);
        return (FAILURE);
    }
    if (buf.st_size == 0)
    {
        ft_printf("%s: is not an object file\n", filename);
        return (FAILURE);
    }
    if ((ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        ft_printf("error: %s: Mmap error on file %s\n", env->execname, filename);
        return (FAILURE);
    }
    init_file(&file, filename, buf.st_size, ptr);
    handle_binary(env, &file);
    destroy_file(&file);
    if (munmap(ptr, buf.st_size) < 0 || close(fd) < 0)
        return (FAILURE);
    return (SUCCESS);
}
