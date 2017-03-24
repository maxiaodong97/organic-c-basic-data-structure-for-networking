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
#ifndef __PT_NET_H__
#define __PT_NET_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <features.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


#include "pt_defs.h"


/*
 * L2 definitions 
 */
#define PT_ETH_ADDR_LEN                6
#define PT_ETH_HDR_LEN                 14
#define PT_ETH_8021Q_HDR_LEN           18
#define PT_ETH_MIN_FRAME_LEN           60
#define PT_ETH_MAX_FRAME_LEN           1518
#define PT_ETH_MAX_PAYLOAD_LEN         1500
#define PT_ETH_TYPE_IP                 0x0800
#define PT_ETH_TYPE_ARP                0x0806
#define PT_ETH_TYPE_8021Q              0x8100
#define PT_ETH_TYPE_LOOPBACK           0x9000

typedef struct pt_eth_hdr_s
{
    uint8_t dst[PT_ETH_ADDR_LEN];   /* destination MAC address      */ 
    uint8_t src[PT_ETH_ADDR_LEN];   /* source MAC address           */
    uint16_t type_len;              /* PT_ETH_TYPE_xxx              */
}pt_eth_hdr_t;

typedef struct pt_eth_8021Q_hdr_s
{
    uint8_t dst[PT_ETH_ADDR_LEN];   /* destination MAC address      */
    uint8_t src[PT_ETH_ADDR_LEN];   /* source MAC address           */
    uint16_t proto_id;              /* protocol ID                  */
    uint16_t control_info;          /* control info                 */
    uint16_t type_len;              /* PT_ETH_TYPE_xxx              */
}pt_eth_8021Q_hdr_t;

#define ETH_HDR(_l2_)                  ((pt_eth_hdr_t*)(_l2_))
#define ETH_DST_ADDR(_l2_)             (ETH_HDR(_l2_)->dst) 
#define ETH_SRC_ADDR(_l2_)             (ETH_HDR(_l2_)->src) 
#define ETH_8021Q_HDR(_l2_)            ((pt_eth_8021Q_hdr_t*)(_l2_))
#define ETH_8021Q_TYPE(_l2_)           (ntohs(ETH_8021Q_HDR(_l2_)->type_len))
#define ETH_IS_IP(_l2_)                (ntohs(ETH_HDR(_l2_)->type_len) == PT_ETH_TYPE_IP)
#define ETH_IS_ARP(_l2_)               (ntohs(ETH_HDR(_l2_)->type_len) == PT_ETH_TYPE_ARP)
#define ETH_IS_8021Q(_l2_)             (ntohs(ETH_HDR(_l2_)->type_len) == PT_ETH_TYPE_8021Q)

#define ETH_SET_DST_ADDR(_l2_, _mac_)                           \
    {                                                           \
        PT_MEMCPY(ETH_DST_ADDR(_l2_), _mac_, PT_ETH_ADDR_LEN);  \
    }

#define ETH_SET_SRC_ADDR(_l2_, _mac_)                           \
    {                                                           \
        PT_MEMCPY(ETH_SRC_ADDR(_l2_), _mac_, PT_ETH_ADDR_LEN);  \
    }

#define PT_IP4_ADDR_LEN                4

typedef struct pt_arp_hdr_s{
    uint16_t hw_type;                   /* hardware type, 1 for ethernet    */ 
    uint16_t proto_type;                /* protocol type, ether type        */
    uint8_t hw_addr_len;                /* hardware address len, 6 for eth  */
    uint8_t proto_addr_len;             /* ipv4 address len is 4            */
    uint16_t opcode;                    /* 1 for request, 2 for reply       */
    uint8_t src_mac[PT_ETH_ADDR_LEN];   /* sender hardware address          */
    uint8_t src_ip[PT_IP4_ADDR_LEN];    /* sender protocol address          */
    uint8_t dst_mac[PT_ETH_ADDR_LEN];   /* target hardware address          */
    uint8_t dst_ip[PT_IP4_ADDR_LEN];    /* target protocol address          */
} pt_arp_hdr_t;

#define ARP_HDR(_l3_)               ((pt_arp_hdr_t *)(_l3_))
#define ARP_SENDER_MAC(_l3_)        (ARP_HDR(_l3_)->src_mac)
#define ARP_TARGET_MAC(_l3_)        (ARP_HDR(_l3_)->dst_mac)
#define ARP_SENDER_IP(_l3_)         (ARP_HDR(_l3_)->src_ip)
#define ARP_TARGET_IP(_l3_)         (ARP_HDR(_l3_)->dst_ip)
#define ARP_SET_REQUEST(_l3_)       { ARP_HDR(_l3_)->opcode = htons(ARPOP_REQUEST); }
#define ARP_SET_REPLY(_l3_)         { ARP_HDR(_l3_)->opcode = htons(ARPOP_REPLY); }

#define ARP_SET_SENDER_MAC(_l3_, _mac_)                             \
    {                                                               \
        PT_MEMCPY(ARP_SENDER_MAC(_l3_), _mac_, PT_ETH_ADDR_LEN);    \
    }

