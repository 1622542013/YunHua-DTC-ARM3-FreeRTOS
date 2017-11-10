/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2016 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    net_debug.h
 * Purpose: Network Library Debug Configuration
 * Rev.:    V7.2.0
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include "rl_net_lib.h"

#ifdef __CC_ARM
  #pragma diag_suppress 111
#else
  #pragma clang diagnostic ignored "-Wformat-nonliteral"
#endif

#ifndef DBG_SYSTEM_EN
  #error "::Network:CORE:Debug configuration update required"
#endif

#if (DBG_TIME)
  #define __DBG_TIME    net_dbg_time()
#else
  #define __DBG_TIME    ""
#endif


/* Print debug information message */
void net_dbg_info (int32_t proc, const char *fmt, ...) {
  __va_list args;

  switch (proc) {

#if (DBG_SYSTEM_EN)
    case NET_SYSTEM:        break;
  #if (DBG_MEM > 1)
    case NET_DYN_MEMORY:    break;
  #endif
  #if (DBG_ETH > 1)
    case NET_ETH_LINK:      break;
  #endif
  #if (DBG_PPP > 1)
    case NET_PPP_LINK:      break;
  #endif
  #if (DBG_SLIP > 1)
    case NET_SLIP_LINK:     break;
  #endif
  #if (DBG_LOOP > 1)
    case NET_LOOP_BACK:     break;
  #endif
#endif

#if (DBG_CORE4_EN)
  #if (DBG_IP4 > 1)
    case NET_IP4_CORE:      break;
  #endif
  #if (DBG_ICMP > 1)
    case NET_ICMP_CONTROL:  break;
  #endif
  #if (DBG_IGMP > 1)
    case NET_IGMP_HOST:     break;
  #endif
  #if (DBG_NBNS > 1)
    case NET_NBNS_CLIENT:   break;
  #endif
  #if (DBG_DHCP > 1)
    case NET_DHCP_CLIENT:   break;
  #endif
  #if (DBG_ARP > 1)
    case NET_ARP_CACHE:     break;
  #endif
#endif

#if (DBG_CORE6_EN)
  #if (DBG_IP6 > 1)
    case NET_IP6_CORE:      break;
  #endif
  #if (DBG_ICMP6 > 1)
    case NET_ICMP6_CONTROL: break;
  #endif
  #if (DBG_DHCP6 > 1)
    case NET_DHCP6_CLIENT:  break;
  #endif
  #if (DBG_NDP6 > 1)
    case NET_NDP_CACHE:     break;
  #endif
#endif

#if (DBG_SOCKET_EN)
  #if (DBG_UDP_SOCKET > 1)
    case NET_UDP_SOCKET:    break;
  #endif
  #if (DBG_TCP_SOCKET > 1)
    case NET_TCP_SOCKET:    break;
  #endif
  #if (DBG_BSD_SOCKET > 1)
    case NET_BSD_SOCKET:    break;
  #endif
#endif

#if (DBG_SERVICE_EN)
  #if (DBG_HTTP_SERVER > 1)
    case NET_HTTP_SERVER:   break;
  #endif
  #if (DBG_FTP_SERVER > 1)
    case NET_FTP_SERVER:    break;
  #endif
  #if (DBG_FTP_CLIENT > 1)
    case NET_FTP_CLIENT:    break;
  #endif
  #if (DBG_TELNET_SERVER > 1)
    case NET_TELNET_SERVER: break;
  #endif
  #if (DBG_TFTP_SERVER > 1)
    case NET_TFTP_SERVER:   break;
  #endif
  #if (DBG_TFTP_CLIENT > 1)
    case NET_TFTP_CLIENT:   break;
  #endif
  #if (DBG_SMTP_CLIENT > 1)
    case NET_SMTP_CLIENT:   break;
  #endif
  #if (DBG_DNS_CLIENT > 1)
    case NET_DNS_CLIENT:    break;
  #endif
  #if (DBG_SNMP_AGENT > 1)
    case NET_SNMP_AGENT:    break;
  #endif
  #if (DBG_SNTP_CLIENT > 1)
    case NET_SNTP_CLIENT:   break;
  #endif
#endif

    default: return;
  }
  printf ("%s%s:",__DBG_TIME,net_dbg_proc(proc));

  va_start (args,fmt);
  vprintf (fmt,args);
  va_end (args);
}

