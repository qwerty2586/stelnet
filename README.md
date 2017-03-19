##stelnet

stelnet is tool for encrypting telnet communication. Project is intended as proof of concept for applications where ssh 
server is too heavyweight, but you still need good secure connection.

## server mode
Server mode runs on computer with telnet host. Server listens for stelnet client to connect. 
After initializing connection and successful handshake and autentization few bytes are 
exchanged between client and server to get synchronize state between host and client.
After that only encrypted telnet data goes through channel. 
Only change state is done by by disconnecting either client or server

## client mode
Client mode runs on computer with telnet client. 
Client mode connecting to server stelnet part over network. After successful handshake and autentization, client and 
server exchange few bytes with state. After that client mode waits for telnet client to connect.
To ensure security, by default client mode listens only on localhost. 

<!-- language: lang-none -->


    ---------------------
    |      telnet       |
    ---------------------
             |
             V        
    ---------------------                         
    |     stelnet -c    |                        
    ---------------------
             |
             |                  local pc
    ====================================            
             |
             |                  network
             |
    ====================================            
             |                  remote pc                  
             V  
    ---------------------                         
    |     stelnet -s    |                        
    ---------------------             
             |
             V        
    ---------------------                         
    |      telnetd      |                        
    ---------------------                                  
    
    




