/**
 * Copyright (c) 2011 maxiaodong97@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __PT_HASH_T__
#define __PT_HASH_T__

#include "pt_defs.h"
#include "pt_slist.h"

typedef struct pt_hash_element_s
{
    pt_snode_t node;         /* will be linked together     */
    uint16_t keysz;          /* key size                    */
    uint16_t valsz;          /* value size                  */
    uint8_t  data[0];        /* key_data + value_data       */
}pt_hash_element_t;

typedef struct pt_hash_iter_s
{
    struct pt_hash_s *hp;    /* pointer to the hash         */
    pt_hash_element_t *elp;  /* pointer to the elements     */
    pt_slist_t *list;        /* pointer to the list         */
} pt_hash_iter_t;

typedef struct pt_hash_s
{
    pt_slist_t *lists;       /* array of single linked list */
    uint32_t nlists;         /* number of lists             */
    uint32_t nelements;      /* number of elements          */

    /*
     * construct a iterator to go through the pt_hash table
     */
    pt_hash_iter_t* (*iter_construct)(struct pt_hash_s *hp);

    /*
     * destruct the iterator
     */
    void (*iter_destruct)(pt_hash_iter_t *pthi);

    /*
     * go to the next iterator
     */
    int32_t (*iter_next)(pt_hash_iter_t* kip, void** keyp, uint16_t* keyszp,
                   void** valp, uint16_t* valszp);

    /*
     * dump out all the elements in the pt_hash
     */
    void (*debug_print)(struct pt_hash_s *hp);

    /*
     * find val valsz given a key and keysz
     * @return 0 if succeed
     */
    int32_t (*find)(struct pt_hash_s* hp, const void* key, uint16_t keysz, 
        void** valp, uint16_t *valsz);

    /*
     * insert an elements to the hash table
     * @return: 0 success, -1 if memory allocation failure 1 if duplication found
     */
    int32_t (*insert)(struct pt_hash_s* hp, const void* key, uint16_t keysz,
        const void* val, uint16_t valsz);
   
} pt_hash_t;


/*
 * create a hash table with nlists, each list is 4 bytes long 
 *
 * @return pointer to pt_hash_t
 */
pt_hash_t* create_hash(uint32_t nlists);

/*
 * destruct a pt_hash_t, including the elements and lists array
 */
void destroy_hash(pt_hash_t *hp);

#endif /* !__PT_HASH_T__*/
