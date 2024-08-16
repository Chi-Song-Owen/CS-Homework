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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};

/* Word and header/footer size (bytes) */
#define WSIZE 4

/* Double word size (bytes) */
#define DSIZE 8

/* Extend heap by this amount (bytes) */ 
#define CHUNKSIZE (1 << 12) 


/* 16 bytes alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) 

/* 
 * Read and write a word at address p 
 * Read and write a 8-byte address at address p
 */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))
#define GET_PTR(p)   (char *)(*(unsigned long *)(p))
#define PUT_PTR(p, pval)  (*(unsigned long *)(p) = (unsigned long)(pval))

/* 给定序号，找到链表头节点位置 */
#define GET_HEAD(num) (GET_PTR(heap_listp + DSIZE * num))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   
#define GET_ALLOC(p) (GET(p) & 0x1)   

/* 
 * Given block ptr bp, compute address of its header and footer, 
 * compute address of its pred and succ
 */
#define HDRP(bp)       ((char *)(bp) - WSIZE)                      
#define PREP(bp)       (char *)(bp)
#define SUCP(bp)       ((char *)(bp) + DSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) 
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) 

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */
static char *freelist_tailp = 0; /* Pointer to tail of freelist(Epilogue) */
// int count = 0;
/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void *place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static int search(size_t size);
static void insert(void *bp);
static void delete(void *bp);

/* 
 * search - find the number equals to the class
 */
static int search(size_t size)
{
    int i = 5;
    while(i <= 20) {
      if(size <= (1 << i))
        return i - 5;
      i++;
    }
    return i - 5;
}

/*
 *  insert block to the head of the list
 */
static void insert(void *bp)
{
  /* the size of block */
  size_t size = GET_SIZE(HDRP(bp));
  /* 根据块大小找到头节点位置 */
  int num = search(size);
  /* 空的，直接放 */
  // // printf("%d\n", num);
  if(GET_HEAD(num) == NULL) {
    // // printf("in2\n");

    PUT_PTR(heap_listp + DSIZE * num, bp);
    /* 前驱 */
    PUT_PTR(PREP(bp), NULL);
    /* 后继 */
    PUT_PTR(SUCP(bp), NULL);
  }
  else {
    /* bp的后继放第一个节点 */
    PUT_PTR(SUCP(bp), GET_HEAD(num));
    /* 第一个节点的前驱放bp */
    PUT_PTR(GET_HEAD(num), bp);
    /* bp的前驱为空 */
    PUT_PTR(PREP(bp), NULL);
    /* 头节点放bp */
    PUT_PTR(heap_listp + DSIZE * num, bp);
  }
}

/*
 *  删除块,清理指针
 */
static void delete(void *bp)
{
    /* 块大小 */
    size_t size = GET_SIZE(HDRP(bp));
    /* 根据块大小找到头节点位置 */
    int num = search(size);
    /* 
     * 唯一节点,后继为null,前驱为null 
     * 头节点设为null
     */
      // // printf("mallocd\n"); 
    if (GET_PTR(PREP(bp)) == NULL && GET_PTR(SUCP(bp)) == NULL) {
        // // printf("malloc1\n"); 
      PUT_PTR(heap_listp + DSIZE * num, NULL);
    } 
    /* 
     * 最后一个节点 
     * 前驱的后继设为null
     */
    else if (GET_PTR(PREP(bp)) != NULL && GET_PTR(SUCP(bp)) == NULL) {
              // // printf("malloc3\n"); 

      PUT_PTR(SUCP(GET_PTR(PREP(bp))), NULL);
    } 
    /* 
     * 第一个结点 
     * 头节点设为bp的后继
     */
    else if (GET_PTR(SUCP(bp)) != NULL && GET_PTR(PREP(bp)) == NULL){
              // // printf("malloc4\n"); 

      PUT_PTR(heap_listp + DSIZE * num, GET_PTR(SUCP(bp)));
      PUT_PTR(PREP(GET_PTR(SUCP(bp))), NULL);
    }
    /* 
     * 中间结点 
     * 前驱的后继设为后继
     * 后继的前驱设为前驱
     */
    else {
              // // printf("malloc5\n"); 

        PUT_PTR(PREP(GET_PTR(SUCP(bp))), GET_PTR(PREP(bp)));
        PUT_PTR(SUCP(GET_PTR(PREP(bp))), GET_PTR(SUCP(bp)));
    }
}

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 */
static void *extend_heap(size_t words) 
{
    char *bp, *p;
    size_t size;
    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
      return NULL;
    p = freelist_tailp;
    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(p), PACK(size, 0));         /* Free block header */
    PUT(FTRP(p), PACK(size, 0));         /* Free block footer */
    freelist_tailp = NEXT_BLKP(p);
    PUT_PTR(SUCP(p), 0);    /* Free block succ */

    PUT(HDRP(freelist_tailp), PACK(0, 1)); /* New epilogue header */
    /* Coalesce if the previous block was free */
    return coalesce(p);
}