#define ARP_SET_TARGET_MAC(_l3_, _mac_)                             \
    {                                                               \
        PT_MEMCPY(ARP_TARGET_MAC(_l3_), _mac_, PT_ETH_ADDR_LEN);    \
    }

#define ARP_SET_SENDER_IP(_l3_, _ip_)                               \
    {                                                               \
        PT_MEMCPY(ARP_SENDER_IP(_l3_), _ip_, PT_IP4_ADDR_LEN);      \
    }

#define ARP_SET_TARGET_IP(_l3_, _ip_)                               \
    {                                                               \
        PT_MEMCPY(ARP_TARGET_IP(_l3_), _ip_, PT_IP4_ADDR_LEN);      \
    }


/*
 * L3 definitions
 */
static inline
char *
L3_FROM_L2(const char *l2)
{
    if (ETH_IS_IP(l2)) {
        return (char *)(l2 + sizeof(pt_eth_hdr_t));    
    }    

    if (ETH_IS_8021Q(l2) && ETH_8021Q_TYPE(l2) == PT_ETH_TYPE_IP) {
        return (char *)(l2 + sizeof(pt_eth_8021Q_hdr_t)); 
    }
    return NULL;
}

#define PT_IP_PROTO_ICMP            1
#define PT_IP_PROTO_TCP             6
#define PT_IP_PROTO_UDP             17
#define PT_IP_VER_4                 4
#define PT_IP_VER_6                 6
#define PT_IP_MAX_TTL               255
#define PT_IP4_HDR_LEN              20
#define PT_IP4_MIN_HDR_LEN          20
#define PT_IP4_MAX_HDR_LEN          60
#define PT_IP4_FLAG_RSVD            0x8000
#define PT_IP4_FLAG_DF              0x4000
#define PT_IP4_FLAG_MF              0x2000
#define PT_IP4_FRAGOFF_MASK         0x1fff
#define PT_IP6_ADDR_LEN             16

#define IP_PROTO_2_STRING(_proto_)  \
    ( \
    _proto_ == PT_IP_PROTO_TCP   ? "TCP"     : \
    _proto_ == PT_IP_PROTO_UDP   ? "UDP"     : \
    _proto_ == PT_IP_PROTO_ICMP  ? "ICMP"    : \
    "")


typedef struct pt_ip4_hdr_s
{
    uint8_t   verh_len;     /* 4-7 version, 0-3  hdr len in word    */
    uint8_t   tos;          /* type of services                     */
                            /* 0-2 precedence,                      */
                            /* 3 low delay,                         */   
                            /* 4 high throughput                    */
                            /* 5 high reliability                   */
    uint16_t  total_len;    /* total length of the datagram         */ 
    uint16_t  id;           /* ip id                                */
    uint16_t  frag_ofs;     /* 0 must be 0, 1 DF, 2 MF, 3-15 offset */
    uint8_t   ttl;          /* time to live, 0 must be destroy      */
    uint8_t   proto;        /* l4 proto                             */
    uint16_t  checksum;     /* header checksum                      */
    uint32_t  src_addr;     /* source ip address                    */
    uint32_t  dst_addr;     /* destination ip address               */
}pt_ip4_hdr_t;

typedef struct pt_ip6_hdr_s
{
    uint32_t  verh         :4;          /* version                  */ 
    uint32_t  traffic_class:8;          /* traffic class            */  
    uint32_t  flow_label   :20;         /* flow label               */
    uint16_t  total_len;                /* total length of datagram */
    uint8_t   next_hdr;                 /* next header              */
    uint8_t   hop_limit;                /* time to live             */
    uint32_t  src_addr[4];              /* source ip address        */
    uint32_t  dst_addr[4];              /* destination ip address   */
}pt_ip6_hdr_t;

#define IP4_HDR(_l3_)               ((pt_ip4_hdr_t*)(_l3_))
#define IP4_IS_TCP(_l3_)            (IP4_HDR(_l3_)->protocol == PT_IP_PROTO_TCP)
#define IP4_IS_UDP(_l3_)            (IP4_HDR(_l3_)->protocol == PT_IP_PROTO_UDP)
#define IP4_IS_ICMP(_l3_)           (IP4_HDR(_l3_)->protocol == PT_IP_PROTO_ICMP)
#define IP4_VERSION(_l3_)           (IP4_HDR(_l3_)->verh_len >> 4)
#define IP4_HDR_LEN(_l3_)           ((IP4_HDR(_l3_)->verh_len & 0x0f) << 2)
#define IP4_TOS(_l3_)               (IP4_HDR(_l3_)->tos)
#define IP4_TOTAL_LEN(_l3_)         (ntohs(IP4_HDR(_l3_)->total_len))
#define IP4_ID(_l3_)                (ntohs(IP4_HDR(_l3_)->id))
#define IP4_FRAG_OFS(_l3_)          (ntohs(IP4_HDR(_l3_)->frag_ofs))
#define IP4_TTL(_l3_)               (IP4_HDR(_l3_)->ttl)
#define IP4_PROTOCOL(_l3_)          (IP4_HDR(_l3_)->proto)
#define IP4_CHECKSUM(_l3_)          (ntohs(IP4_HDR(_l3_)->checksum))
#define IP4_SRC_ADDR(_l3_)          (IP4_HDR(_l3_)->src_addr)
#define IP4_DST_ADDR(_l3_)          (IP4_HDR(_l3_)->dst_addr)

