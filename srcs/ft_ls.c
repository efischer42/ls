/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efischer <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 17:25:57 by efischer          #+#    #+#             */
/*   Updated: 2019/05/21 17:21:52 by efischer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void		ft_open_dir(char *path, uint8_t flags);

static void		ft_find_next_dir(char *path, t_list *lst, uint8_t flags)
{
	char			*tmp;

	while (lst != NULL)
	{
		if (((t_dir*)(lst->content))->type == 'd'
		&& ft_strequ(((t_dir*)(lst->content))->name, ".") == 0
		&& ft_strequ(((t_dir*)(lst->content))->name, "..") == 0)
		{
			ft_putchar('\n');
			tmp = ft_asprintf("%s%s", path, ((t_dir*)(lst->content))->name);
			ft_printf("%s:\n", tmp);
			ft_strdel(&tmp);
			ft_open_dir((ft_asprintf("%s%s/", path,
			((t_dir*)(lst->content))->name)), flags);
		}
		ft_free_dir_info(((t_dir*)(lst->content)));
		lst = lst->next;
	}
}

void		ft_open_dir(char *path, uint8_t flags)
{
	void			*dir;
	struct dirent	*dirent;
	t_dir			dir_info;
	t_padding		padding;
	t_list			*lst;

	lst = NULL;
	dir = opendir(path);
	ft_bzero(&padding, sizeof(padding));
	while ((dirent = readdir(dir)) != NULL)
	{
		ft_bzero(&dir_info, sizeof(dir_info));
		ft_get_dir_info(ft_asprintf("%s/%s", path, dirent->d_name),
		dirent->d_name, &dir_info, &padding);
		ft_lstadd(&lst, ft_lstnew(&dir_info, sizeof(t_dir)));
	}
	ft_merge_sort(&lst, &ft_sort_name);
	if ((flags & FLAG_T) == FLAG_T)
		ft_merge_sort(&lst, &ft_sort_time);
	if ((flags & FLAG_REV) == FLAG_REV)
		ft_merge_sort(&lst, &ft_sort_rev);
	ft_printlist(lst, &padding, flags);
	if ((flags & FLAG_R) == FLAG_R)
		ft_find_next_dir(path, lst, flags);
	ft_strdel(&path);
	ft_free_lst(&lst);
}

static uint8_t		ft_manage_args(int *current_arg, int ac, char **av)
{
	size_t	i;
	uint8_t	flags;

	flags = 0;
	while (*current_arg < ac && av[*current_arg][0] == '-')
	{
		i = 0;
		while (av[*current_arg][i] != '\0')
		{
			if (av[*current_arg][i] == 'l')
				flags |= FLAG_L;
			else if (av[*current_arg][i] == 'a')
				flags |= FLAG_A;
			else if (av[*current_arg][i] == 'r')
				flags |= FLAG_REV;
			else if (av[*current_arg][i] == 't')
				flags |= FLAG_T;
			else if (av[*current_arg][i] == 'R')
				flags |= FLAG_R;
			i++;
		}
		(*current_arg)++;
	}
	return (flags);
}

static void		ft_list_dir(t_list **lst_dir, t_list **lst_file, int ac, char **av)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	if (ac == 0)
		ft_lstadd(lst_dir, ft_lstnew("./", 3));
	while (i < ac)
	{
		if (opendir(av[i]) == NULL)
		{
			if (errno == ENOTDIR)
				ft_lstadd(lst_file, ft_lstnew(av[i], ft_strlen(av[i]) + 1));
			else
			{
				perror((tmp = ft_asprintf("ft_ls: file not found '%s'", av[i])));
				ft_strdel(&tmp);
			}
		}
		else if (av[i] != NULL)
		{
			if (av[i][ft_strlen(av[i]) - 1] != '/')
			{
				tmp = ft_strjoin(av[i], "/");
				ft_lstadd(lst_dir, ft_lstnew(tmp, ft_strlen(tmp) + 1));
				ft_strdel(&tmp);
			}
			else
				ft_lstadd(lst_dir, ft_lstnew(av[i], ft_strlen(av[i]) + 1));
		}
		i++;
	}
}

static void		ft_files(t_list *lst_file, uint8_t flags)
{
	t_dir		dir_info;
	t_padding	padding;
	t_list		*lst;

	lst = NULL;
	ft_bzero(&padding, sizeof(padding));
	while (lst_file != NULL)
	{
		ft_bzero(&dir_info, sizeof(dir_info));
		ft_get_dir_info(ft_asprintf("./%s", lst_file->content),
		lst_file->content, &dir_info, &padding);
		ft_lstadd(&lst, ft_lstnew(&dir_info, sizeof(dir_info)));
		lst_file = lst_file->next;
	}
	ft_merge_sort(&lst, &ft_sort_name);
	if ((flags & FLAG_T) == FLAG_T)
		ft_merge_sort(&lst, &ft_sort_time);
	if ((flags & FLAG_REV) == FLAG_REV)
		ft_merge_sort(&lst, &ft_sort_rev);
	ft_printlist(lst, &padding, flags);
	ft_free_lst(&lst);
}

static void		ft_directories(t_list *lst_dir, uint8_t flags)
{
	t_list	*head;

	head = lst_dir;
	while (lst_dir != NULL)
	{
		if (lst_dir != head || lst_dir->next != NULL)
			ft_printf("%.*s:\n", ft_strlen(lst_dir->content) - 1, lst_dir->content);
		ft_open_dir(ft_strdup(lst_dir->content), flags);
		if (lst_dir->next != NULL)
			ft_putchar('\n');
		lst_dir = lst_dir->next;
	}
}

int				main(int ac, char **av)
{
	t_list	*lst_dir;
	t_list	*lst_file;
	int		i;
	uint8_t	flags;

	i = 1;
	lst_dir = NULL;
	lst_file = NULL;
	flags = ft_manage_args(&i, ac, av);
	ft_list_dir(&lst_dir, &lst_file, ac - i, av + i);
	if (lst_file != NULL)
	{
		ft_files(lst_file, flags);
		ft_putchar('\n');
	}
	ft_directories(lst_dir, flags);
	ft_free_lst(&lst_file);
	ft_free_lst(&lst_dir);
	return (0);
}
