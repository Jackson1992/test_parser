/*
    Copyright (c) 2009-2012 250bpm s.r.o.
    Copyright (c) 2007-2009 iMatix Corporation
    Copyright (c) 2011 VMware, Inc.
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of Crossroads I/O project.

    Crossroads I/O is free software; you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Crossroads is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <limits>

#include "../include/xs/xs.h"

#include "options.hpp"
#include "err.hpp"

xs::options_t::options_t () :
    sndhwm (1000),
    rcvhwm (1000),
    affinity (0),
    identity_size (0),
    rate (100),
    recovery_ivl (10000),
    multicast_hops (1),
    sndbuf (0),
    rcvbuf (0),
    type (-1),
    linger (-1),
    reconnect_ivl (100),
    reconnect_ivl_max (0),
    backlog (100),
    maxmsgsize (std::numeric_limits <uint64_t>::max ()),
    rcvtimeo (-1),
    sndtimeo (-1),
    ipv4only (1),
    keepalive (0),
    protocol (0),
    filter (XS_FILTER_PREFIX),
    survey_timeout (-1),
    delay_on_close (true),
    delay_on_disconnect (true),
    send_identity (false),
    recv_identity (false),
    socket_id (0)
{
}

int xs::options_t::setsockopt (int option_, const void *optval_,
    size_t optvallen_)
{
    switch (option_) {

    case XS_SNDHWM:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        sndhwm = *((int*) optval_);
        return 0;

    case XS_RCVHWM:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        rcvhwm = *((int*) optval_);
        return 0;

    case XS_AFFINITY:
        if (optvallen_ != sizeof (uint64_t)) {
            errno = EINVAL;
            return -1;
        }
        affinity = *((uint64_t*) optval_);
        return 0;

    case XS_IDENTITY:

        //  Empty identity is invalid as well as identity longer than
        //  255 bytes. Identity starting with binary zero is invalid
        //  as these are used for auto-generated identities.
        if (optvallen_ < 1 || optvallen_ > 255 ||
              *((const unsigned char*) optval_) == 0) {
            errno = EINVAL;
            return -1;
        }
        identity_size = optvallen_;
        memcpy (identity, optval_, identity_size);
        return 0;

    case XS_RATE:
        if (optvallen_ != sizeof (int) || *((int*) optval_) <= 0) {
            errno = EINVAL;
            return -1;
        }
        rate = *((int*) optval_);
        return 0;
        
    case XS_RECOVERY_IVL:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        recovery_ivl = *((int*) optval_);
        return 0;

    case XS_SNDBUF:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        sndbuf = *((int*) optval_);
        return 0;

    case XS_RCVBUF:
        if (optvallen_ != sizeof (int) || *((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        rcvbuf = *((int*) optval_);
        return 0;

    case XS_LINGER:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        linger = *((int*) optval_);
        return 0;

    case XS_RECONNECT_IVL:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        if (*((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        reconnect_ivl = *((int*) optval_);
        return 0;

    case XS_RECONNECT_IVL_MAX:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        if (*((int*) optval_) < 0) {
            errno = EINVAL;
            return -1;
        }
        reconnect_ivl_max = *((int*) optval_);
        return 0;

    case XS_BACKLOG:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        backlog = *((int*) optval_);
        return 0;

    case XS_MAXMSGSIZE:
        if (optvallen_ != sizeof (uint64_t)) {
            errno = EINVAL;
            return -1;
        }
        maxmsgsize = *((uint64_t*) optval_);
        return 0;

    case XS_MULTICAST_HOPS:
        if (optvallen_ != sizeof (int) || *((int*) optval_) <= 0) {
            errno = EINVAL;
            return -1;
        }
        multicast_hops = *((int*) optval_);
        return 0;

    case XS_RCVTIMEO:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        rcvtimeo = *((int*) optval_);
        return 0;

    case XS_SNDTIMEO:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        sndtimeo = *((int*) optval_);
        return 0;

    case XS_IPV4ONLY:
        {
            if (optvallen_ != sizeof (int)) {
                errno = EINVAL;
                return -1;
            }
            int val = *((int*) optval_);
            if (val != 0 && val != 1) {
                errno = EINVAL;
                return -1;
            }
            ipv4only = val;
            return 0;
        }

    case XS_KEEPALIVE:
        {
            if (optvallen_ != sizeof (int)) {
                errno = EINVAL;
                return -1;
            }
            int val = *((int*) optval_);
            if (val != 0 && val != 1) {
                errno = EINVAL;
                return -1;
            }
            keepalive = val;
            return 0;
        }

    case XS_PROTOCOL:
        {
            if (optvallen_ != sizeof (int)) {
                errno = EINVAL;
                return -1;
            }
            int val = *((int*) optval_);
            if (val < 0) {
                errno = EINVAL;
                return -1;
            }
            protocol = val;
            return 0;
        }

    case XS_FILTER:
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        filter = *((int*) optval_);
        return 0;

    case XS_SURVEY_TIMEOUT:
        if (type != XS_SURVEYOR) {
            errno = ENOTSUP;
            return -1;
        }
        if (optvallen_ != sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        survey_timeout = *((int*) optval_);
        return 0;

    }

    errno = EINVAL;
    return -1;
}

int xs::options_t::getsockopt (int option_, void *optval_, size_t *optvallen_)
{
    switch (option_) {

    case XS_SNDHWM:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = sndhwm;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_RCVHWM:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rcvhwm;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_AFFINITY:
        if (*optvallen_ < sizeof (uint64_t)) {
            errno = EINVAL;
            return -1;
        }
        *((uint64_t*) optval_) = affinity;
        *optvallen_ = sizeof (uint64_t);
        return 0;

    case XS_IDENTITY:
        if (*optvallen_ < identity_size) {
            errno = EINVAL;
            return -1;
        }
        memcpy (optval_, identity, identity_size);
        *optvallen_ = identity_size;
        return 0;

    case XS_RATE:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rate;
        *optvallen_ = sizeof (int);
        return 0;
        
    case XS_RECOVERY_IVL:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = recovery_ivl;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_SNDBUF:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = sndbuf;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_RCVBUF:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rcvbuf;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_TYPE:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = type;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_LINGER:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = linger;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_RECONNECT_IVL:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = reconnect_ivl;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_RECONNECT_IVL_MAX:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = reconnect_ivl_max;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_BACKLOG:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = backlog;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_MAXMSGSIZE:
        if (*optvallen_ < sizeof (uint64_t)) {
            errno = EINVAL;
            return -1;
        }
        *((uint64_t*) optval_) = maxmsgsize;
        *optvallen_ = sizeof (uint64_t);
        return 0;

    case XS_MULTICAST_HOPS:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = multicast_hops;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_RCVTIMEO:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = rcvtimeo;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_SNDTIMEO:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = sndtimeo;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_IPV4ONLY:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = ipv4only;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_KEEPALIVE:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = keepalive;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_PROTOCOL:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = protocol;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_FILTER:
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = filter;
        *optvallen_ = sizeof (int);
        return 0;

    case XS_SURVEY_TIMEOUT:
        if (type != XS_SURVEYOR) {
            errno = ENOTSUP;
            return -1;
        }
        if (*optvallen_ < sizeof (int)) {
            errno = EINVAL;
            return -1;
        }
        *((int*) optval_) = survey_timeout;
        *optvallen_ = sizeof (int);
        return 0;

    }

    errno = EINVAL;
    return -1;
}