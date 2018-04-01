#pragma once

#include <cstdint>

#include <boost/asio.hpp>

#include "separator_cmd.h"
#include "async.h"

const uint32_t size_block_read = 8096;

class server;

/// Обработчик одного соединения
class handler_connect
{
public:
    handler_connect( server* server, 
        async::handle_t simple_cmd, boost::asio::ip::tcp::socket&& socket, uint32_t bulk );
    ~handler_connect();

private:
    void do_read();
    void disconnect();

    std::string data;
    char m_buffer[size_block_read];
    separator_cmd m_separator_cmd;
    server* m_server = nullptr;
    async::handle_t m_handler;
    boost::asio::ip::tcp::socket m_socket;
};