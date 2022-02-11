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

#include "socket.H"
#include "IOmanip.H"

namespace Foam
{
    defineTypeNameAndDebug(socket, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
Foam::socket::socket
(
    const dictionary& dict
)
:
    dict_(dict),
    ioc_(),
    resolver_(ioc_),
    sock_(ioc_),
    addr_()
{
    // The io_context is required for all I/O
    word host = dict_.get<word>("host");
    label port = dict_.get<label>("port");

    // Look up the domain name
    // auto const results = resolver_.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    // net::connect(socket, results.begin(), results.end());

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    
    // addr_ = host + ':' + word(port); //std::to_string(ep.port());
    tcp::endpoint endpoint(
    boost::asio::ip::address::from_string(host), port);
// socket.connect(endpoint);
    sock_.connect(endpoint);


}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::socket::~socket()
{
    sock_.close();//beast::socket::close_code::normal);
}


Foam::word Foam::socket::read()
{
    size_t size = 0;

    net::read(sock_, net::buffer(&size, sizeof(size_t)));
    std::vector<char> msg(size);
    net::read(sock_, net::buffer(msg.data(), size));
    word itemToReceive = msg.data();

    return itemToReceive;
}


void Foam::socket::write(word w)
{
    size_t size = w.size() + 1;

    net::write(sock_, net::buffer(&size, sizeof(size_t)));
    net::write(sock_, net::buffer(w.c_str(), size));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// ************************************************************************* //
