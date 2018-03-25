#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "object.h"


////////////////// INTERNAL PROTOTYPES //////////////////
/**
 *  Returns size of satatype represented by character.
 *  Valid characters are:
 *
 *  | CHAR  |  REPRESENTS
 *  |-------|----------------
 *  |   i   |  int
 *  |   l   |  long
 *  |   f   |  float
 *  |   c   |  char
 *  |   d   |  double
 *  |   *   |  void *
 *
 *  \param   c  Char to get size of
 *  \return  sizeof() datatype represented by char
 */
size_t o_size_from_char(char c);

/**
 *  Returns size of satatype represented by character.
 *  Valid characters are:
 *
 *   | VALUE |  DESCRIPTION               |
 *   |-------|----------------------------|
 *   | 0b00  | N/A                        |
 *   | 0b01  | 4 byte datatype            |
 *   | 0b10  | 8 byte datatype            |
 *   | 0b11  | sizeof(vois *)             |
 *
 *  \param   bits Bit-representation to get size of
 *  \return  sizeof() datatype represented by bits
 */
size_t o_size_from_bits(int bits);

/**
 *  Counts pointers notet in a format-string
 * 
 *  \param   format  Format string describing object
 *  \return  Number of pointers in format string
 */
size_t o_pointers_in_string_rep(char **format);

/**
 *  Counts pointers in compact header bit-vector
 *
 *  \param   intptr_t  Bit-vector to use
 *  \return  Number of pointers in bit-vector
 */
size_t o_pointers_in_bitvector(intptr_t header_data);

/**
 *  Calculates size of object described by bit-vector
 *
 *  \param   intptr_t  Bit-vector to use
 *  \return  Size of object described by bit-vector
 */
size_t o_size_from_bitvector(intptr_t header_data);

/**
 *  Calculates size of object described by string format
 *
 *  \param   format  String format describing object
 *  \return  Number of pointers in bit-vector
 */
size_t o_size_from_string_rep(char **format);

/**
 *  Returns pointer at given index if exists
 *
 *  \param   header_data  bitvektor from header
 *  \param   index  index of pointer
 *  \return  Number of pointers in bit-vector
 */
void **o_get_pointer_from_bitvector(intptr_t header_data, size_t index);

/**
 *  Returns pointer at given index if exists
 *
 *  \param   format string representation of object
 *  \param   index  index of pointer
 *  \return  Number of pointers in bit-vector
 */
void **o_get_pointer_from_string_rep(char **format, size_t index);

////////////////// FUNCTION IMPLEMENTATIONS //////////////////

intptr_t o_get_header(void *ptr)
{
  #if HEAP_GROWTH == UP
  return *((intptr_t *)(ptr - sizeof(intptr_t)));
  #else
  return *((intptr_t *)(ptr + sizeof(intptr_t)));
  #endif
}

void *o_alloc_struct(heap_t *h, char *layout)
{
  // TODO: Implement o_alloc_struct
  assert(false && "o_alloc_struct not implemented");
  return NULL;
}

void *o_alloc_union(heap_t *h, size_t bytes, s_trace_f f)
{
  // TODO: Implement o_alloc_union
  assert(false && "o_alloc_union not implemented");
  return NULL;
}

void *o_alloc_raw(heap_t *h, size_t bytes)
{
  // TODO: Implement o_alloc_raw
  assert(false && "o_alloc_raw not implemented");
  return NULL;
}

size_t o_size_from_char(char c)
{
  switch(c)
    {
    case '*':
      return sizeof(void *);
      break;
    case 'i':
      return sizeof(int);
      break;
    case 'f':
      return sizeof(float);
      break;
    case 'c':
      return sizeof(char);
      break;
    case 'l':
      return sizeof(long);
      break;
    case 'd':
      return sizeof(double);
      break;
    default:
      return 0; // TODO: How detect this state of 'unknown' type? 
      break;
    }
}

size_t o_size_from_bits(int bits)
{
  if (bits == 3) {
    return sizeof(void *);
  }
  else if(bits == 2) {
    return 8;
  }
  else if (bits == 1) {
    return  4;
  }
  return 0;
}

