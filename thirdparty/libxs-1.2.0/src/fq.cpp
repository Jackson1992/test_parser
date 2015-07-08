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

#include "fq.hpp"
#include "pipe.hpp"
#include "err.hpp"
#include "msg.hpp"

xs::fq_t::fq_t () :
    active (0),
    current (0),
    more (false)
{
}

xs::fq_t::~fq_t ()
{
    xs_assert (pipes.empty ());
}

void xs::fq_t::attach (pipe_t *pipe_)
{
    pipes.push_back (pipe_);
    pipes.swap (active, pipes.size () - 1);
    active++;
}

void xs::fq_t::terminated (pipe_t *pipe_)
{
    //  Remove the pipe from the list; adjust number of active pipes
    //  accordingly.
    if (pipes.index (pipe_) < active) {
        active--;
        if (current == active)
            current = 0;
    }
    pipes.erase (pipe_);
}

void xs::fq_t::activated (pipe_t *pipe_)
{
    //  Move the pipe to the list of active pipes.
    pipes.swap (pipes.index (pipe_), active);
    active++;
}

int xs::fq_t::recv (msg_t *msg_, int flags_)
{
    return recvpipe (msg_, flags_, NULL);
}

int xs::fq_t::recvpipe (msg_t *msg_, int flags_, pipe_t **pipe_)
{
    //  Deallocate old content of the message.
    int rc = msg_->close ();
    errno_assert (rc == 0);

    //  Round-robin over the pipes to get the next message.
    for (pipes_t::size_type count = active; count != 0; count--) {

        //  Try to fetch new message. If we've already read part of the message
        //  subsequent part should be immediately available.
        bool fetched = pipes [current]->read (msg_);

        //  Check the atomicity of the message. If we've already received the
        //  first part of the message we should get the remaining parts
        //  without blocking.
        xs_assert (!(more && !fetched));

        //  Note that when message is not fetched, current pipe is deactivated
        //  and replaced by another active pipe. Thus we don't have to increase
        //  the 'current' pointer.
        if (fetched) {
            if (pipe_)
                *pipe_ = pipes [current];
            more =
                msg_->flags () & msg_t::more ? true : false;
            if (!more) {
                current++;
                if (current >= active)
                    current = 0;
            }
            return 0;
        }
        else {
            active--;
            pipes.swap (current, active);
            if (current == active)
                current = 0;
        }
    }

    //  No message is available. Initialise the output parameter
    //  to be a 0-byte message.
    rc = msg_->init ();
    errno_assert (rc == 0);
    errno = EAGAIN;
    return -1;
}

bool xs::fq_t::has_in ()
{
    //  There are subsequent parts of the partly-read message available.
    if (more)
        return true;

    //  Note that messing with current doesn't break the fairness of fair
    //  queueing algorithm. If there are no messages available current will
    //  get back to its original value. Otherwise it'll point to the first
    //  pipe holding messages, skipping only pipes with no messages available.
    for (pipes_t::size_type count = active; count != 0; count--) {
        if (pipes [current]->check_read ())
            return true;

        //  Deactivate the pipe.
        active--;
        pipes.swap (current, active);
        if (current == active)
            current = 0;
    }

    return false;
}
