#ifndef _TcpServer_hpp_
#define _TcpServer_hpp_

#include <string>
#include <thread>
#include <functional>

using receive_callback_t = std::function<void(const std::string &s)>;

class TcpServer
{
    public:

        TcpServer(int port = 39003);
        ~TcpServer();

        int writeline(const char *pc);
        int writeline(const std::string &line);

        void set_callback(receive_callback_t callback);

    private:

        char rcv_buf[2048];
        std::string keeped_;

        int port_;            //! TCP port (serving on localhost)
        int sockfd_;        //! Listening socket descriptor

        int sock_open_();
        int accept_();
        int receive_();
        void process_buf(ssize_t scnt);
        
        int clientfd_ = 0;    //! Client socked descriptor. Disconnected if <= 0
        void client_handler();
        std::thread *client_thread;
        bool terminate_;    // Termination flag for thread(s)

        receive_callback_t callback_;
};

#endif
