#ifndef BASE_DEFINES_H
#define BASE_DEFINES_H
#include <stdint.h>
#include <rte_config.h>


#define MAX_WORKER_CORES 128

#define DC_DEFAULT_SOCKET 0

#define MAX_UINT64_VALUE ( 0xffffffffffffffffULL )
#define BAD_UINT64_VALUE ( 0xffffffffffffffffULL )
#define BAD_UINT32_VALUE ( 0xffffffff )
#define BAD_UINT16_VALUE ( 0xffff )
#define BAD_UINT8_VALUE ( (uint8_t)0xff )

#define BYTES_IN_GIGA ( 1024 * 1024 * 1024ULL )
#define MAIN_LCORE_ID 0
#define CACHE_LINE_SIZE ( 64 )

#define DC_ALWAYS_INLINE inline
#define _is_power_of_two( f ) ( ( ( f ) & ( f - 1 ) ) == 0 )
// https://en.wikipedia.org/wiki/XOR_swap_algorithm

#define LOOPBACK_IP_ADDR "127.0.0.1"



//   ---  ETHERTYPE  CONSTANTS---
#define ET_IPV4 ( 0x08 )
#define ET_ARP ( 0x0608 )

#define ET_IPV6 ( 0xdd86 )
#define ET_FLOW_CONTROL ( 0x0888 )

#define ET_VLAN ( 0x81 )
#define ET_QINQ ( 0xA888 )
#define VLAN_HDR_LEN ( 4 )
#define VLAN_MASK ( (uint16_t)0xff0f ) // TO GET VLAN IP FROM hdr

#define IP_NP_TCP ( (uint8_t)6 )
#define IP_NP_UDP ( (uint8_t)17 )






#endif // BASE_DEFINES_H