#define IP4_SET_HDR_LEN(_l3_, _len_)                                            \
    {                                                                           \
        IP4_HDR(_l3_)->verh_len = (PT_IP_VER_4 << 4) | ((_len_ >> 2) & 0x0f);   \
    }
#define IP4_SET_TOS(_l3_, _tos_)    {IP4_TOS(_l3_) = _tos_; }  
#define IP4_SET_TOTAL_LEN(_l3_, _len_)                                          \
    {                                                                           \
        IP4_HDR(_l3_)->total_len = htons(_len_);                                \
    }
#define IP4_SET_ID(_l3_, _id_)      {IP4_HDR(_l3_)->id = htons(_id_); }
#define IP4_SET_NO_FRAG(_l3_)       {IP4_HDR(_l3_)->frag_ofs = htons(0x4000);}
#define IP4_SET_FRAG_OFF(_l3_, _ofs_)                                           \
    {                                                                           \
        IP4_HDR(_l3_)->frag_ofs = htons((_ofs_ << 2) | 0x2000);                 \
    }
#define IP4_SET_TTL(_l3_, _ttl_)    {IP4_TTL(_l3_) = _ttl_; }
#define IP4_SET_PROTO(_l3_, _p_)    {IP4_HDR(_l3_)->proto = _p_; }
#define IP4_SET_CHECKSUM(_l3_, _s_) {IP4_HDR(_l3_)->checksum = _s_; }
#define IP4_SET_SRC_ADDR(_l3_, _s_) {IP4_SRC_ADDR(_l3_) = _s_; }
#define IP4_SET_DST_ADDR(_l3_, _s_) {IP4_DST_ADDR(_l3_) = _s_; }

#define IP_VER_4(_l3_)              (IP4_VERSION(_l3_) == PT_IP_VER_4)
#define IP_VER_6(_l3_)              (IP4_VERSION(_l3_) == PT_IP_VER_6)

#define IP6_HDR(_l3_)               ((pt_ip6_hdr_t*)(_l3_))
#define IP6_HDR_LEN(_l3_)           (sizeof(pt_ip6_hdr_t))
#define IP6_VERH(_l3_)              (IP6_HDR(_l3_)->verh)
#define IP6_TRAFFIC_CLASS(_l3_)     (IP6_HDR(_l3_)->traffic_class)
#define IP6_FLOW_LABEL(_l3_)        (IP6_HDR(_l3_)->flow_label)
#define IP6_TOTAL_LEN(_l3_)         (ntohs(IP6_HDR(_l3_)->total_len))
#define IP6_NEXT_HDR(_l3_)          (IP6_HDR(_l3_)->next_hdr)
#define IP6_HOP_LIMIT(_l3_)         (IP6_HDR(_l3_)->hop_limit)
#define IP6_SRC_ADDR(_l3_)          (IP6_HDR(_l3_)->src_addr)
#define IP6_DST_ADDR(_l3_)          (IP6_HDR(_l3_)->dst_addr)

/* IPv4 address 1.2.3.4 */
#define IP4_ADDR_1(_ip_)            ((_ip_ >> 24) & 0xFF)
#define IP4_ADDR_2(_ip_)            ((_ip_ >> 16) & 0xFF)
#define IP4_ADDR_3(_ip_)            ((_ip_ >> 8 ) & 0xFF)
#define IP4_ADDR_4(_ip_)            ((_ip_ >> 0 ) & 0xFF)

/* IPv6 address 1:2:3:4::5:6:7:8 */
#define IP6_ADDR_1(_ip_)            ((_ip_[3] >> 16) & 0xFFFF)
#define IP6_ADDR_2(_ip_)            ((_ip_[3] >> 0 ) & 0xFFFF)
#define IP6_ADDR_3(_ip_)            ((_ip_[2] >> 16) & 0xFFFF)
#define IP6_ADDR_4(_ip_)            ((_ip_[2] >> 0 ) & 0xFFFF)
#define IP6_ADDR_5(_ip_)            ((_ip_[1] >> 16) & 0xFFFF)
#define IP6_ADDR_6(_ip_)            ((_ip_[1] >> 0 ) & 0xFFFF)
#define IP6_ADDR_7(_ip_)            ((_ip_[0] >> 16) & 0xFFFF)
#define IP6_ADDR_8(_ip_)            ((_ip_[0] >> 0 ) & 0xFFFF)

static inline 
uint8_t
IP6_PROTOCOL(const char *l3)
{
    return IP6_NEXT_HDR(l3);    
}

