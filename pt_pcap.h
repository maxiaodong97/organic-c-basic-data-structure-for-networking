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
#ifndef __PT_PCAP_H__
#define __PT_PCAP_H__
#include <stdint.h>

#define PCAP_LITTLE_ENDIAN  0xa1b2c3d4
#define PCAP_BIG_ENDIAN     0xd4c3b2a1

typedef struct pt_pcap_hdr_s {
    uint32_t magic_number;    /* 0xa1b2c3d4 or 0xd4c3b2a1                    */
    uint16_t version_major;   /* major number                                */
    uint16_t version_minor;   /* minor number                                */
    int32_t  this_zone;       /* GMT to the local correction                 */
    uint32_t sig_figs;        /* 0                                           */
    uint32_t snap_len;        /* max 65535                                   */
    uint32_t network_type;    /* link type, must be ETHERNET                 */
} pt_pcap_file_hdr_t;

#define PCAP_LINKTYPE_ETHERNET   1

typedef struct pt_pcaprec_hdr_s {
    uint32_t ts_sec;          /* timestamp seconds                           */
    uint32_t ts_usec;         /* timestamp microseconds                      */
    uint32_t incl_len;        /* number of octets of packet saved in file    */
    uint32_t orig_len;        /* actual length of packet                     */
} pt_pcaprec_hdr_t;

#define PCAP_FILE_HDR_SIZE              (sizeof(pt_pcap_file_hdr_t))
#define PCAP_REC_HDR_SIZE               (sizeof(pt_pcaprec_hdr_t))

#define PCAP_FILE_HDR(_b_)              ((pt_pcap_file_hdr_t*)_b_)
#define PCAP_FILE_MAGIC_NUM(_b_)        (PCAP_FILE_HDR(_b_)->magic_number)
#define PCAP_FILE_SNAP_LEN(_b_)         (PCAP_FILE_HDR(_b_)->snap_len)
#define PCAP_FILE_NETWORK_TYPE(_b_)     (PCAP_FILE_HDR(_b_)->network_type)

#define PCAP_REC_HDR(_b_)               ((pt_pcaprec_hdr_t*)_b_)
#define PCAP_REC_SEC(_b_)               (PCAP_REC_HDR(_b_)->ts_sec)
#define PCAP_REC_USEC(_b_)              (PCAP_REC_HDR(_b_)->ts_usec)
#define PCAP_REC_INCL_LEN(_b_)          (PCAP_REC_HDR(_b_)->incl_len)
#define PCAP_REC_ORIG_LEN(_b_)          (PCAP_REC_HDR(_b_)->orig_len)

#endif
