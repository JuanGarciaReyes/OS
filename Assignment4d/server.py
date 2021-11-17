# -*- coding: utf-8 -*-
"""
Created on Wed Nov 17 12:02:55 2021

@author: Juan Garcia
"""

import socket
from _thread import *

#Creates the socket
ServerSocket = socket.socket()
#Create the addresses
host = '127.0.0.1'
port = 1233
#set to listen from income connections
ThreadCount = 0
try:
    ServerSocket.bind((host, port))
except socket.error as e:
    print(str(e))

print('Waiting for a Connection..')
ServerSocket.listen(5)

#Function to send the info to the client
def threaded_client(connection):
    #open the file
    bio_text = open("bio.txt")
    connection.send(str.encode('Welcome to the Server'))
    #read the file
    buffer = bio_text.read()
    #Sends 1024 bytes at the time to the client 
    while len(buffer) > 0:
        connection.send(buffer[:1024].encode())
        buffer = buffer[1024:]
    #close the conncetion
    connection.close()

#Creates new threads for every connection
while True:
    Client, address = ServerSocket.accept()
    print('Connected to: ' + address[0] + ':' + str(address[1]))
    start_new_thread(threaded_client, (Client, ))
    ThreadCount += 1
    print('Thread Number: ' + str(ThreadCount))
ServerSocket.close()