/* Pring debug error message */
void net_dbg_error (int32_t proc, const char *fmt, ...) {
  __va_list args;

  switch (proc) {

#if (DBG_SYSTEM_EN)
    case NET_SYSTEM:        break;
  #if (DBG_MEM > 0)
    case NET_DYN_MEMORY:    break;
  #endif
  #if (DBG_ETH > 0)
    case NET_ETH_LINK:      break;
  #endif
  #if (DBG_PPP > 0)
    case NET_PPP_LINK:      break;
  #endif
  #if (DBG_SLIP > 0)
    case NET_SLIP_LINK:     break;
  #endif
  #if (DBG_LOOP > 0)
    case NET_LOOP_BACK:     break;
  #endif
#endif

#if (DBG_CORE4_EN)
  #if (DBG_IP4 > 0)
    case NET_IP4_CORE:      break;
  #endif
  #if (DBG_ICMP > 0)
    case NET_ICMP_CONTROL:  break;
  #endif
  #if (DBG_IGMP > 0)
    case NET_IGMP_HOST:     break;
  #endif
  #if (DBG_NBNS > 0)
    case NET_NBNS_CLIENT:   break;
  #endif
  #if (DBG_DHCP > 0)
    case NET_DHCP_CLIENT:   break;
  #endif
  #if (DBG_ARP > 0)
    case NET_ARP_CACHE:     break;
  #endif
#endif

#if (DBG_CORE6_EN)
  #if (DBG_IP6 > 0)
    case NET_IP6_CORE:      break;
  #endif
  #if (DBG_ICMP6 > 0)
    case NET_ICMP6_CONTROL: break;
  #endif
  #if (DBG_DHCP6 > 0)
    case NET_DHCP6_CLIENT:  break;
  #endif
  #if (DBG_NDP6 > 0)
    case NET_NDP_CACHE:     break;
  #endif
#endif

#if (DBG_SOCKET_EN)
  #if (DBG_UDP_SOCKET > 0)
    case NET_UDP_SOCKET:    break;
  #endif
  #if (DBG_TCP_SOCKET > 0)
    case NET_TCP_SOCKET:    break;
  #endif
  #if (DBG_BSD_SOCKET > 0)
    case NET_BSD_SOCKET:    break;
  #endif
#endif

#if (DBG_SERVICE_EN)
  #if (DBG_HTTP_SERVER > 0)
    case NET_HTTP_SERVER:   break;
  #endif
  #if (DBG_FTP_SERVER > 0)
    case NET_FTP_SERVER:    break;
  #endif
  #if (DBG_FTP_CLIENT > 0)
    case NET_FTP_CLIENT:    break;
  #endif
  #if (DBG_TELNET_SERVER > 0)
    case NET_TELNET_SERVER: break;
  #endif
  #if (DBG_TFTP_SERVER > 0)
    case NET_TFTP_SERVER:   break;
  #endif
  #if (DBG_TFTP_CLIENT > 0)
    case NET_TFTP_CLIENT:   break;
  #endif
  #if (DBG_SMTP_CLIENT > 0)
    case NET_SMTP_CLIENT:   break;
  #endif
  #if (DBG_DNS_CLIENT > 0)
    case NET_DNS_CLIENT:    break;
  #endif
  #if (DBG_SNMP_AGENT > 0)
    case NET_SNMP_AGENT:    break;
  #endif
  #if (DBG_SNTP_CLIENT > 0)
    case NET_SNTP_CLIENT:   break;
  #endif
#endif

    default: return;
  }
  printf ("%s%s-ERR:",__DBG_TIME,net_dbg_proc(proc));

  va_start (args,fmt);
  vprintf (fmt,args);
  va_end (args);
}
