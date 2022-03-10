# Description
## mymalloc
- mymalloc is an implementation of the malloc function in the c standard library, in the file mymalloc.c a myfree function is also implemented to free pointers passed to mymalloc.

- mymalloc takes an int as an argument which indicates number of bytes, a char pointer indicating the name of the file in which the call to mymalloc is made, and a char pointer indicating the line number of the call to mymalloc.
 
- If their is a block of memory large enough to store the number of bytes requested a pointer to the first byte a block is returned. If the number of bytes requested are not available mymalloc returns a NULL pointer. 

## myfree
- myfree takes a pointer as arugment, a char pointer indicating the name of the file in which the call to myfree is made, and a char pointer indicating the line number of the call to myfree.

- myfree frees the block corresponding to the pointer that is passed and coalesces it with any contiguous free blocks if any. If a NULL pointer, a pointer not obtained from mymalloc, a pointer not at the start of a block, or a free pointer are passed to myfree a message indicating the error is printed to stdout.

## Test Plan
- We have tested the following cases for mymalloc:
  - requesting more memory than exists, which results in a NULL pointer.
  - allocating all of memory, then trying to allocate more.
  - allocating all of memory, freeing it, then allocating more.
  - allocating all of memory in multiple chunks, and freeing all chunks in a different order than requested, then allocating a larger chunk.

- We have tested the following cases for myfree:
  - freeing addresses out of range
  - freeing an address not at the start of a chunk
  - freeing an address at the start of a free chunk
  - freeing a NULL pointer

### Note
- Macros defined in mymalloc.h change all calls to malloc(size_t size) to mymalloc(size_t size, __FILE__, __LINE__)
