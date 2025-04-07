#! /usr/bin/python3
import zmq
from time import sleep


ctx = zmq.Context()

publisher = ctx.socket(zmq.PUB)
publisher.bind("tcp://*:5555")

messages = ["100", "200", "300", "400", "500"]
# These keys will be used as envelopes in this multipart message
keys = ["A", "B", "C", "D", "E"]

for i in range(0, 15):  # 0 thru 15 exclusive
    index = i % 5  # 0 thru 4 inclusive
    print("publisher sending hello with envelope of {}\n".format(keys[index]))
    sleep(1)
    publisher.send_string(keys[index], zmq.SNDMORE)
    publisher.send_json({keys[index]: messages[index]})
