#pragma once

#include <set>
#include <memory>

#include <boost/asio.hpp>

#include "async.h"
#include "handler_connect.h"

/// Сервер
///
/// Ожидает соединения, создается обработчик на каждое соединение,
/// создается отдельный обработчик для обычных комманд
class server
{
    friend handler_connect;
public:
    server( boost::asio::io_service& io_service, 
        const boost::asio::ip::tcp::endpoint& end_point, uint32_t bulk );

private:
    void do_accept();
    void remove_handler( handler_connect* hc );

    std::set<std::shared_ptr<handler_connect>> m_handlers;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_service& m_service;
    async::handle_t m_simple_cmd_handler = nullptr;
    uint32_t m_bulk = 0;
};