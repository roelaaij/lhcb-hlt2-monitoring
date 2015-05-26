//
//  Simple message queuing broker in C++
//  Same as request-reply broker but using QUEUE device
//
// Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>
#include <iostream>

#include "zmq.hpp"

int main (void)
{
    zmq::context_t context(1);

    //  Socket facing clients
    zmq::socket_t frontend {context, ZMQ_XSUB};
    frontend.bind("tcp://*:31337");

    //  Socket facing services
    zmq::socket_t backend{context, ZMQ_XPUB};
    backend.connect("tcp://localhost:31338");

    //  Start the proxy
    zmq_proxy(frontend, backend, nullptr);
    return 0;
}
