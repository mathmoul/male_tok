#include "malloc.h"

static void *create_large_chunk(size_t size)
{
    t_chunk *cur;
    t_chunk *new;

    printf("size = %zu | t_chunk = %zu\n", size, sizeof(t_chunk));
    printf("ps = %zu", PS);
    cur = g_env.large;
    size = (((size + sizeof(t_chunk)) / g_env.pagesize) + 1) * g_env.pagesize;
    if ((new = mmmap(size)) != NULL)
    {
        new->size = size - sizeof(t_chunk);
        new->free = 0;
        FREE_IT(new->free);
        new->next = NULL;
        new->previous = NULL;
        if (cur)
        {
            while (cur->next)
                cur = cur->next;
            cur->next = new;
            new->previous = cur;
        }
        else
            g_env.large = new;
        return ((void *)(new + 1));
    }
    return (NULL);
}

void *large_m(size_t size)
{
    printf("LARGE MALLOC\n");
    return (create_large_chunk(size));
}
