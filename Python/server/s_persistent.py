import socket
import os

port = 60001
s = socket.socket()
host = ""

s.bind((host, port))
s.listen(5)


while True:
	print 'Server listening....'
	conn, addr = s.accept()

	while True:
		print 'Got connection from', addr
		filename = conn.recv(1024)
		print filename
		if(filename=="END"):
			print("Closing the connection")
			conn.close()
			break
		if(os.path.exists(filename)):
			f = open(filename,'rb')
			l = f.read(1024)
			while l:
				conn.send(l)
			#   print('Sent ',repr(l))
				l = f.read(1024)
			f.close()

			print('Done sending')
			conn.send("FOR_EOF")
		else:
			print("File doesnt exist")
			conn.send("FILE_ERROR")
	conn.close()