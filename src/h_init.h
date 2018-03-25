#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "h_init.c"
#ifndef h_init_h
#define h_init_h

/**
 * The opaque data type holding all the heap data
 */
typedef struct heap heap_t;

/**
 * Create a new heap with bytes total size (including both spaces
 * and metadata), meaning strictly less than bytes will be
 * available for allocation.
 *
 * \param bytes         the total size of the heap in bytes
 * \param unsafe_stack  true if pointers on the stack are to be considered unsafe pointers
 * \param gc_threshold  the memory pressure at which gc should be triggered (1.0 = full memory)
 * \return              the new heap
 * \see valid_bytes
 * \see valid_threshold
 */
heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold);

/**
 *
 * Delete a heap.
 * \param h             the heap
 * \see   h_delete_dbg
 */
void h_delete(heap_t *h);

/**
 * Delete a heap and trace, killing off stack pointers.
 *
 * \param h          the heap 
 * \param dbg_value  a value to be written into every pointer into h on the stack
 * \see   h_delete
 */
void h_delete_dbg(heap_t *h, void *dbg_value);

/**
 * Checks if an address points to somewhere in a heap's
 * in page-area.
 *
 * This doesn't check if the pointer is in an actually
 * *used* memory area, and is thus meant to be used for
 * quick checks of stack pointers.
 *
 * a page's *used* memory area.
 * 
 * \param h     A heap with pages.
 *
 * \param addr  An address.
 *
 * \return      `true` if an address points to somewhere within
 *              the heap's pages, (even if not in an actual
 *              memory area!), `false` otherwise.
 *
 * \see address_within_pages
 */
bool address_inside_heap_memory(heap_t *h, void *addr);

/**
 * Checks if an address points to somewhere in a heap's
 * *used* memory area. This is a more exact, but slower
 * way of checking if an address points to a heap.
 * 
 * \param h     A heap with pages.
 *
 * \param addr  An address.
 *
 * \return      `true` if an address points to a used memory
 *              area in the heap's pages, `false` otherwise.
 *
 * \see address_within_pages
 */
bool address_within_pages(heap_t *h, void *addr);


#endif
