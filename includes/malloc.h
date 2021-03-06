#ifndef MALLOC_H
#define MALLOC_H

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#include "../lib/libft/includes/libft.h"

#define MALLOC(x) ft_malloc(x)
#define FREE(x) ft_free(x)
#define REALLOC ft_realloc

#define _ 0

#define MALLOC_TYPE int

#define TINY_PAGE_SIZE 256 * 4096 // 512 * pagesize = 1048576 taille zone
#define TINY_MAX_SIZE 512 - sizeof(t_zone) // tiny Maxsize

#define SMALL_PAGE_SIZE 2048 * 4096           // 8388608 taille zone
#define SMALL_MAX_SIZE 15360 - sizeof(t_zone) // small Maxsize

#define TINY 0
#define SMALL 1
#define LARGE 2


#define E g_env
#define Z t_zone
#define PS g_env.pagesize

#define FLAGS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE

struct s_chunk
{
	size_t          size;
	char            free;
	struct s_chunk *next;
	struct s_chunk *previous;
};

typedef struct s_chunk t_chunk;

struct s_zone
{
	t_chunk *      head;
	t_chunk *      tail;
	struct s_zone *previous;
	struct s_zone *next;
};
typedef struct s_zone t_zone;

typedef void *(*t_malloc_function)(size_t);
typedef void *(*t_realloc_function)(void *, size_t);

struct s_env
{
	char     initialized;
	size_t   pagesize;
	t_zone * tiny_zone;
	t_zone * small_zone;
	t_chunk *large_zone;
};
typedef struct s_env   t_env;
extern t_env           g_env;
extern pthread_mutex_t g_mutex;
t_zone *               set_zone(MALLOC_TYPE);
void *                  ft_malloc(size_t size);
void *                 ft_realloc(void *ptr, size_t size);
void                   ft_free(void *ptr);
int                    init_env();
void *                 map(size_t size);
int                    unmap(void *ptr, size_t size);
void *                 tiny_small_malloc(size_t);
void *                 large_malloc(size_t);
void *                 search_free_chunk(t_zone *, int, size_t);
void *                 expand_zone(t_zone *, int, size_t);
void *                 split_block(t_zone *, t_chunk *, int, size_t);
void *                 fusion_block(t_zone *, t_chunk *, int, size_t);
int                    in_chunk(t_chunk *, t_chunk *);
void *                 tiny_realloc(void *ptr, t_chunk *chunk, size_t size);
void *                 small_realloc(void *ptr, t_chunk *chunk, size_t size);
void *                 large_realloc(void *ptr, t_chunk *chunk, size_t size);
size_t                 have_enough_space(t_chunk *chunk, size_t size);
void *                 find_zone(t_zone *head, t_chunk *searched);
void *                 find_chunk(t_chunk *head, t_chunk *searched);
void *                 move_and_free(void *ptr, size_t chunk_size, size_t size);
int                    defrag();
int                    debug();
void                   print_mem_addr(void *);
void                   printf_chunk_infos(t_chunk *, unsigned long long *);
void                   printf_zone_infos(t_zone **, unsigned long long *, int);
void  printf_large_infos(t_chunk **chunk, unsigned long long *);
void  ft_show_alloc_mem();
void *ft_calloc(size_t, size_t);
void  init_tiny();
void  init_small();
typedef void (*t_init_zone_function)();
extern t_init_zone_function g_init_zone[2];
void print_zones(unsigned long long *); 
unsigned long long min(unsigned long long a, unsigned long long b, unsigned long long c);

void show_alloc_mem();
#endif
