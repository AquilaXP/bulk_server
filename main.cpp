#include <iostream>

#include <boost\asio.hpp>
#include <boost/circular_buffer.hpp>

#include "server.h"

int main( int ac, char* av[] )
{
    try
    {
        if( ac != 3 )
            throw std::runtime_error( "incorect count param: need 2; 1-port, 2- bulk" );

        uint16_t port = std::atoi( av[1] );
        uint32_t bulk = std::atoi( av[2] );

        boost::asio::io_service io_service;

        server ser( io_service, 
            boost::asio::ip::tcp::endpoint( boost::asio::ip::address(), port ), bulk );
        io_service.run();
    }
    catch( const std::exception& err )
    {
        std::cerr << "Error:" << err.what() << std::endl;
    }
    
    return 0;
}