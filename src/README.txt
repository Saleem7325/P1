P1 Test Plan mymalloc
----------------------

- How do we know that mymalloc works correctly?

	- If mymalloc can can return a pointer to a block of the requested size in mem if their are enough bytes 
	  available in mem otherwise mymalloc should return NULL. We know my malloc works correctly based on the
	  tests we conducted which include:

		- Allocating all of memory, then attempting to allocate more which results in a NULL pointer.

		- Attempting to allocate more memory than exists, which reuslts in a NULL pointer

		- Allocating all of memory, then freeing it, then allocating more.
	
		- Allocating memory in multiple chunks, then freeing pointer in random order, then allocating all of memory.

		- Implementing a BST then using mymalloc to allocate nodes when inserting along with using
		  myfree when deleting nodes from the BST. Then ensuring nodes are stored properly through inorder
		  traversal of the BST

- How do we know myfree works correctly?

	- myfree should free the pointer it is given unless the pointer is not a pointer to the beginning of
	  a block in mem, not in mem, NULL, or a pointer to the beginning of a free block in which case it prints
	  an error message to stdout specifying the error. We know myfree works correctly based on the tests we
	  have conducted which include:

		- Attempting to free an address not in mem, returns an error message.

		- Attemping to free an address not at the start of a block, returns an error message.

		- Attempting to free an an already free pointer, returns an error message.

		- Attempting to free a NULL pointer, returns an error message.
  
- We conducted each of our tests 50 times then measured the amount of time each test took, then found the  
  average time of each test out of 50, and none of the tests took an unsual amount of time. We noticed no 
  abnormal behavior from mymalloc or myfree during any of these tests they did exactly what we expected
  and allocated/freed everything as needed along with reporting any errors.   
