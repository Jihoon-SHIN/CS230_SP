/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "20150441",
    /* First member's full name */
    "Ji Hoon, Shin",
    /* First member's email address */
    "jihoon9030@kaist.ac.kr",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
/* Basic constants and macros */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1<<12)

#define MAX(x, y) ((x) >(y)? (x) : (y))
// Pack a size and allocated bit into a word
#define PACK(size, alloc) ((size) | (alloc))

// Read and write a word at address p
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

// Read the size and allocation bit from address p
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

// Address of free block's predecessor and successor entries
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

// Address of free block's predecessor and successor entries
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

//Global var
static void *heap_listp;

// Functions
static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static void *extend_heap(size_t words);
void *mm_malloc(size_t size);
static void *coalesce(void *bp);
void mm_free(void *bp);
void *mm_realloc(void *ptr,size_t size); 
int mm_init(void);

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
		return -1;
	PUT(heap_listp, 0);
	PUT(heap_listp +(1*WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp +(2*WSIZE), PACK(DSIZE, 1));
	PUT(heap_listp +(3*WSIZE), PACK(0, 1));
	heap_listp += (2*WSIZE);
	
	if(extend_heap(CHUNKSIZE/WSIZE)==NULL)
		return -1;
	return 0;

}

static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;

	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;
	
	//Set headers and footer
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

	return coalesce(bp);
}



static void *find_fit(size_t asize)
{
	void *bp;
	
	for(bp = heap_listp; GET_SIZE(HDRP(bp)) >0; bp = NEXT_BLKP(bp)){
		if(!GET_ALLOC(HDRP(bp))&& (asize <= GET_SIZE(HDRP(bp)))){
			return bp;
		}
	}

	return NULL;

}

static void place(void *bp, size_t asize)
{
	size_t csize = GET_SIZE(HDRP(bp));
	if((csize - asize) >= (2* DSIZE)){
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp =NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize-asize, 0));
		PUT(FTRP(bp), PACK(csize-asize, 0));
	}
	else{
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
}

/*
* mm_malloc - Allocate a block by incrementing the brk pointer.
*	Always allocate a block whose size is a multiple of the aligment.
*
*/

void *mm_malloc(size_t size)
{
    size_t asize;
	size_t extendsize;
	char *bp;
	if(size==0)
		return NULL;
	if(size<=DSIZE)
		asize=2*DSIZE;
	else
		asize = DSIZE * ((size + (DSIZE) + (DSIZE-1))/ DSIZE);
	
	if((bp= find_fit(asize)) != NULL){
		place(bp, asize);
		return bp;
	}

	extendsize = MAX(asize, CHUNKSIZE);
	if((bp= extend_heap(extendsize/WSIZE))==NULL)
		return NULL;
	place(bp, asize);
	return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp)
{
	size_t size = GET_SIZE(HDRP(bp));

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	coalesce(bp);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    //size_t copySize;
	size_t oldsize;
	size_t msize = MAX(ALIGN(size)+DSIZE, 2*DSIZE);
	if(size==0){
		mm_free(ptr);
		return 0;
	}
	if(ptr == NULL) 
		return mm_malloc(size);
	else{
		oldsize = GET_SIZE(HDRP(oldptr));
		if(msize==oldsize)
			return oldptr;
		if(msize<=oldsize){
			size = msize;
			if(oldsize-size <= 24)
				return oldptr;
			PUT(HDRP(ptr), PACK(size,1));
			PUT(FTRP(ptr), PACK(size,1));
			PUT(HDRP(NEXT_BLKP(ptr)), PACK(oldsize-size,1));
			free(NEXT_BLKP(ptr));
			return ptr;
		}
    	newptr = mm_malloc(size);
    	if (newptr == NULL)
      		return NULL;
    	//copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    	if (size < oldsize)
      		//copySize
			oldsize= size;
    	memcpy(newptr, oldptr, oldsize);
    	mm_free(oldptr);
    	return newptr;
	}
}

/*
	the method that coalesce in Implicit list.
*/
static void *coalesce(void *bp)
{
	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));
	
	if (prev_alloc && next_alloc){
		return bp;
	}
	else if (prev_alloc && !next_alloc){
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}
	else if (!prev_alloc  && next_alloc){
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}
	else{
		size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size,0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size,0));
		bp = PREV_BLKP(bp);
	}
	return bp;
}














