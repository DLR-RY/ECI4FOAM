/*---------------------------------------------------------------------------*\
            Copyright (c) 2021, German Aerospace Center (DLR)
-------------------------------------------------------------------------------
License
    This file is part of the ECI4FOAM source code library, which is an
	unofficial extension to OpenFOAM.
    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.
    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "websocket.H"
#include "IOmanip.H"

namespace Foam
{
    defineTypeNameAndDebug(websocket, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
Foam::websocket::websocket
(
    const dictionary& dict
)
:
    ioc_(),
    resolver_(ioc_),
    ws_(ioc_),
    addr_()
{
    // 
    word host = dict.get<word>("host");
    word port = dict.get<word>("port");
    word endpoint = dict.getOrDefault<word>("endPoint","/");

    // Look up the domain name
    auto const results = resolver_.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    auto ep = net::connect(ws_.next_layer(), results);
  
    addr_ = host + ':' + std::to_string(ep.port());

    // Set a decorator to change the User-Agent of the handshake
    ws_.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::request_type& req)
        {
            req.set(http::field::user_agent,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-coro");
        }));

    // Perform the websocket handshake
    ws_.handshake(addr_, endpoint);
}


Foam::websocket::websocket
(
    word host,
    label port,
    word endpoint
)
:
    ioc_(),
    resolver_(ioc_),
    ws_(ioc_),
    addr_()
{
    // Look up the domain name
    auto const results = resolver_.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    auto ep = net::connect(ws_.next_layer(), results);
  
    addr_ = host + ':' + std::to_string(ep.port());

    // Set a decorator to change the User-Agent of the handshake
    ws_.set_option(beast::websocket::stream_base::decorator(
        [](beast::websocket::request_type& req)
        {
            req.set(http::field::user_agent,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-coro");
        }));

    // Perform the websocket handshake
    ws_.handshake(addr_, endpoint);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::websocket::~websocket()
{
    ws_.close(beast::websocket::close_code::normal);
}


Foam::word Foam::websocket::read()
{
    beast::flat_buffer buffer;

    ws_.read(buffer);

    word response = boost::beast::buffers_to_string(buffer.data());

    return response;
}


void Foam::websocket::write(word w)
{
    ws_.write(net::buffer(w));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// ************************************************************************* //
