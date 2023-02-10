
#include "core/tcp_manager.h"

void TCPManager::StartConnection()
{
    // Make the socket
    tcp_socket_ = std::make_shared<::asio::ip::tcp::socket>(
        tcp_acceptor_->get_executor().context());

    // Asynchronously accept connections on the socket
    tcp_acceptor_->async_accept(*tcp_socket_, &TCPManager::HandleAccept);
}

void TCPManager::HandleAccept(::asio::error_code &err,
                              ::asio::ip::tcp::socket peer)
{
    // If there aren't errors, register the callbacks.
    if (!err)
    {
        // Read asynchronously
        ::asio::async_read(*tcp_socket_, ::asio::buffer(data_, 1024),
                           &TCPManager::HandleRead);

        // Write asynchronously
        ::asio::async_write(*tcp_socket_, ::asio::buffer(msg_, 1024),
                            &TCPManager::HandleWrite);
    }

    // Start any future connections.
    this->StartConnection();
}

void TCPManager::HandleRead(::asio::error_code &err, size_t num_bytes)
{
    // If there isn't an error, perform the read.
    if (!err)
    {
        std::cout << data_ << std::endl;
    }
    else
    {
        // Otherwise print the error and close the TCP socket.
        std::cerr << "ERROR: " << err.message() << std::endl;
        tcp_socket_->close();
    }
}

void TCPManager::HandleWrite(::asio::error_code &err, size_t num_bytes)
{
    // If there isn't an error, do nothing useful.
    if (!err)
    {
        std::cout << "Message: '" << msg_ << "' sent successfully" << std::endl;
    }
    else
    {
        // Otherwise print the error and close the TCP socket.
        std::cerr << "ERROR: " << err.message() << std::endl;
        tcp_socket_->close();
    }
}