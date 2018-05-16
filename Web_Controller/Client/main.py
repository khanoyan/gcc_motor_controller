

# import binascii
# import sys
from datetime import datetime
import socket
# import tcpClient
import sys
import select
import struct

TCP_IP='10.22.0.11'
TCP_PORT = 23
BUFFER_SIZE = 1024

s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP,TCP_PORT))
print("Connected to "+TCP_IP)
dt = datetime.now()
currentTime = dt.microsecond 
lastTime = dt.microsecond
while (1):
	dt = datetime.now()
	currenTime=dt.microsecond
	print(currentTime)
	if(currentTime-lastTime>20000):
		lastTime =currentTime
		line = 7
		while sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
			line = sys.stdin.read(1)

		if line>0 and line<5:
			print(line)
			direction = int(line)
			a = chr(183)
			b = chr(direction)
			c = chr(0)
			d = chr(10)
			e = chr(237)
			values = ( a,b,c,d,e)
			d = struct.Struct('c c c c c')
			data = d.pack(*values)
			print("packed")
			s.send(data)
			print("Sending packets")
		elif line == 5:
				break
		else:
			a = chr(183)
			b = chr(0)
			c = chr(0)
			d = chr(10)
			e = chr(237)
			values = ( a,b,c,d,e)
			d = struct.Struct('c c c c c')
			data = d.pack(*values)
			s.send(data)
			print("Sending packets")	
	
	
	
	# print "sent data", type(data)
	# print sys.getsizeof(data)
	# print binascii.hexlify(data)
	#data = s.recv(BUFFER_SIZE)


s.close()
print("Closing Connection")
#set to 50 hz loop
#menu input
#direction for input
# loop command without stopping for input
#send packet
#be able to exit the menu
# 