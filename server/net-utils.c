/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2009, 2017 Red Hat, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
#include <config.h>

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#endif

#include <common/log.h>

#include "net-utils.h"
#include "sys-socket.h"

/**
 * red_socket_set_keepalive:
 * @fd: a socket file descriptor
 * @keepalive: whether to enable keepalives on @fd
 *
 * Returns: #true if the operation succeeded, #false otherwise.
 */
bool red_socket_set_keepalive(int fd, bool enable, int timeout)
{
    int keepalive = !!enable;

    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) == -1) {
        if (errno != ENOTSUP) {
            g_warning("setsockopt for keepalive failed, %s", strerror(errno));
            return false;
        }
    }

    if (!enable) {
        return true;
    }

#ifdef HAVE_TCP_KEEPIDLE
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &timeout, sizeof(timeout)) == -1) {
        if (errno != ENOTSUP) {
            g_warning("setsockopt for keepalive timeout failed, %s", strerror(errno));
            return false;
        }
    }
#endif

    return true;
}

/**
 * red_socket_set_no_delay:
 * @fd: a socket file descriptor
 * @no_delay: whether to enable TCP_NODELAY on @fd
 *
 * Returns: #true if the operation succeeded, #false otherwise.
 */
bool red_socket_set_no_delay(int fd, bool no_delay)
{
    int optval = no_delay;

    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY,
                   &optval, sizeof(optval)) != 0) {
        if (errno != ENOTSUP && errno != ENOPROTOOPT) {
            spice_warning("setsockopt failed, %s", strerror(errno));
            return false;
        }
    }

    return true;
}

/**
 * red_socket_set_non_blocking:
 * @fd: a socket file descriptor
 * @non_blocking: whether to enable O_NONBLOCK on @fd
 *
 * Returns: #true if the operation succeeded, #false otherwise.
 */
bool red_socket_set_non_blocking(int fd, bool non_blocking)
{
#ifdef _WIN32
    u_long ioctl_nonblocking = 1;

    if (ioctlsocket(fd, FIONBIO, &ioctl_nonblocking) != 0) {
        spice_warning("ioctlsocket(FIONBIO) failed, %d", WSAGetLastError());
        return false;
    }
    return true;
#else
    int flags;

    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        spice_warning("fnctl(F_GETFL) failed, %s", strerror(errno));
        return false;
    }

    if (non_blocking) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if (fcntl(fd, F_SETFL, flags) == -1) {
        spice_warning("fnctl(F_SETFL) failed, %s", strerror(errno));
        return false;
    }

    return true;
#endif
}

/**
 * red_socket_get_no_delay:
 * @fd: a socket file descriptor
 *
 * Returns: The current value of TCP_NODELAY for @fd, -1 if an error occurred
 */
int red_socket_get_no_delay(int fd)
{
    int delay_val;
    socklen_t opt_size = sizeof(delay_val);

    if (getsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &delay_val,
                   &opt_size) == -1) {
            spice_warning("getsockopt failed, %s", strerror(errno));
            return -1;
    }

    return delay_val;
}
