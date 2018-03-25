#include "h_init.h"
#include "stacktrace.h"
#include "object.h"

extern char **environ;


typedef struct _node_t node_t;
typedef struct _list_t list_t;
typedef struct _iter_t iter_t;

struct _node_t
{
    void *element;
    struct _node_t *next;
};

struct _list_t
{
    node_t *first;
    node_t *last;
};



struct _iter_t
{
    node_t *cur;
};

iter_t *iter(list_t *l)
{
    assert(l);
    iter_t *it = calloc(1, sizeof(struct _iter_t));
    it->cur = l->first;
    return it;
}

void iter_next(iter_t *it)
{
    assert(it);
    if (it->cur)
        {
            it->cur = it->cur->next;
        }
}

bool iter_done(iter_t *it)
{
    assert(it);
    return !it->cur;
}

void *iter_get(iter_t *it)
{
    assert(it && it->cur);
    return it->cur->element;
}

void iter_free(iter_t *it)
{
    free(it);
}



list_t *list_new()
{
    return calloc(1, sizeof(struct _list_t));
}


node_t *list_node_new()
{
    return calloc(1, sizeof(struct _node_t));
}


void list_add(list_t *l, void *elt)
{
    assert(l);
    assert((l->first && l->last) || (!l->first && !l->last));

    node_t *new_last = list_node_new();
    new_last->element = elt;

    if (l->first && l->last)
        {
            l->last->next = new_last;
            l->last = new_last;
        }
    else
        {
            l->first = l->last = new_last;
        }
}

size_t h_used(heap_t *h){ //dummy
	return 1;
}

bool heap_is_safe(heap_t *h) { //dummy
	return true;
}

#define Dump_registers()						\
    jmp_buf env;								\
    if (setjmp(env)) abort();					\

void *stack_find_bottom() {
	void *bottom = environ;
	return bottom;
}


void *stack_find_top() {
	void *top = __builtin_frame_address(0);
	return top;
}


bool stack_check_pointer(heap_t *h, void *p) { 
	return ((address_inside_heap_memory(h, p)) && (address_within_pages(h, p)));
}


void* stack_trace(heap_t *h, void *current, void *bottom) {
	while(current < bottom){
		intptr_t i = *(intptr_t *)current;
		void *p = (void*)i;   
		if (stack_check_pointer(h, p)) {
			printf("First pointer on adr: %ld\n", (long)&p);
			printf("&First pointer on adr: %ld\n", (long)p);
			printf("Stack adr to first pointer on adr: %ld\n", (long)current);
			return current;
		}
		else {current = current+sizeof(void *);}
	}
	return NULL;
}



list_t* gc_list(heap_t *h) {
	list_t *l = list_new();
	Dump_registers();
	void *bottom = stack_find_bottom();
	void *current = stack_find_top();

	while(current != NULL) {
		current = stack_trace(h, current, bottom);

		if (current==NULL) {break;}
		list_add(l, current);
		
		intptr_t i = *(intptr_t *)current;
		void *p = (void*)i; 
		//flag_page_as_new_space(h, p);
		current = current+sizeof(void*);
	}

	return l;
}



void* p_free_addr(page_t *p, size_t s){}

void* h_free_addr(heap_t *h, size_t s){}
bool is_page_newspace(heap_t *h, void* a){}

void* o_copy_object(void* p){
	
}


intptr_t gc_depth(heap_t *h, void *p){
	intptr_t header = o_get_header(p);
	if (O_HEADER_GET_TYPE(header)==3) {
		return O_HEADER_GET_PTR(header);
	}
	size_t number_of_ptrs_in_object = o_pointers_in_object(p);

	for(size_t i = 0;i<=number_of_ptrs_in_object;i++) {
		if (number_of_ptrs_in_object==0) {
			if (is_page_newspace(h, p)) {
				return (intptr_t)p;
			}
			void* new_address = o_copy_object(p);
			intptr_t fwd_address = (intptr_t)new_address;
			return fwd_address;
		}
		void** current_object = o_get_pointer_in_object(p,i);
		if (current_object==NULL) {assert(false);}
		intptr_t new_address = gc_depth(h, *current_object);
		number_of_ptrs_in_object--;
	}
	assert(false);
}

size_t h_gc(heap_t *h) {
	size_t start_bytes = h_used(h);
	Dump_registers();
	void *bottom = stack_find_bottom();
	void *current = stack_find_top();
	if (heap_is_safe(h)) { //SAFE
		while(current != NULL) {
			current = stack_trace(h, current, bottom);
			if (current==NULL) {

				size_t end_bytes = h_used(h);
				return (start_bytes - end_bytes);
			}
			else {
				intptr_t i = *(intptr_t *)current;
				void *p = (void*)i; 
				i = gc_depth(h, p);
				current = current+sizeof(void*);
			}
		}
	}
	//UNSAFE
	list_t *l = gc_list(h); 
    iter_t *it;
    for (it = iter(l); !iter_done(it); iter_next(it))
	{
		intptr_t i = *(intptr_t *)current;
		void *p = (void*)i; 
		gc_depth(h, iter_get(it));
	}
    iter_free(it);

	size_t end_bytes = h_used(h);
	return (start_bytes - end_bytes);	
}

  


int main(int argc, char *argv[])
{
  	heap_t *h = h_init(1024*16, true, 0.95);
	long *a = calloc(1,sizeof(long));
	*a = 123456;
	long *b = calloc(1,sizeof(long));
	*b = 123456;
	int one = 1;
	int two = 2;
	int three = 3;
	return 0;
}
