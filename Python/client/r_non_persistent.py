import socket

host = ""
port = 60002

while True:
    s = socket.socket()
    try:
        flag1=0
    	s.connect((host, port))
    	fname=raw_input("Enter File name: ")
        if(fname=="q"):
            flag1=1
            print("Exiting")
            s.send("END")
        else:
            print(fname)
            s.send(fname)
            flag=0
            with open(fname, 'wb') as f:
                while True:
                    data = s.recv(1024)
                    if("ERROR" in data):
                        flag=1
                        break
                    if not data:
                        break
                    f.write(data)
            f.close()
            if not flag:
                print('Successfully get the file')
            else:
                print("No such file found")
        s.close()
        print('connection closed')
    except:
        print("Unable to connect to the server")
    if(flag1):
        break