static inline 
void
dump_ip_hdr(const char *l3, FILE *fp)
{
    if (IP_VER_4(l3)) {
        PT_FPRINTF(fp, "ver = %d, hdr_len = %d, tos = %d, total_len = %d, "
            "id = %d, frag_ofs = 0x%x, ttl = %d, proto = %d, "
            "checksum = 0x%x, src_addr = %d.%d.%d.%d, dst_addr = "
            "%d.%d.%d.%d\n", 
            IP4_VERSION(l3), IP4_HDR_LEN(l3), IP4_TOS(l3), IP4_TOTAL_LEN(l3), 
            IP4_ID(l3), IP4_FRAG_OFS(l3), IP4_TTL(l3), IP4_PROTOCOL(l3), 
            IP4_CHECKSUM(l3), 
            IP4_ADDR_4(IP4_SRC_ADDR(l3)), IP4_ADDR_3(IP4_SRC_ADDR(l3)),
            IP4_ADDR_2(IP4_SRC_ADDR(l3)), IP4_ADDR_1(IP4_SRC_ADDR(l3)), 
            IP4_ADDR_4(IP4_DST_ADDR(l3)), IP4_ADDR_3(IP4_DST_ADDR(l3)),
            IP4_ADDR_2(IP4_DST_ADDR(l3)), IP4_ADDR_1(IP4_DST_ADDR(l3))); 
        return;
    } 

    if (IP_VER_6(l3)) {
        PT_FPRINTF(fp, "ver = %d, traffic_class = %d, flow_label = %d, "
            "total_len = %d, next_hdr = %d, hop_limit = %d, "
            "src_addr = %x:%x:%x:%x::%x:%x:%x:%x, dst_addr = "
            "%x:%x:%x:%x::%x:%x:%x:%x\n", 
            IP4_VERSION(l3), IP6_TRAFFIC_CLASS(l3), IP6_FLOW_LABEL(l3), 
            IP6_TOTAL_LEN(l3), IP6_NEXT_HDR(l3), IP6_HOP_LIMIT(l3), 
            IP6_ADDR_8(IP6_SRC_ADDR(l3)), IP6_ADDR_7(IP6_SRC_ADDR(l3)),
            IP6_ADDR_6(IP6_SRC_ADDR(l3)), IP6_ADDR_5(IP6_SRC_ADDR(l3)), 
            IP6_ADDR_4(IP6_SRC_ADDR(l3)), IP6_ADDR_3(IP6_SRC_ADDR(l3)),
            IP6_ADDR_2(IP6_SRC_ADDR(l3)), IP6_ADDR_1(IP6_SRC_ADDR(l3)), 
            IP6_ADDR_8(IP6_DST_ADDR(l3)), IP6_ADDR_7(IP6_DST_ADDR(l3)),
            IP6_ADDR_6(IP6_DST_ADDR(l3)), IP6_ADDR_5(IP6_DST_ADDR(l3)), 
            IP6_ADDR_4(IP6_DST_ADDR(l3)), IP6_ADDR_3(IP6_DST_ADDR(l3)),
            IP6_ADDR_2(IP6_DST_ADDR(l3)), IP6_ADDR_1(IP6_DST_ADDR(l3))); 
        return;
    }

    PT_FPRINTF(fp, "Non V4 or V6 IP, %d, l3 = %p\n", 
            IP4_VERSION(l3), l3);
}

/*
 * L4 definitions
 */
static inline 
char *
L4_FROM_L3(const char *l3)
{
    if (IP_VER_4(l3)) {
        if (IP4_PROTOCOL(l3) != PT_IP_PROTO_TCP &&
            IP4_PROTOCOL(l3) != PT_IP_PROTO_UDP) {
            return NULL;    
        }    
        return (char *)(l3 + IP4_HDR_LEN(l3));
    }

    if (IP_VER_6(l3)) {
        /* TODO: Need to support extended header later */
        if (IP6_NEXT_HDR(l3) != PT_IP_PROTO_TCP && 
            IP6_NEXT_HDR(l3) != PT_IP_PROTO_UDP) {
            return NULL;    
        } 
        return (char*)(l3 + IP6_HDR_LEN(l3));
    }
    return NULL;
}

#define PT_UDP_HDR_LEN              8
#define PT_TCP_HDR_LEN              20
#define PT_TCP_MIN_HDR_LEN          20
#define PT_TCP_MAX_HDR_LEN          60
#define PT_TCP_FLAG_FIN             0x01
#define PT_TCP_FLAG_SYN             0x02
#define PT_TCP_FLAG_RST             0x04
#define PT_TCP_FLAG_PSH             0x08
#define PT_TCP_FLAG_ACK             0x10
#define PT_TCP_FLAG_URG             0x20

#define PT_TCP_OPTION_EOO           0   /* End of Option List [RFC793]       */
#define PT_TCP_OPTION_NOOP          1   /* No-Operation [RFC793]             */
#define PT_TCP_OPTION_MSS           2   /* Maximum Segment Size [RFC793]     */
#define PT_TCP_OPTION_MSS_LEN       4   /* MSS option length                 */
#define PT_TCP_OPTION_WS            3   /* Window scale option [RFC1323]     */
#define PT_TCP_OPTION_WS_LEN        3   /* WS option length                  */
#define PT_TCP_OPTION_WS_MAX        14  /* Must be less or equal to this     */
#define PT_TCP_OPTION_SACK_PERM     4   /* SACK permitted option [RFC2018]   */
#define PT_TCP_OPTION_SACK_PERM_LEN 2   /* SACK permitted option length      */
#define PT_TCP_OPTION_SACK          5   /* SACK option vary length           */
#define PT_TCP_OPTION_TSOPT         8   /* Time stamp option [RFC1323]       */
#define PT_TCP_OPTION_TSOPT_LEN     10  /* TSOPT option length               */
#define PT_TCP_OPTION_SACK_MAX_BLKS 3   /* Maximum number of sack blocks     */

