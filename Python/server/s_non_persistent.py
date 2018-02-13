import socket
import os

port = 60002
s = socket.socket()
host = ""

s.bind((host, port))
s.listen(5)

# filename = raw_input("Enter file to share:")
print 'Server listening....'

while True:
    conn, addr = s.accept()
    print 'Got connection from', addr
    filename = conn.recv(1024)
    if(filename=="END"):
        print("Closing the connection")
        conn.close()
        continue
    if(os.path.exists(filename)):
        f = open(filename,'rb')
        l = f.read(1024)
        while (l):
           conn.send(l)
           l = f.read(1024)
        f.close()

        print('Done sending')
    else:
        print("File doesnt exist")
        conn.send("ERROR")
    conn.close()