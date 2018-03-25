#define _POSIX_C_SOURCE 200809L // Must be defined before includes

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>


#ifndef WORDSIZE
#define WORDSIZE sizeof(void *)
#endif

#ifndef PAGESIZE
#define PAGESIZE 2048
#endif

#ifndef MAX_HEADER_SIZE
#define MAX_HEADER_SIZE 1024
#endif

/**
 * The datatype holding all the heap data
 *
 * gc_threshold  The percentage of the heap that has to be
 *               used to trigger a garbage collection cycle.
 *
 * pagesize      The size (in bytes) of each page in the heap.
 *
 * unsafe_stack  Whether to consider stack pointers to the
 *               heap as unsafe (or safe).
 */
struct heap {
  float gc_threshold;
  size_t pagesize;
  bool unsafe_stack;
};

typedef struct heap heap_t;


/**
 * A datatype representing one page in the heap.
 *
 * new_space       
 *
 * promoted        Indicates whether unsafe pointers were
 *                 found to be pointing toward the page.
 *
 * distance_front  Distance (in bytes) from the beginning of
 *                 the page header to the front of the page.
 *                 Must be <= pagesize - sizeof(page header)
 *                 
 */
struct page {
  bool new_space;
  bool promoted;
  size_t distance_front;
};

typedef struct page page_t;

bool valid_bytes(size_t, size_t);
bool valid_threshold(float);
void create_pages (void *, int, size_t);

heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold)
{
  assert(valid_threshold(gc_threshold));
  assert(valid_bytes(bytes, MAX_HEADER_SIZE));

  size_t heap_header_size   = WORDSIZE;
  int    total_pages        = (bytes-heap_header_size)/PAGESIZE;
  size_t total_size         = total_pages*PAGESIZE + heap_header_size;
  int    align_amount       = 12;  // amount to left-shift
  size_t alignment          = (sizeof(void *)) << align_amount;

  void *heap_temp;
  int  result = posix_memalign(&heap_temp, alignment, total_size);
  if(result != 0) {
  // TODO: handle errors, see 'man posix_memalign'
  }

  heap_t *heap = (heap_t *)heap_temp;
  heap->gc_threshold = gc_threshold;
  heap->pagesize = PAGESIZE;
  heap->unsafe_stack = unsafe_stack;
  void *pages_start = (char *)heap + heap_header_size; // cast to char for incrementation in bytes
  create_pages(pages_start, total_pages, PAGESIZE);

  return heap;
}

/**
 * Creates the pages of the heap.
 *
 * \param start     Address of the first page.
 * \param n_pages   Amount of pages to create.
 * \param pagesize  The total size of each page.
 */
void create_pages (void *start, int n_pages, size_t pagesize)
{
  char *page_addr = start;
  page_t template;
  template.new_space = false;
  template.promoted = false;
  size_t page_header_size = 1; // TODO
  template.distance_front = page_header_size;
  
  int i;
  for(i = 0; i < n_pages; i++){
    *((page_t *)page_addr) = template; 
    page_addr += pagesize;
  }
}

/**
 * Validates a size argument for h_init.
 *
 * \param bytes        The amount of bytes.
 * \param header_size  The expected size of the heaps header.
 * \return             True if the size argument is valid,
 *                     false otherwise.
 * \see h_init
 * \see valid_threshold
 */
bool valid_bytes(size_t bytes, size_t header_size)
{
  return bytes >= 2*PAGESIZE + header_size;
}

/**
 * Validates a threshold argument for h_init.
 *
 * \param gc_threshold  The threshold.
 * \return              True if the threshold argument is valid,
 *                      false otherwise.
 * \see h_init
 * \see valid_bytes
 */
bool valid_threshold (float gc_threshold)
{
  return gc_threshold > 0 && gc_threshold < 1;
}

void h_delete(heap_t *h)
{
  assert(h != NULL && "Heap is NULL");
  free(h);
}

void h_delete_dbg(heap_t *h, void *dbg_value)
{
  // TODO: Traverse and delete stackpointers to heap
  h_delete(h);
}


bool address_within_pages(heap_t *h, void *addr){
	return true;
}

bool address_inside_heap_memory(heap_t *h, void *addr) {
	return true;
}

