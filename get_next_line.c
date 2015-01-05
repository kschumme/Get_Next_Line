/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kschumme <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/05 23:06:13 by kschumme          #+#    #+#             */
/*   Updated: 2015/01/05 23:06:17 by kschumme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "get_next_line.h"
#include "libft.h"

static t_list		*get_cut(t_list **list, int fd, int *ret)
{
	t_list	*lst;
	char	*buf;

	if (!(buf = (char *)malloc(sizeof(char) * (BUF_SIZE + 1))))
		return (NULL);
	*ret = read(fd, buf, BUF_SIZE);
	buf[BUF_SIZE] = '\0';
	lst = ft_lstln(buf, BUF_SIZE);
	ft_lstaddend(list, lst);
	return (lst);
}

static int			get_buf(t_list **list, int start, int fd, int *ilk)
{
	int		count;
	int		ret;
	char	*chr_return;
	t_list	*lst;

	count = 0;
	ret = 1;
	lst = *list;
	while (ret > 0)
	{
		if (lst == NULL)
			lst = get_cut(list, fd, &ret);
		if ((chr_return = ft_strchr((char *)lst->content + start, '\n')))
			return ((chr_return - (char *)lst->content) + (BUF_SIZE * count));
		count++;
		start = 0;
		lst = lst->next;
	}
	*ilk = -1;
	return (BUF_SIZE * count);
}

static char			*get_copy(t_list **list, int pos, int *start)
{
	int		i;
	char	*str;
	int		max;

	i = 0;
	max = pos - *start;
	str = (char *)malloc(sizeof(char) * (max + 1));
	while (i <= max && list != NULL && *list != NULL)
	{
		ft_strncpy(&str[i], (char *)(*list)->content + *start, max - i);
		i += BUF_SIZE - *start;
		if (i <= max)
			ft_lstshift(list);
		*start = 0;
	}
	*start = pos % BUF_SIZE + 1;
	str[max] = '\0';
	return (str);
}

int					get_next_line(int const fd, char **line)
{
	static t_list	*list;
	static int		pos;
	static int		start;
	char			*str;
	static int		ilk;

	if (start == -2)
	{
		*line = ft_strnew(0);
		return (0);
	}
	if (fd < 0)
		return (-1);
	ilk = 0;
	pos = get_buf(&list, start, fd, &ilk);
	str = get_copy(&list, pos, &start);
	*line = str;
	if (ilk == -1)
	{
		start = -2;
		ft_putstr(*line);
		return (0);
	}
	return (1);
}