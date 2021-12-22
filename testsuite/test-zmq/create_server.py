
import zmq


context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://127.0.0.1:8000")

message = socket.recv()


socket.send(b"World")