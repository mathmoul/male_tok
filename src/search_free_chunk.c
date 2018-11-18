#include "malloc.h"

void *fusion_block(t_zone *cz, t_chunk *chunk, int t, size_t size)
{
    t_chunk *tmp;

    tmp = chunk->next;
    chunk->size += tmp->size + sizeof(t_chunk);
    chunk->next = tmp->next;
    if (chunk->next)
        chunk->next->previous = chunk;
    else
        cz->tail = chunk;
    return (split_block(cz, chunk, t, size));
}

void *split_block(t_zone *cz, t_chunk *cc, int t, size_t size)
{
    t_chunk *new;
    size_t res;

    if (t == TINY)
        res = 16;
    else if (t == SMALL)
        res = TINY_MAX_SIZE;
    else
        res = SMALL_MAX_SIZE;
    if (cc->size > size + sizeof(t_chunk) + res)
    {
        new = (void *) cc + sizeof(t_chunk) + size;
        new->free = 1;
        new->next = cc->next;
        if (new->next)
            new->next->previous = new;
        else if (cz)
            cz->tail = new;
        new->size = cc->size - size - sizeof(t_chunk);
        cc->next = new;
        new->previous = cc;
        cc->size = size;
    }
    cc->free = 0;
    return (void *) (cc + 1);
}

void *search_free_chunk(t_zone *zone, int t, size_t size)
{
    t_zone * current_zone;
    t_chunk *current_chunk;

    current_zone = zone;
    if (current_zone)
        current_chunk = current_zone->head;
    while (current_zone)
    {
        while (current_chunk)
        {
            if (current_chunk->free == 1 && current_chunk->size >= size)
                return split_block(current_zone, current_chunk, t, size);
            current_chunk = current_chunk->next;
        }
        current_zone = current_zone->next;
        if (current_zone)
            current_chunk = current_zone->head;
    }
    return (NULL);
}