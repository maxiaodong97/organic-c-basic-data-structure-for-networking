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
#include "pt_hash.h"
/*
 * hash function, return the hash bucket index
 */
static int32_t
pt_hash_hash(struct pt_hash_s* hp, const void* key, uint16_t keysz)
{
    int32_t hval = 0;
    uint32_t *k = (uint32_t*)key;

    while (keysz >= sizeof *k) {
        hval ^= *k ^ (*k << 7) ^ (*k >> 5);
        keysz -= sizeof *k;
        k++;
    }

    hval ^= (hval >> 16);
    hval &= hp->nlists - 1;
    return hval;
}

static int32_t
pt_hash_find_from_list(pt_slist_t *list, 
        const void* key, uint16_t keysz,
        void** valp, uint16_t *valsz)
{
    pt_snode_t *node = PT_SLIST_HEAD(list);
    /* then looking for the key */
    while(node != NULL) {
        pt_hash_element_t *elp = (pt_hash_element_t *)node;
        if (elp->keysz == keysz && 
            PT_MEMCMP(elp->data, key, elp->keysz) == 0) {
            break;    
        }
        node = PT_SNODE_NEXT(node);
    }

    if (node != NULL) {
        pt_hash_element_t *elp = (pt_hash_element_t *)node;
        if (valp)
            *valp = (void*) (elp->data + elp->keysz);
        if (valsz)
            *valsz = elp->valsz;
        return 0;
    }
    return -1;
}

/*
 * pt_hash_find
 *
 * return the pointer of the value based on the size of the key
 *
 * @param: hp - lfnd_hash pointer
 * @param: key and keysz - input key
 * @param: valp and valsz - if succeed, valp will point to the value.
 * @return: if succeed, 0 is returned, otherwise, -1 is returned.
 *
 * Note: caller must NOT free the value pointer returned.
 */
int32_t
pt_hash_find(struct pt_hash_s* hp,
        const void* key, uint16_t keysz,
        void** valp, uint16_t *valsz)
{
    pt_slist_t *list = NULL;

    /*
     * If there's no elements in the hash, return right away
     */
    if (!hp || ! hp->nelements)
        return -1;

    list = &hp->lists[pt_hash_hash(hp, key, keysz)];

    return pt_hash_find_from_list(list, key, keysz, valp, valsz);
}

/*
 * insert an elements to the hash table
 * @return: 0 success, -1 if memory allocation failure 1 if duplication found
 */
int32_t
pt_hash_insert(struct pt_hash_s* hp,
        const void* key, uint16_t keysz,
        const void* val, uint16_t valsz)
{
    pt_slist_t *list = NULL;
    pt_hash_element_t *newelp = NULL;

    newelp = PT_CALLOC(1, sizeof(*newelp) + keysz + valsz);
    if (!newelp) {
        return -1;
    }

    list = &hp->lists[pt_hash_hash(hp, key, keysz)];

    newelp->valsz = valsz;
    newelp->keysz = keysz;
    PT_MEMCPY(newelp->data, key, keysz);
    PT_MEMCPY(newelp->data + keysz, val, valsz);

    PT_SLIST_ADD(list, &(newelp->node));
    hp->nelements++;
    return 0;
}

/*
 * dump all the hash table content
 */
void
pt_hash_debug(struct pt_hash_s* hp)
{
    uint32_t i;

    for (i = 0; i < hp->nlists; i++) {
        pt_snode_t *node = PT_SLIST_HEAD(&(hp->lists[i]));
        while (node != NULL) {
            pt_hash_element_t *elp = (pt_hash_element_t*)node;
            elp = elp;
            PT_DEBUG("\t\t elp %p: "
                "key=%02x%02x%02x%02x, val=%02x%02x%02x%02x\n", elp,
                elp->data[0], elp->data[1], elp->data[2], elp->data[3],
                elp->data[elp->keysz+0], elp->data[elp->keysz+1], elp->data[elp->keysz+2],
                elp->data[elp->keysz+3]);
            node = PT_SNODE_NEXT(node);
        }
    }
}

pt_hash_iter_t*
pt_hash_iter_construct(struct pt_hash_s* hp)
{
    pt_hash_iter_t* kip = NULL;

    PT_ASSERT(hp != 0);

    kip = (pt_hash_iter_t *)PT_CALLOC(1, sizeof *kip);
    if (!kip) {
        return NULL;
    }

    /*
     * Loop for the first element.
     */
    kip->hp = hp;
    for (kip->list = hp->lists; kip->list < hp->lists + hp->nlists;
            kip->list++) {
        kip->elp = (pt_hash_element_t*)PT_SLIST_HEAD(kip->list);
        if (kip->elp != NULL) {
            return kip;    
        }
    }

    return kip;
}

void
pt_hash_iter_destruct(pt_hash_iter_t* kip)
{
    if (kip) {
        PT_ASSERT(kip->hp != 0);
        PT_FREE(kip);
    }
}

int32_t
pt_hash_iter_next(pt_hash_iter_t* kip, void** keyp, uint16_t* keyszp,
                   void** valp, uint16_t* valszp)
{
    if (!kip->elp)
        return -1;

    /*
     * We return the current element and then skip to the next one. This will
     * allow the user to safely delete the hash element we return from here
     */
    if (keyp)
        *keyp = kip->elp->data;
    if (keyszp)
        *keyszp = kip->elp->keysz;
    if (valp)
        *valp = (void*) (kip->elp->data + kip->elp->keysz);
    if (valszp)
        *valszp = kip->elp->valsz;

    /*
     * Skip to next element
     */
    kip->elp = (pt_hash_element_t*)PT_SNODE_NEXT(&(kip->elp->node));
    if (kip->elp != NULL) {
        return 0;    
    }

    while (++kip->list - kip->hp->lists < (signed)kip->hp->nlists) {
        kip->elp = (pt_hash_element_t*) PT_SLIST_HEAD(kip->list);
        if (kip->elp != NULL) {
            return 0;        
        }
    }

    /*
     * last element, next time return -1;
     */
    kip->elp = 0;
    return 0;
}

/*
 * construct the hash table. nlists will be round up to power of 2
 */
pt_hash_t*
create_hash(uint32_t nlists)
{
    pt_hash_t *hp = NULL;
    uint32_t n = 0;

    hp = (pt_hash_t*) PT_CALLOC(1, sizeof(*hp));
    if (!hp) {
        return NULL;
    }

    for (n = 1; n < nlists;) {
        n <<= 1;
    }

    nlists = n;

    n = nlists * sizeof *hp->lists;
    hp->lists = (pt_slist_t*) PT_CALLOC(nlists, sizeof(*hp->lists));

    if (!hp->lists) {
        PT_FREE(hp);
        return NULL;
    }

    hp->nlists = nlists;
    hp->iter_construct = pt_hash_iter_construct;
    hp->iter_destruct = pt_hash_iter_destruct;
    hp->iter_next = pt_hash_iter_next;
    hp->find = pt_hash_find;
    hp->insert = pt_hash_insert;
    hp->debug_print = pt_hash_debug;
    return hp;
}

/*
 * hash table destruct, free the lists
 */
void
destroy_hash(pt_hash_t* hp)
{
    if (hp) {
        int i = 0;
        for (i = 0; i < hp->nlists; i++) {
            pt_snode_t *node = NULL;
            while((node = PT_SLIST_REMOVE_HEAD(&(hp->lists[i]))) != NULL) {
                PT_FREE((pt_hash_element_t *)node);
            }
        }
        PT_FREE(hp);
    }
}


