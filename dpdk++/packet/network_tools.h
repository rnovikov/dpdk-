#ifndef NETWORK_TOOLS_H
#define NETWORK_TOOLS_H
#include <base/base_types.h>

#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <rte_tcp.h>
#define DC_TCP_HEADER_LEN(tcp)( ( tcp->data_off & 0xF0 ) >> 2 )
#endif // NETWORK_TOOLS_H
