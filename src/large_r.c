/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   large_r.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoullec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 18:37:00 by mmoullec          #+#    #+#             */
/*   Updated: 2019/01/15 18:39:55 by mmoullec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	*l_realloc(void *ptr, size_t size, t_chunk *chunk)
{
	if (size <= chunk->size)
		return (ptr);
	return (move_and_free(ptr, chunk->size, size));
}

void		*large_realloc(void *ptr, t_chunk *chunk, size_t size)
{
	if (in_chunk(g_env.large_zone, chunk))
		return (l_realloc(ptr, size, chunk));
	return (NULL);
}
