#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#

from pygecko.multiprocessing import geckopy
import pickle

class PickleJar(object):
    def __init__(self, fname, buffer_size=500):
        self.fd = open(fname, 'wb')
        self.buffer = {}
        self.buffer_size = buffer_size
        self.counter = 0

    def __del__(self):
        self.close()

    def push(self, topic, data):
        # self.buffer.append(data)
        if topic not in self.buffer:
            self.buffer[topic] = []
        self.buffer[topic].append(data)
        self.counter += 1
        if self.counter > self.buffer_size:
            self.write()
            self.counter = 0

    def write(self):
        for d in self.buffer:
            pickle.dump(d, self.fd)
        self.buffer = {}

    def close(self):
        self.fd.close()


class Callback(object):
    def __init__(self):
        self.jar = PickleJar('test.pickle')
    def callback(self, topic, msg):
        # geckopy.loginfo("recv[{}]: {}".format(topic, msg))
        self.jar.push(topic, msg)

def subscriber(**kwargs):
    geckopy.init_node(**kwargs)
    c = Callback()
    geckopy.Subscriber(['camera'], c.callback)

    geckopy.spin(2) # it defaults to 100hz, this is just to slow it down
    print('sub bye ...')

if __name__ == '__main__':

    args = {
        'geckocore': {
            'key': 'nav'
        }
    }
    subscriber(**args)