#define PT_TCP_MAX_WINDOW_SIZE   65535  /* 16 bit window without WS option   */

typedef struct pt_udp_hdr_s
{
    uint16_t  src_port;     /* udp source port      */
    uint16_t  dst_port;     /* udp destination port */
    uint16_t  len;          /* header + payload     */
    uint16_t  checksum;     /* header + payload     */
}pt_udp_hdr_t;

typedef struct pt_tcp_hdr_s
{
    uint16_t  src_port;     /* TCP source port              */
    uint16_t  dst_port;     /* TCP destination port         */
    uint32_t  seq;          /* TCP sequence number          */
    uint32_t  ack;          /* TCP acknowledge number       */
    uint8_t   data_ofs;     /* TCP payload offset in words  */
    uint8_t   flags;        /* TCP flags                    */
    uint16_t  window;       /* TCP window                   */
    uint16_t  checksum;     /* TCP checksum: pseudo_hdr +   */
                            /*      tcp_hdr + data          */
    uint16_t  urg_ptr;      /* TCP urg pointer              */
}pt_tcp_hdr_t;

typedef struct pt_pseudo_hdr_s
{
    uint32_t src_ip;        /* source IP address            */
    uint32_t dst_ip;        /* destination IP address       */
    uint8_t reserved;       /* reserved field 0             */
    uint8_t proto;          /* protocol  ip->proto          */
    uint16_t total_len;     /* L4 header + payload len      */
}pt_pseudo_hdr_t;

typedef struct pt_pseudo_v6_hdr_s
{
    uint32_t src_ip[4];     /* source IP address            */
    uint32_t dst_ip[4];     /* destination IP address       */
    uint32_t total_len;     /* upper layer packet length    */
    uint8_t reserve[3];     /* must be 0                    */
    uint16_t next_hdr;      /* next header field            */
}pt_pseudo_v6_hdr_t;

#define TCP_HDR(_l4_)               ((pt_tcp_hdr_t*)_l4_)
#define TCP_SRC_PORT(_l4_)          ntohs(TCP_HDR(_l4_)->src_port)
#define TCP_DST_PORT(_l4_)          ntohs(TCP_HDR(_l4_)->dst_port)
#define TCP_HDR_LEN(_l4_)           ((TCP_HDR(_l4_)->data_ofs >> 4) << 2)
#define TCP_WINDOW(_l4_)            ntohs(TCP_HDR(_l4_)->window)
#define TCP_SEQ(_l4_)               ntohl(TCP_HDR(_l4_)->seq)
#define TCP_ACK(_l4_)               ntohl(TCP_HDR(_l4_)->ack)
#define TCP_CHECKSUM(_l4_)          ntohs(TCP_HDR(_l4_)->checksum)
#define TCP_URG_PTR(_l4_)           ntohs(TCP_HDR(_l4_)->urg_ptr)
#define TCP_FLAGS(_l4_)             (TCP_HDR(_l4_)->flags)
#define TCP_FLAGS_IS_ACK(_l4_)      ((TCP_FLAGS(_l4_) & PT_TCP_FLAG_ACK) != 0)
#define TCP_FLAGS_IS_FIN(_l4_)      ((TCP_FLAGS(_l4_) & PT_TCP_FLAG_FIN) != 0)
#define TCP_FLAGS_IS_SYN(_l4_)      ((TCP_FLAGS(_l4_) & PT_TCP_FLAG_SYN) != 0)
#define TCP_FLAGS_IS_RST(_l4_)      ((TCP_FLAGS(_l4_) & PT_TCP_FLAG_RST) != 0)
#define TCP_FLAGS_IS_PSH(_l4_)      ((TCP_FLAGS(_l4_) & PT_TCP_FLAG_PSH) != 0)
#define TCP_OPTION(_l4_)            ((uint8_t*)(TCP_HDR(_l4_) + 1))
#define TCP_OPTION_LEN(_l4_)        (TCP_HDR_LEN(_l4_) - sizeof(pt_tcp_hdr_t))
#define TCP_OPTION_LEN_MAX          40
#define TCP_DATA(_l4_)              ((char*)TCP_HDR(_l4_) + TCP_HDR_LEN(_l4_))

