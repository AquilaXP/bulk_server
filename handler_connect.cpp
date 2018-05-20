#include "handler_connect.h"

#include <boost/asio/deadline_timer.hpp>
#include "server.h"

const auto timeout_read = boost::posix_time::seconds(1);

handler_connect::handler_connect( server* server, async::handle_t simple_cmd, boost::asio::ip::tcp::socket&& socket, uint32_t bulk )
    : m_server(server),
    m_socket( std::move( socket ) ),
    deadline( socket.get_io_service() ),
    m_simple_handler( simple_cmd )
{
    m_handler = async::connect( bulk );
    m_separator_cmd.init( simple_cmd, m_handler );
    deadline.expires_from_now( timeout_read );
    deadline.async_wait( [this]( const boost::system::error_code& ec ){ check_deadline(ec); } );
    do_read();
}

handler_connect::~handler_connect()
{
    disconnect();
}

void handler_connect::do_read()
{
    //m_socket.async_read_some(
    boost::asio::async_read( m_socket,
        boost::asio::buffer( m_buffer, size_block_read ),
        [this]( boost::system::error_code ec, std::size_t length )
    {
        /// Дочитываем в любой случае
        if( length != 0 )
            m_separator_cmd.push_data( m_buffer, length );
        if( (ec  || length == 0) && ec != boost::asio::error::operation_aborted )
        {
            disconnect();
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
        deadline.cancel();
        m_server->remove_handler( this );
        async::disconnect( m_handler );
        async::flush( m_simple_handler );
        m_handler = nullptr;
    }
}

void handler_connect::check_deadline( const boost::system::error_code& ec )
{
    if( ec )
    {
        return;
    }
    m_socket.cancel();
    deadline.expires_from_now( timeout_read );
    deadline.async_wait( [this]( const boost::system::error_code& ec ){ check_deadline(ec); } );
    async::flush( m_handler );
    async::flush( m_simple_handler );
}
