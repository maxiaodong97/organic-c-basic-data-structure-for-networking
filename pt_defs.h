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
#ifndef __PT_DEFS_H__
#define __PT_DEFS_H__

/* Use define in stdint */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/uio.h>

#define PT_MAX_FILE_PATH_LEN     1024
#define PT_1K_BUF_SIZE           1024
#define PT_2K_BUF_SIZE           2048
#define PT_MAX_CMD_BUF_LEN       4096

#define PT_CALLOC   calloc
#define PT_MALLOC   malloc
#define PT_REALLOC  realloc
#define PT_FREE     free
#if _DEBUG
#define PT_ERROR    printf
#define PT_DEBUG    printf
#define PT_FPRINTF  fprintf
#else
#define PT_ERROR(...)
#define PT_DEBUG(...)
#define PT_FPRINTF(...)
#endif
#define PT_ASSERT   assert
#define PT_MEMCPY   memcpy
#define PT_MEMCMP   memcmp
#define PT_MEMMOVE  memmove
#define PT_MEMSET   memset

typedef enum { FALSE = 0, TRUE = 1 } bool_t;

static inline
uint32_t IOVEC_LEN(struct iovec *vec, uint32_t nvec)
{
    int i = 0;
    uint32_t sum = 0;
    for (i = 0; i < nvec; i++) {
        sum += vec[i].iov_len; 
    }    
    return sum;
}

#endif /* !__PT_DEFS_H__ */