/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
static void *coalesce(void *bp) 
{
  size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
  size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
  size_t size = GET_SIZE(HDRP(bp));
  void *prevptr = PREV_BLKP(bp), *nextptr = NEXT_BLKP(bp);
  // // printf("co1\n");
  if (prev_alloc && next_alloc) {            /* Case 1 */
    insert(bp);
    return bp;
  }

  /* Coalesce and put the new block to the top of freelist */
  else if (prev_alloc && !next_alloc) {      /* Case 2 */
         // printf("co2\n");

    size += GET_SIZE(HDRP(nextptr));
    // printf("co2\n");
    delete(nextptr);
    // printf("co2\n");
    // printf("co2\n");
    PUT(HDRP(bp), PACK(size, 0));                    /* New block header */
    PUT(FTRP(bp), PACK(size, 0));                    /* New block footer */
    // printf("co2\n");
    insert(bp);
  }

  else if (!prev_alloc && next_alloc) {      /* Case 3 */
         // printf("co3\n");

    size += GET_SIZE(FTRP(prevptr));
    delete(prevptr);

    PUT(HDRP(prevptr), PACK(size, 0));
    PUT(FTRP(prevptr), PACK(size, 0));

    bp = prevptr;

    insert(bp);
  }

  else {                                     /* Case 4 */
       // printf("co4\n");

    size += GET_SIZE(HDRP(prevptr)) + GET_SIZE(FTRP(nextptr));
    delete(nextptr);
    delete(prevptr);

    PUT(HDRP(prevptr), PACK(size, 0));
    PUT(FTRP(prevptr), PACK(size, 0));

    bp = prevptr;

    insert(bp);
  }
  return bp;
}

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static void *place(void *bp, size_t asize)
{
  size_t csize = GET_SIZE(HDRP(bp));
  void *p;
  delete(bp);
  if ((csize - asize) >= (2 * ALIGNMENT)) {
    if (asize >= 100) {
      PUT(HDRP(bp), PACK(csize - asize, 0));
      PUT(FTRP(bp), PACK(csize - asize, 0));

      p = NEXT_BLKP(bp);

      PUT(HDRP(p), PACK(asize, 1));
      PUT(FTRP(p), PACK(asize, 1));

      coalesce(bp);
      return p;
    }
    else {
      PUT(HDRP(bp), PACK(asize, 1));
      PUT(FTRP(bp), PACK(asize, 1));

      p = NEXT_BLKP(bp);

      PUT(HDRP(p), PACK(csize - asize, 0));
      PUT(FTRP(p), PACK(csize - asize, 0));

      coalesce(p);
      return bp;
    }
  }
  else {
    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1));
    return bp;
  }
}

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
void *find_fit(size_t asize)
{
  int num = search(asize);
  void* bp;
  /* 如果找不到合适的块，那么就搜索下一个更大的大小类 */
  while(num <= 16) {
    bp = GET_HEAD(num);
    /* 不为空则寻找 */
    while(bp) {
      if(GET_SIZE(HDRP(bp)) >= asize) {
        return bp;
      }
      /* 用后继找下一块 */
      bp = GET_PTR(SUCP(bp));
    }
    /* 找不到则进入下一个大小类 */
    num++;
  }
  return NULL;
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
  /* Create the initial empty heap */
  // // printf("init1\n");

  if ((heap_listp = mem_sbrk(40 * WSIZE)) == (void *)-1) 
    return -1;
  PUT(heap_listp, 0);
  heap_listp += WSIZE;
  for (int i = 0; i < 17; i++) {
    PUT_PTR(heap_listp + i * DSIZE, 0);
  }
  PUT(heap_listp + 34 * WSIZE, PACK(ALIGNMENT, 1));   /* Prologue header */
  PUT(heap_listp + 37 * WSIZE, PACK(ALIGNMENT, 1));   /* Prologue footer */
  PUT(heap_listp + 38 * WSIZE, PACK(0, 1));              /* Epilogue header */

  freelist_tailp = NEXT_BLKP(heap_listp + 35 * WSIZE);
  // // printf("init2\n");

  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if (extend_heap((CHUNKSIZE + 4 * ALIGNMENT) / WSIZE) == NULL)
    return -1;
  return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
  // count++;
  // printf("%d\n", // count); 
  // // printf("malloc\n");
  size_t asize;      /* Adjusted block size */
  size_t extendsize; /* Amount to extend heap if no fit */
  char *bp;
  /* Ignore spurious requests */
  if (size == 0)
    return NULL;
  /* Adjust block size to include overhead and alignment reqs. */
  if (size <= 3 * DSIZE)
    asize = 2 * ALIGNMENT;
  else
    asize = ALIGN(DSIZE + size);
  /* Search the free list for a fit */
  if ((bp = find_fit(asize)) != NULL) {
    // // printf("malloc1\n"); 

    // // printf("malloc: %ld\n", size);
    return place(bp, asize);
  }

  /* No fit found. Get more memory and place the block */
  extendsize = MAX(asize, CHUNKSIZE);
  if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
    return NULL;
  // // printf("malloc: %ld\n", size);                             
  return place(bp, asize);
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
  // count++;
  // printf("%d\n", // count); 
  if (ptr == NULL) 
    return;
  size_t size = GET_SIZE(HDRP(ptr));
  PUT(HDRP(ptr), PACK(size, 0));                     /* Block header */
  PUT(FTRP(ptr), PACK(size, 0));                     /* Block footer */

  coalesce(ptr);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  
  size_t oldsize, asize, nextsize, presize, wholesize, extendsize;
  void *newptr, *nextptr, *preptr;

  /* If size == 0 then this is just free, and we return NULL. */
  if(size == 0) {
    mm_free(ptr);
    return 0;
  }

  /* If oldptr is NULL, then this is just malloc. */
  if(ptr == NULL) {
    return mm_malloc(size);
  }

  if(size <= 3 * DSIZE)
    asize = 2 * ALIGNMENT;
  else
    asize = ALIGN(DSIZE + size);

  oldsize = GET_SIZE(HDRP(ptr));    

  if(asize == oldsize)                          /* Nothing to do */
    return ptr;
  else if(asize < oldsize) {
    if(oldsize - asize >= 2 *ALIGNMENT) {          /* split a free block */
      PUT(HDRP(ptr), PACK(asize, 1));
      PUT(FTRP(ptr), PACK(asize, 1));
      nextptr = NEXT_BLKP(ptr);
      PUT(HDRP(nextptr), PACK(oldsize - asize, 0));
      PUT(FTRP(nextptr), PACK(oldsize - asize, 0));
      
      insert(nextptr);
    }
    return ptr;
  }
  else {
    nextptr = NEXT_BLKP(ptr);
    nextsize = GET_SIZE(HDRP(nextptr)) + oldsize;
    preptr = PREV_BLKP(ptr);
    presize = GET_SIZE(HDRP(preptr)) + oldsize;
    wholesize = GET_SIZE(HDRP(preptr)) + GET_SIZE(HDRP(nextptr)) + oldsize;
    if(!GET_ALLOC(HDRP(nextptr)) && nextsize >= asize) {
    /* the block and its next block can hold the new block together */

      delete(nextptr);
      if (nextsize - asize >= 2 * ALIGNMENT) { /* split a free block */
        PUT(HDRP(ptr), PACK(asize, 1));
        PUT(FTRP(ptr), PACK(asize, 1));

        nextptr = NEXT_BLKP(ptr);
        PUT(HDRP(nextptr), PACK(nextsize - asize, 0));
        PUT(FTRP(nextptr), PACK(nextsize - asize, 0));

        insert(nextptr);
      }
      else {
        PUT(HDRP(ptr), PACK(nextsize, 1));
        PUT(FTRP(ptr), PACK(nextsize, 1));
      }
      return ptr;
    }
    else if (!GET_ALLOC(HDRP(preptr)) && presize >= asize) {
      delete(preptr);
      memcpy(preptr, ptr, oldsize - DSIZE);
      if (presize - asize >= 2 * ALIGNMENT) { /* split a free block */
        PUT(HDRP(preptr), PACK(asize, 1));
        PUT(FTRP(preptr), PACK(asize, 1));

        newptr = NEXT_BLKP(preptr);
        PUT(HDRP(newptr), PACK(presize - asize, 0));
        PUT(FTRP(newptr), PACK(presize - asize, 0));

        insert(newptr);
      }
      else {
        PUT(HDRP(preptr), PACK(presize, 1));
        PUT(FTRP(preptr), PACK(presize, 1));
      }
      return preptr;
    }
    else if (!GET_ALLOC(HDRP(preptr)) && !GET_ALLOC(HDRP(nextptr)) && wholesize >= asize) {
      delete(preptr);
      delete(nextptr);
      memcpy(preptr, ptr, oldsize - DSIZE);
      if (wholesize - asize >= 2 * ALIGNMENT) { /* split a free block */
        PUT(HDRP(preptr), PACK(asize, 1));
        PUT(FTRP(preptr), PACK(asize, 1));

        newptr = NEXT_BLKP(preptr);
        PUT(HDRP(newptr), PACK(wholesize - asize, 0));
        PUT(FTRP(newptr), PACK(wholesize - asize, 0));

        insert(newptr);
      }
      else {
        PUT(HDRP(preptr), PACK(wholesize, 1));
        PUT(FTRP(preptr), PACK(wholesize, 1));
      }
      return preptr;        
    }
    else { /* Next block is allocated or size is not enough */
    /* malloc a new block and free the old block */
      newptr = find_fit(asize);
      if(!newptr) {
        extendsize = MAX(asize, CHUNKSIZE);
        if((newptr = extend_heap(extendsize / WSIZE)) == NULL)
          return 0;
      }
      newptr = place(newptr, asize);
      memcpy(newptr, ptr, oldsize - DSIZE);
      mm_free(ptr);
      return newptr;
    }
  }
}