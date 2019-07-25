/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/08 10:45:04 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/25 09:55:22 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm_otool.h"

static int	open_file_stats(t_env *env, char const *filename,
	int *fd, size_t *filesize)
{
	struct stat buf;

	if ((*fd = open(filename, O_RDONLY)) < 0)
	{
		ft_printf("error: %s: Open error on file %s\n",
			env->execname, filename);
		return (FAILURE);
	}
	if (fstat(*fd, &buf) < 0)
	{
		ft_printf("error: %s: Fstat error on file %s\n",
			env->execname, filename);
		return (FAILURE);
	}
	if ((*filesize = buf.st_size) == 0)
	{
		ft_printf("%s: is not an object file\n", filename);
		return (FAILURE);
	}
	return (SUCCESS);
}

int			cmd_start(t_env *env, char const *filename)
{
	t_file		file;
	size_t		filesize;
	void		*filestart;
	int			fd;

	ft_bzero(&file, sizeof(t_file));
	if (open_file_stats(env, filename, &fd, &filesize))
		return (FAILURE);
	if ((filestart = mmap(NULL, filesize, PROT_READ,
		MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_printf("error: %s: Mmap error on file %s\n",
			env->execname, filename);
		return (FAILURE);
	}
	create_file(&file, filename, filesize, filestart);
	if (handle_binary(env, &file))
	{
		destroy_file(&file);
		return (FAILURE);
	}
	destroy_file(&file);
	if (munmap(filestart, filesize) < 0 || close(fd) < 0)
		return (FAILURE);
	return (SUCCESS);
}
