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
#ifndef __PT_NODE_H__
#define __PT_NODE_H__

#include "pt_defs.h"

#define PT_SNODE_NEXT(_snode_)   ((_snode_)->next)
#define PT_DNODE_NEXT(_dnode_)   ((_dnode_)->next)
#define PT_DNODE_PREV(_dnode_)   ((_dnode_)->prev)
#define PT_TNODE_LEFT(_tnode_)   ((_tnode_)->left)
#define PT_TNODE_RIGHT(_tnode_)  ((_tnode_)->right)
#define PT_TNODE_HEIGHT(_tnode_) ((_tnode_)->height)

#define PT_SNODE_INIT(_snode_)   {PT_SNODE_NEXT(_snode_) = 0; }
#define PT_DNODE_INIT(_dnode_)   \
    {   \
        PT_DNODE_NEXT(_dnode_) = 0; \
        PT_DNODE_PREV(_dnode_) = 0; \
    }

#define PT_TNODE_INIT(_tnode_)   \
    {   \
        PT_TNODE_LEFT(_tnode_) = 0; \
        PT_TNODE_RIGHT(_tnode_) = 0; \
        PT_TNODE_HEIGHT(_tnode_) = 0; \
    }

/* single linked list node */
typedef struct pt_snode_s
{
    struct pt_snode_s *next; 
} pt_snode_t;

/* double linked list node */
typedef struct pt_dnode_s
{
    struct pt_dnode_s *prev;
    struct pt_dnode_s *next;    
} pt_dnode_t;

/* binary tree node */
typedef struct pt_tnode_s
{
    struct pt_tnode_s *left;    /* left node            */
    struct pt_tnode_s *right;   /* right node           */
    uint32_t height;            /* start from 0 (root)  */ 
} pt_tnode_t;

#endif /* ! __PT_NODE_H__ */

