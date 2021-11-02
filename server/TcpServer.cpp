/**
 * TCP server for reports
 */

#include <cstdio>
#include <cstring>
#include <exception>
#include <iostream>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "TcpServer.hh"
#include "common.hh"

TcpServer::TcpServer(int port):
    port_ {port},
    sockfd_(0),
    clientfd_(0),
    terminate_(false)
{
    // Open port for listening
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd_ < 0) {
        perror("TcpServer: ");
        throw;     // TODO specify
    }

    // set socket for reuse
    socklen_t option = 1;
    setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    struct sockaddr_in server_adr = {0};
    server_adr.sin_family = AF_INET;
    server_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_adr.sin_port = htons(port_);

    if(bind(sockfd_, (struct sockaddr*)&server_adr, sizeof(server_adr)) < 0) {
        perror("TcpServer bind:");
        throw;
    }

    std::cout << '\n' << __PRETTY_FUNCTION__ << ": Serving on port " << port_;
    std::cout << '\n' << __PRETTY_FUNCTION__ << ": Listening...";

    if(-1 == listen(sockfd_, 1))
    {
        // Listen failed
        perror("TcpServer listen: ");
        throw;
    }

    // Start client acception thread
    client_thread = new std::thread(&TcpServer::client_handler, this);
    //accept_();
}

TcpServer::~TcpServer()
{
    std::cout << "\nStopping TCP server\n";
    // Stop accepting thread
    terminate_ = true;
    close(clientfd_);    // unblock read()
    close(sockfd_);    // unblock accept()
    client_thread->join();
    
    // Close descriptors
    close(clientfd_);
    close(sockfd_);
}

void TcpServer::set_callback(receive_callback_t callback)
{
    callback_ = callback;
}

/**
 * Infinite client handling loop.
 *
 * @note Currently we support only 1 client
 */
void TcpServer::client_handler()
{
    while(!terminate_)
    {
        std::cout << '\n' <<  __PRETTY_FUNCTION__ << ": Waiting for client to connect...";
        clientfd_ = accept_(); // Store newly connected client fd
        if(clientfd_ >= 0)
        {
            // Client connected. Communicate until disconnection
            while(receive_() >=0)
                ;
            // Client disconnected
            clientfd_ = 0;    // No more client
        }
    }
    std::cout << '\n' <<  __PRETTY_FUNCTION__ << ": Client thread finished\n";
}

/*!
 * Accepting incoming connection.
 *
 * @note    blocking function
 *
 * @return    new client fd, <= 0 means error
 *
 */
int TcpServer::accept_()
{
    std::cout << '\n' <<  __PRETTY_FUNCTION__ << ": Accepting..." << std::flush;

    struct sockaddr_in client_address;
    socklen_t size = sizeof(client_address);

    int new_fd = accept(sockfd_, (struct sockaddr*)&client_address, &size);

    if(-1 == new_fd) // Accept failed
        perror("TcpServer accept: ");
    else
        std::cout << "\nAccepted incoming connecton: " << clientfd_ << std::flush;

    return new_fd;
}

/*!
 * Extract '\n\n' terminated line(s) from rcv buffer and make a callback if set
 *
 * @param    scnt    Number of bytes in input buffer
 */
void TcpServer::process_buf(ssize_t scnt)
{
    /*
     * Search for \n\n termination.
     * Send all data before termination to callback
     * Store the rest.
     */

    std::cout << '\n' << __PRETTY_FUNCTION__;

    static bool eol_flag = false;
    
    int start_of_chunk = 0;    // Index char for current sentence chunk

    for(int i = 0; i < scnt; ++i) {
        if(rcv_buf[i] == '\n')
            if(eol_flag) // Found termination
            {
                std::cout << " Found termination at: " << i << std::flush;
                // Append the end of sentence, excluding '\n'
                if(i > start_of_chunk + 1)    // there is something besides termination
                    keeped_.append(rcv_buf + start_of_chunk, i - start_of_chunk - 1);

                if(callback_)
                    callback_(keeped_);

                keeped_.clear();
                eol_flag = false;
                start_of_chunk = i + 1;    // now points to next char
            }
            else
                eol_flag = true;
        else
            eol_flag = false;
    }

    // Check if something rest in input buffer
    if(start_of_chunk < scnt) {
        std::cout << " Appending the rest" << std::flush;
        keeped_.append(rcv_buf + start_of_chunk, scnt - start_of_chunk);
        std::cout << " | keeped_: " << keeped_ << std::flush;
    }
}

/**!
 * Receiving from client.
 *
 * @note blocking function
 *
 * @return    number of bytes received, <= 0 indicates an error
 * */
int TcpServer::receive_()
{
    // recv() returns 0 on disconnect, <0 on error
    ssize_t scnt = recv(clientfd_, rcv_buf, sizeof(rcv_buf), 0);

    if(scnt <= 0) {
        // Connection closed
        std::cout << '\n' << __PRETTY_FUNCTION__ << ": Client connection closed" << std::flush;
        close(clientfd_);
        clientfd_ = 0;
        return scnt;
    }

    std::cout << '\n' << __PRETTY_FUNCTION__ << ": Received " << scnt << " bytes : ";

    dump(rcv_buf, scnt);

    // Send data for processing
    process_buf(scnt);

    return scnt;
}

int TcpServer::writeline(const char *pc)
{
    // Write line to socket
    size_t len = strlen(pc);
    int cnt = send(clientfd_, pc, len, 0);
    if(cnt != (int)len) {
        std::cout << '\n' << __PRETTY_FUNCTION__ << ": Error writing line: " << pc;
        return 1;
    }
    return 0;
}

int TcpServer::writeline(const std::string &line)
{
    // Write line to socket
    writeline(line.c_str());
    return 0;
}

int TcpServer::sock_open_()
{
    return 0;
}
