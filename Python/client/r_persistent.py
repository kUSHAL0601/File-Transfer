import socket                   

host = ""
port = 60001                

while True:
    s = socket.socket()  
    s.connect((host, port))
    #s.send("Hello server!")
    #print("Enter File Name")
    flag1=0
    while True:
        fname=raw_input("Enter File name:")
        if fname=='q':
            flag1=1
            s.send("END")
            break
       # print("SEND 1")
        s.send(fname)
        #print("SEND 2")
        flag=0
        with open(fname, 'wb') as f:
            #print 'file opened'
            while True:
            # print('receiving data...')
                data = s.recv(1024)
                if "FILE_ERROR" in data:
                    flag=1
                    break
                #print('data=%s', (data))
                if "FOR_EOF" in data or not data:
                    break
                # write data to a file
                f.write(data)
                #print("Hello")

        f.close()
        if not flag:
            print('Successfully get the file')
        else:
            print('No such file found')
    s.close()
    print('connection closed')
    if flag1:
        break