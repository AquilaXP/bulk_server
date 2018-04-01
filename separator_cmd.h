#pragma once

#include <cstdint>
#include <string>
#include <iostream>

#include "async.h"

/// Делит сырые данные на части:
/// сырые данные с обычными коммандами и сырые данные с блоками комманд, 
/// после отсылается в разные обработчики.
class separator_cmd
{
public:
    separator_cmd() = default;

    void init( async::handle_t single_cmd, async::handle_t block_cmd )
    {
        m_single_cmd = single_cmd;
        m_block_cmd = block_cmd;
    }

    void push_data( const char* data, size_t size )
    {
        m_buffer.append( data, size );

        while( true )
        {
            auto b = m_buffer.find_first_of( '\n', m_procced_symbol );
            if( b == m_buffer.npos )
                break;

            bool curr_close = false;
            if( b - m_procced_symbol == 1 )
            {
                switch( m_buffer[b - 1] )
                {
                    case '{':
                        ++m_lvl;
                        break;
                    case '}':
                        --m_lvl;
                        curr_close = true;
                        break;
                }
            }
            if( m_lvl == 0 && curr_close == false )
            {
                async::receive( m_single_cmd, &m_buffer[m_procced_symbol], b - m_procced_symbol + 1 );
            }
            else
            {
                async::receive( m_block_cmd, &m_buffer[m_procced_symbol], b - m_procced_symbol + 1 );
            }

            m_procced_symbol = b + 1;
        }
        Update();
    }
private:
    void Update()
    {
        std::move( m_buffer.begin() + m_procced_symbol, m_buffer.end(), m_buffer.begin() );
        m_buffer.erase( m_buffer.size() - m_procced_symbol );
        m_procced_symbol = 0;
    }

    uint32_t m_procced_symbol = 0;
    std::string m_buffer;
    uint32_t m_lvl = 0;
    async::handle_t m_single_cmd = nullptr;
    async::handle_t m_block_cmd = nullptr;
};
