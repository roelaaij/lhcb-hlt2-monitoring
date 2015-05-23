#!/usr/bin/env python2

from monitoring_pb2 import Chunk
import zmq

if __name__ == '__main__':
    ctx = zmq.Context(1)
    sck = ctx.socket(zmq.SUB)
    sck.connect("tcp://localhost:31337")
    sck.setsockopt(zmq.SUBSCRIBE, "")
    while True:
        string = sck.recv()
        res = Chunk()
        Chunk.ParseFromString(res, string)
        print(res.tck)
