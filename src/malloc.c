/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoullec <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 18:43:17 by mmoullec          #+#    #+#             */
/*   Updated: 2019/01/15 18:43:19 by mmoullec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_env           g_env = {0, 0, NULL, NULL, NULL};
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

inline int init_env(void)
{
    g_env.pagesize = getpagesize();
    g_env.initialized = 1;
    g_env.tiny_zone = set_zone(TINY);
    g_env.small_zone = set_zone(SMALL);
    return (g_env.tiny_zone && g_env.small_zone);
}

static inline t_malloc_function get_function(size_t size)
{
    return (size <= SMALL_MAX_SIZE ? tiny_small_malloc : large_malloc);
}

void *ft_malloc(size_t size)
{
    void *ptr;

    pthread_mutex_lock(&g_mutex);
    if (!g_env.initialized && !init_env())
    {
        pthread_mutex_unlock(&g_mutex);
        return (NULL);
    }
    ptr = size == 0 ? NULL : get_function(size)(size);
    pthread_mutex_unlock(&g_mutex);
    return (ptr);
}
