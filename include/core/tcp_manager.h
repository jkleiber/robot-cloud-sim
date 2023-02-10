
#pragma once

#include <iostream>
#include <string.h>

#include <asio.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

class TCPManager
{
public:
    TCPManager(::asio::io_context &io, ::asio::ip::tcp::endpoint endpoint)
    {
        tcp_acceptor_ =
            std::make_shared<::asio::ip::tcp::acceptor>(io, endpoint);
        this->StartConnection();
    }

private:
    // TCP acceptors
    std::shared_ptr<::asio::ip::tcp::acceptor> tcp_acceptor_;

    // TCP connection information.
    std::shared_ptr<::asio::ip::tcp::socket> tcp_socket_;

    // Test buffers
    std::string msg_ = "test";
    char data_[1024];

    void StartConnection();
    void HandleAccept(::asio::error_code &err, ::asio::ip::tcp::socket peer);
    void HandleRead(::asio::error_code &err, size_t num_bytes);
    void HandleWrite(::asio::error_code &err, size_t num_bytes);
};