#define TCP_SET_SPORT(_l4_, _s_)    { TCP_HDR(_l4_)->src_port = htons(_s_); }
#define TCP_SET_DPORT(_l4_, _d_)    { TCP_HDR(_l4_)->dst_port = htons(_d_); }
#define TCP_SET_HDR_LEN(_l4_, _l_)  { TCP_HDR(_l4_)->data_ofs = (_l_ >> 2) << 4; } 
#define TCP_SET_WIN(_l4_, _w_)      { TCP_HDR(_l4_)->window = htons(_w_); }
#define TCP_SET_SEQ(_l4_, _s_)      { TCP_HDR(_l4_)->seq = htonl(_s_); }
#define TCP_SET_ACK(_l4_, _a_)      { TCP_HDR(_l4_)->ack = htonl(_a_); }
#define TCP_SET_CHECKSUM(_l4_, _c_) { TCP_HDR(_l4_)->checksum = _c_; }
#define TCP_SET_FLAGS(_l4_, _f_)    { TCP_FLAGS(_l4_) = (_f_); }
#define TCP_SET_FLAG_FIN(_l4_)      { TCP_FLAGS(_l4_) |= PT_TCP_FLAG_FIN; } 
#define TCP_SET_FLAG_SYN(_l4_)      { TCP_FLAGS(_l4_) |= PT_TCP_FLAG_SYN; } 
#define TCP_SET_FLAG_RST(_l4_)      { TCP_FLAGS(_l4_) |= PT_TCP_FLAG_RST; } 
#define TCP_SET_FLAG_PSH(_l4_)      { TCP_FLAGS(_l4_) |= PT_TCP_FLAG_PSH; } 
#define TCP_SET_FLAG_ACK(_l4_)      { TCP_FLAGS(_l4_) |= PT_TCP_FLAG_ACK; } 
#define TCP_CLR_FLAGS(_l4_)         { TCP_FLAGS(_l4_) = 0; }
#define TCP_CLR_FIN(_l4_)           { TCP_FLAGS(_l4_) &= ~PT_TCP_FLAG_FIN; }
#define TCP_CLR_SYN(_l4_)           { TCP_FLAGS(_l4_) &= ~PT_TCP_FLAG_SYN; }
#define TCP_CLR_RST(_l4_)           { TCP_FLAGS(_l4_) &= ~PT_TCP_FLAG_RST; }
#define TCP_CLR_PSH(_l4_)           { TCP_FLAGS(_l4_) &= ~PT_TCP_FLAG_PSH; }
#define TCP_CLR_ACK(_l4_)           { TCP_FLAGS(_l4_) &= ~PT_TCP_FLAG_ACK; }

#define TCP_SEQ_EQU(_s1_, _s2_)     ((_s1_) - (_s2_) == 0)
#define TCP_SEQ_LE(_s1_, _s2_)      ((int)((_s1_) - (_s2_)) <= 0)
#define TCP_SEQ_GE(_s1_, _s2_)      ((int)((_s1_) - (_s2_)) >= 0)
#define TCP_SEQ_LT(_s1_, _s2_)      ((int)((_s1_) - (_s2_)) < 0)
#define TCP_SEQ_GT(_s1_, _s2_)      ((int)((_s1_) - (_s2_)) > 0)

#define UDP_HDR(_l4_)               ((pt_udp_hdr_t*)_l4_)
#define UDP_SRC_PORT(_l4_)          (ntohs(UDP_HDR(_l4_)->src_port))
#define UDP_DST_PORT(_l4_)          (ntohs(UDP_HDR(_l4_)->dst_port))
#define UDP_DATA_LEN(_l4_)          (ntohs(UDP_HDR(_l4_)->len) - sizeof(pt_udp_hdr_t))
#define UDP_CHECKSUM(_l4_)          (ntohs(UDP_HDR(_l4_)->checksum))
#define UDP_DATA(_l4_)              ((char*)UDP_HDR(_l4_) + sizeof(pt_udp_hdr_t))

#define UDP_SET_SPORT(_l4_, _s_)    { UDP_HDR(_l4_)->src_port = htons(_s_); }
#define UDP_SET_DPORT(_l4_, _d_)    { UDP_HDR(_l4_)->dst_port = htons(_d_); }
#define UDP_SET_LEN(_l4_, _l_)      { UDP_HDR(_l4_)->len = htons(_l_); } 
#define UDP_SET_CHECKSUM(_l4_, _c_) { UDP_HDR(_l4_)->checksum = _c_; }

static inline
uint16_t
TCP_DATA_LEN(const char *l3, const char *l4) 
{
    if (IP_VER_4(l3)) {
        if (IP4_PROTOCOL(l3) != PT_IP_PROTO_TCP) {
            return 0;    
        } 
        return IP4_TOTAL_LEN(l3) - IP4_HDR_LEN(l3) - TCP_HDR_LEN(l4);
    }

    if (IP_VER_6(l3)) {
        if (IP6_PROTOCOL(l3) != PT_IP_PROTO_TCP) {
            return 0;    
        } 
        return IP6_TOTAL_LEN(l3) - IP6_HDR_LEN(l3) - TCP_HDR_LEN(l4);
    }
    return 0;
}

