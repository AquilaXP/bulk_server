#include "handler_connect.h"

#include "server.h"

handler_connect::handler_connect( server* server, async::handle_t simple_cmd, boost::asio::ip::tcp::socket&& socket, uint32_t bulk )
    : m_server(server), m_socket( std::move( socket ) )
{
    m_handler = async::connect( bulk );
    m_separator_cmd.init( simple_cmd, m_handler );
    do_read();
}

handler_connect::~handler_connect()
{
    disconnect();
}

void handler_connect::do_read()
{
    boost::asio::async_read(
        m_socket, boost::asio::buffer( m_buffer, size_block_read ),
        [this]( boost::system::error_code ec, std::size_t length )
    {
        /// Дочитываем в любой случае
        if( length != 0 )
            m_separator_cmd.push_data( m_buffer, length );

        if( ec || length == 0 )
        {
            m_server->remove_handler( this );
        }
        else
        {
            do_read();
        }
    } );
}

void handler_connect::disconnect()
{
    if( m_handler )
    {
        async::disconnect( m_handler );
        m_handler = nullptr;
    }
}