void **o_get_pointer_in_object(void *ptr, size_t index)
{
  // TODO: Same code as o_pointers_in_object - merge functions
  intptr_t header = o_get_header(ptr);
  int header_type = (int) O_HEADER_GET_TYPE(header);

  // Compact header
  if (header_type == 1) {
    intptr_t data = O_HEADER_GET_DATA(header);
    int compact_type = (int) (data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
    data = data >> 2; // TODO: remove 'magic' constant OR make into MACRO
    if (compact_type == 1) {
      return o_get_pointer_from_bitvector(data, index);
    }
  }
  // String format
  else if (header_type == 0) {
    return o_get_pointer_from_string_rep((char **)O_HEADER_GET_PTR(header), index);
  }
  // Forwarding pointer
  else if (header_type == 3) {
    return o_get_pointer_in_object((void *)O_HEADER_GET_PTR(header), index);
  }
  // Union or unknown
  return NULL;
}

void **o_get_pointer_from_bitvector(intptr_t header_data, size_t index)
{
  // TODO: Implement o_alloc_raw
  assert(false && "o_get_pointer_from_bitvector not implemented");
  return NULL;
}

void **o_get_pointer_from_string_rep(char **format, size_t index)
{
  // TODO: Implement o_alloc_raw
  assert(false && "o_get_pointer_from_string_rep not implemented");
  return NULL;
}


size_t o_get_object_size(void *ptr)
{
  // TODO: Same code as o_pointers_in_object - merge functions
  intptr_t header = o_get_header(ptr);
  int header_type = (int) O_HEADER_GET_TYPE(header);

  // Compact header
  if (header_type == 1) {
    intptr_t data = O_HEADER_GET_DATA(header);
    int compact_type = (int) (data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
    data = data >> 2; // TODO: remove 'magic' constant OR make into MACRO
    if (compact_type == 1) {
      return o_size_from_bitvector(data);
    }
    return (size_t)data;
  }
  // String format
  else if (header_type == 0) {
    return o_size_from_string_rep((char **)O_HEADER_GET_PTR(header));
  }
  // Forwarding pointer
  else if (header_type == 3) {
    return o_get_object_size((void *)O_HEADER_GET_PTR(header));
  }
  // Union or unknown
  return 0;
}

size_t o_size_from_bitvector(intptr_t header_data)
{
  int t = (int) (header_data & 3UL);
  size_t bytes = 0;
  while(t != 00) {
    bytes += o_size_from_bits(t);
    // Advance to next bytes in vector
    header_data = header_data >> 2; // TODO: Remove 'magic' constant OR make into MACRO
    t = (int) (header_data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
  }
  return bytes;
}

size_t o_size_from_string_rep(char **format)
{
  // TODO: Implement advanced notation (eg. 3* for ***)
  size_t bytes = 0;
  char *cursor = (char *)format;
  while(*cursor != '\0') {
    bytes += o_size_from_char(*cursor);
    ++cursor;
  }
  return bytes;
}

size_t o_pointers_in_object(void *ptr)
{
  // TODO: Same code as o_get_object_size - merge functions
  intptr_t header = o_get_header(ptr);
  int header_type = (int) O_HEADER_GET_TYPE(header);

  // Compact header
  if (header_type == 1) {
    intptr_t data = O_HEADER_GET_DATA(header);
    int compact_type = (int) (data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
    if (compact_type == 1) {
      data = data >> 2; // TODO: remove 'magic' constant OR make into MACRO
      return o_pointers_in_bitvector(data);
    }
    return 0;
  }
  // String format
  else if (header_type == 0) {
    return o_pointers_in_string_rep((char **)O_HEADER_GET_PTR(header));
  }
  // Union or unknown
  return 0;
}

size_t o_pointers_in_string_rep(char **format)
{
  // TODO: Implement advanced notation (eg. 3* for ***)
  size_t count = 0;
  char *cursor = (char *)format;
  while(*cursor != '\0') {
    if(*cursor == '*') {
      ++count;
    }
    ++cursor;
  }
  return count;
}

size_t o_pointers_in_bitvector(intptr_t header_data){  
  int t = (int) (header_data & 3UL);
  size_t count = 0;
  while(t != 00) {
    if (t == 3) {
      count++;
    }
    // TODO: Exchange for 'val = o_advance_bitvector(&bitvector);' which shifts the
    //       bitvector and returns extracted value.
    header_data = header_data >> 2; // TODO: Remove 'magic' constant OR make into MACRO
    t = (int) (header_data & 3UL); // TODO: Remove 'magic' constant OR make into MACRO
  }
  return count;
}


////////////////// DEVFUNCTIONS/TEMPTESTS  //////////////////
/*
typedef struct {
  intptr_t header;
  char c;
} test_data_t;

int main()
{ 
  
  // Header extraction test
  test_data_t *td = calloc(1,sizeof(test_data_t));
  td->header = 0xDEADBEEF00000000;
  td->c = 'A';
  intptr_t header = o_get_header(&(td->c));
  puts("HEADER EXTRACTION:");
  puts("==============================================");
  printf("Header:\t\t\t%p\nHeader pointer:\t\t%p\nObject pointer:\t\t%p\n",
         (void *)(td->header),
         (void *)td, (void *)(&(td->c)));
  printf("Extracted header:\t%p\n\n", (void *)header);
  free(td);

  // Count pointers
  test_data_t *compact_vector = calloc(1,sizeof(test_data_t));
  test_data_t *string_format = calloc(1,sizeof(test_data_t));
  compact_vector->header = 0x0000000000FFADF5;
  char obj_format[] = "**i*d**ddc*\0";
  string_format->header = O_HEADER_SET_PTR((intptr_t)(string_format->header), (intptr_t)obj_format);
  puts("COUNT COMPACT POINTERS:");
  puts("==============================================");
  printf("Vector:\t\t\t%zu (7 expected)\n", o_pointers_in_object(&(compact_vector->c)));
  printf("String:\t\t\t%zu (6 expected)\n\n", o_pointers_in_object(&(string_format->c)));
  free(compact_vector);
  free(string_format);

  // Calculate object size
  compact_vector = calloc(1,sizeof(test_data_t));
  test_data_t *compact_size = calloc(1,sizeof(test_data_t));
  string_format = calloc(1,sizeof(test_data_t));
  compact_vector->header = 0x0000000000000E95;
  compact_size->header = 0x0000000000001801;
  string_format->header = O_HEADER_SET_PTR((intptr_t)(string_format->header), (intptr_t)obj_format);
  puts("CALCULATE OBJECT SIZE:");
  puts("==============================================");
  printf("Vector:\t\t\t %zu b  ( %zu expected)\n",
         o_get_object_size(&(compact_vector->c)),
         (sizeof(void *) + 20));
  printf("Raw size:\t\t%zu b  (384 expected)\n",
         o_get_object_size(&(compact_size->c)));
  printf("String:\t\t\t %zu b  ( %zu expected)\n\n",
         o_get_object_size(&(string_format->c)),
         (6*sizeof(void *) + 3*sizeof(double) + sizeof(char) + sizeof(int)));
  free(compact_vector);
  free(string_format);


  exit(0);
}
*/
