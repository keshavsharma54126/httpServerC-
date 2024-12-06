#include<iostream>
#include<cstring>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

#define PORT 8000

using namespace std;

int main(){
    //first we create a socket
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0){
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    //configure the socket address structure

    struct sockaddr_in address ;
    int opt = 1;
    int addrlen = sizeof(address);
     memset(&address,0,sizeof(address));
     address.sin_family = AF_INET;
     address.sin_addr.s_addr = INADDR_ANY;
     address.sin_port = htons(PORT);

    //bind the socket to the port 
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt , sizeof(opt))){
        perror("setsockopt failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))< 0){
        perror("Bind failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    //listen for the incoming connections 
    if(listen(server_fd,3)<0){
        perror("listen failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    cout<<"http server is running on port"<<PORT<<endl;

    //accept and handle client connections
    while(true){
        int new_socket =accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
        if(new_socket<0){
            perror("Accept failed");
            close(server_fd);
            return EXIT_FAILURE;
        }
    

        //read the client's http request;
        char buffer[30000] = {0};
        if(read(new_socket,buffer,sizeof(buffer))<0){
            perror("Error while reading request");
            close(server_fd);
            return EXIT_FAILURE;
        }
        cout<<"Request:\n"<<buffer<<endl;

        string response = "<html><body><h1>Hello, My name is Keshav and this is an http server implemented in cpp from scratch</h1></br><img src=https://hackernoon.imgix.net/images/jot3yv6.jpg/></body></html>";

        //construct an http response
        string http_response = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n"
                                "Content-Length: " + to_string(response.size()) + "\r\n"
                                "\r\n" +
                                response;

        //send the http response
        ;
        if(send(new_socket,http_response.c_str(),http_response.size(),0)<0){
            perror("Errro while sending message");
            close(server_fd);
            return EXIT_FAILURE;
        }


        //close the connection
        close(new_socket);


    }

    //close the server socket
    close(server_fd);
    return 0;

}