static inline
void
dump_tcp_hdr(const char *l4, FILE *fp)
{
    PT_FPRINTF(fp, "src_port = %d, dst_port = %d, seq = %u, ack = %u, "
        "hdr_len = %d, flags = %s%s%s%s%s window = %d, "
        "checksum = 0x%x, urg = 0x%x, ", 
        TCP_SRC_PORT(l4), TCP_DST_PORT(l4), TCP_SEQ(l4), TCP_ACK(l4), 
        TCP_HDR_LEN(l4), TCP_FLAGS_IS_ACK(l4) ? "ACK," : "", 
        TCP_FLAGS_IS_FIN(l4) ? "FIN," : "", 
        TCP_FLAGS_IS_SYN(l4) ? "SYN," : "", 
        TCP_FLAGS_IS_PSH(l4) ? "PSH," : "", 
        TCP_FLAGS_IS_RST(l4) ? "RST," : "", 
        TCP_WINDOW(l4), TCP_CHECKSUM(l4), TCP_URG_PTR(l4));
}

typedef struct pt_icmp_hdr_s
{
    uint8_t icmp_type;      /* ICMP type, 0, reply, 3 non-reach, 8 request, */
    uint8_t icmp_code;      /* sub code of type                             */
    uint16_t checksum;      /* ICMP packet checksum                         */
    uint16_t quench_low;    /* ID for ping                                  */
    uint16_t quench_high;   /* sequence for ping or nextmtu for frag        */
} pt_icmp_hdr_t;

#define ICMP_HDR(_l4_)                  ((pt_icmp_hdr_t*)(_l4_))
#define ICMP_TYPE(_l4_)                 (ICMP_HDR(_l4_)->icmp_type)
#define ICMP_CODE(_l4_)                 (ICMP_HDR(_l4_)->icmp_code)
#define ICMP_CHECKSUM(_l4_)             (ICMP_HDR(_l4_)->checksum)
#define ICMP_QUENCH_LOW(_l4_)           (ntohs(ICMP_HDR(_l4_)->quench_low))
#define ICMP_QUENCH_HIGH(_l4_)          (ntohs(ICMP_HDR(_l4_)->quench_high))
#define ICMP_SET_TYPE(_l4_,_t_)         {ICMP_HDR(_l4_)->icmp_type = _t_;}
#define ICMP_SET_CODE(_l4_, _c_)        {ICMP_HDR(_l4_)->icmp_code = _c_;}
#define ICMP_SET_CHECKSUM(_l4_, _c_)    {ICMP_HDR(_l4_)->checksum = _c_; }
#define ICMP_SET_QUENCH_LOW(_l4_, _l_)  {ICMP_HDR(_l4_)->quench_low = htons(_l_);}
#define ICMP_SET_QUENCH_HIGH(_l4_, _h_) {ICMP_HDR(_l4_)->quench_high = htons(_h_);}

