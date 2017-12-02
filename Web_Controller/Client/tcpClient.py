import struct
import socket

a = chr(183)
b = chr(4)
c = chr(0)
d = chr(10)
e = chr(237)

values = ( a,b,c,d,e)
s = struct.Struct('c c c c c')
data = s.pack(*values)

mybytes = 'B704000AED'.decode('hex')
TCP_IP='10.22.0.11'
TCP_PORT = 23
BUFFER_SIZE = 1024

s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP,TCP_PORT))
s.send(mybytes)
#data = s.recv(BUFFER_SIZE)
s.close()

print "sent data", mybytes