static inline
uint16_t 
COMPUTE_CHECKSUM(uint8_t *data, uint32_t len)
{
    uint32_t sum = 0;  
    uint16_t *temp = (uint16_t *)data;
    while(len > 1){
        sum += *temp++;
        if(sum & 0x80000000)   
            sum = (sum & 0xFFFF) + (sum >> 16);
        len -= 2;
    }
    if(len) {
        sum += (uint16_t) *((uint8_t *)temp);
    }
    while(sum>>16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return ~sum;
}

static inline
uint16_t 
COMPUTE_CHECKSUM_IOVEC(struct iovec *vec, uint32_t nvec)
{
    uint32_t sum = 0;  
    uint32_t i = 0;
    for (i = 0; i < nvec; i++) {
        uint16_t *temp = (uint16_t *)vec[i].iov_base;
        uint32_t len = vec[i].iov_len;
        while(len > 1) {
            sum += *temp++;
            if(sum & 0x80000000)   
                sum = (sum & 0xFFFF) + (sum >> 16);
            len -= 2;
        }
        if(len) {
            sum += (uint16_t) *((uint8_t *)temp);
        }
    }

    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return ~sum;
}

#define PT_FLOW_KEY4_SIZE       sizeof(pt_flow_key4_t)   /* 16 bytes */
#define PT_FLOW_KEY6_SIZE       sizeof(pt_flow_key6_t)   /* 40 bytes */
#define PT_IP4_PRINT_FMT        "%d.%d.%d.%d" 
#define PT_IP4_PRINT(_ip_)      IP4_ADDR_4(_ip_), IP4_ADDR_3(_ip_), \
                                IP4_ADDR_2(_ip_), IP4_ADDR_1(_ip_) 

#define PT_IP6_PRINT_FMT        "%x:%x:%x:%x::%x:%x:%x:%x"
#define PT_IP6_PRINT(_ip_)      IP6_ADDR_8(_ip_), IP6_ADDR_7(_ip_), \
                                IP6_ADDR_6(_ip_), IP6_ADDR_5(_ip_), \
                                IP6_ADDR_4(_ip_), IP6_ADDR_3(_ip_), \
                                IP6_ADDR_2(_ip_), IP6_ADDR_1(_ip_)
 
/*
 * ipv4 flow key, must be 4 bytes aligned 
 * for non-TCP/UDP/ICMP protocol key, src_port and dst_port are zero, 
 * proto is the IP protocol number. 
 */
typedef struct pt_flow_key_s 
{
    uint32_t src_ip;        /* source IP        */
    uint32_t dst_ip;        /* destination IP   */
    uint16_t src_port;      /* source port      */
    uint16_t dst_port;      /* destination port */
    uint8_t proto;          /* IP protocol      */
    uint8_t pad[3];         /* Must be set to 0 */
} pt_flow_key4_t;

/*
 * ipv6 flow key, must be 4 bytes aligned 
 * for non-TCP/UDP/ICMP protocol key, src_port and dst_port are zero, 
 * proto is the IP protocol number. 
 */
typedef struct pt_flow_key6_s 
{
    uint32_t src_ip[4];     /* source IP        */
    uint32_t dst_ip[4];     /* destination IP   */
    uint16_t src_port;      /* source port      */
    uint16_t dst_port;      /* destination port */
    uint8_t proto;          /* IP protocol      */
    uint8_t pad[3];         /* Must be set to 0 */
} pt_flow_key6_t;

/*
 * Construct the IPv4 or IPv6 key from l3
 * 
 * For non-TCP/UDP/ICMP traffic, the src_port and dst port are 0
 * For ICMP traffic, src_port is ICMP_TYPE, dst_port is ICMP_CODE
 */
static inline int 
get_flow_key(const char *l3, void *key, uint16_t *keysize, bool_t b_reverse)
{
    char *l4 = L4_FROM_L3(l3); /* For non-TCP/UDP, this will return NULL */
    uint8_t proto = 0;
    uint16_t src_port = 0;
    uint16_t dst_port = 0;
    pt_flow_key4_t *key4 = (pt_flow_key4_t *)key;
    pt_flow_key6_t *key6 = (pt_flow_key6_t *)key;

    proto = IP_VER_4(l3) ? IP4_PROTOCOL(l3) : 
        IP_VER_6(l3) ? IP6_PROTOCOL(l3) : 0;

    if (proto == 0 || l4 == NULL) {
        return -1;    
    }

    switch(proto) {
    case PT_IP_PROTO_TCP: 
        src_port = TCP_SRC_PORT(l4);
        dst_port = TCP_DST_PORT(l4);
        break;
    case PT_IP_PROTO_UDP:
        src_port = UDP_SRC_PORT(l4);
        dst_port = UDP_DST_PORT(l4);
        break;
    case PT_IP_PROTO_ICMP:
        break;  
    default: 
        return -1;
    }

    if (IP_VER_4(l3)) {
        *keysize = sizeof(pt_flow_key4_t);    
        key4->proto = proto;
        key4->pad[0] = key4->pad[1] = key4->pad[2] = 0;
        key4->src_ip = b_reverse ? IP4_DST_ADDR(l3) : IP4_SRC_ADDR(l3);    
        key4->dst_ip = b_reverse ? IP4_SRC_ADDR(l3) : IP4_DST_ADDR(l3);
        key4->src_port = b_reverse ? dst_port : src_port;
        key4->dst_port = b_reverse ? src_port : dst_port;
    }
    else {
        /* Must be IPv6 */
        int i = 0;
        *keysize = sizeof(pt_flow_key6_t);    
        key6->proto = proto; 
        key6->pad[0] = key6->pad[1] = key6->pad[2] = 0;
        for (i = 0; i < 3; i++) {
            key6->src_ip[i] = b_reverse? IP6_DST_ADDR(l3)[i] : 
                    IP6_SRC_ADDR(l3)[i];   
            key6->dst_ip[i] = b_reverse? IP6_SRC_ADDR(l3)[i] :
                    IP6_DST_ADDR(l3)[i]; 
        } 
        key6->src_port = b_reverse ? dst_port : src_port;
        key6->dst_port = b_reverse ? src_port : dst_port;
    }
    return 0;
}

/*
 * Dump a flow key to a file (in a single line)
 * "src_ip [src_port] --> dst_ip [dst_port] ip_protocol"
 */
static inline void
dump_flow_key(const void *key, const uint16_t keysize, FILE *fp)
{
    pt_flow_key4_t *key4 = (pt_flow_key4_t *)key;
    pt_flow_key6_t *key6 = (pt_flow_key6_t *)key;

    if (fp == NULL) {
        return;    
    }

    if (keysize == PT_FLOW_KEY4_SIZE) {
        PT_FPRINTF(fp, PT_IP4_PRINT_FMT"[%d] --> "PT_IP4_PRINT_FMT"[%d] %s\n", 
            PT_IP4_PRINT(key4->src_ip), 
            key4->src_port,
            PT_IP4_PRINT(key4->dst_ip), 
            key4->dst_port, 
            IP_PROTO_2_STRING(key4->proto));
    } 
    else {
         PT_FPRINTF(fp, PT_IP6_PRINT_FMT"[%d] --> "PT_IP6_PRINT_FMT"[%d] %s\n", 
            PT_IP6_PRINT(key6->src_ip), 
            key6->src_port,
            PT_IP6_PRINT(key6->dst_ip), 
            key6->dst_port, 
            IP_PROTO_2_STRING(key6->proto));
    }
}

#endif /* !__PT_NET_H__ */
