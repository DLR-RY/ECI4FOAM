/*---------------------------------------------------------------------------*\
            Copyright (c) 2021, German Aerospace Center (DLR)
-------------------------------------------------------------------------------
License
    This file is part of the VoFLibrary source code library, which is an
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

#include "zmq_socket.H"
#include "IOmanip.H"

namespace Foam
{
    defineTypeNameAndDebug(zmq_socket, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
Foam::zmq_socket::zmq_socket
(
    const dictionary& dict
)
:
    ctx_(),
    sock_(ctx_, zmq::socket_type::req),
    addr_()
{
    // The io_context is required for all I/O
    word host = dict.get<word>("host");
    label port = dict.get<label>("port");
    
    addr_ = host + ':' + name(port);

    sock_.connect(word("tcp://" + addr_));
}

Foam::zmq_socket::zmq_socket
(
    word host,
    label port
)
:
    ctx_(),
    sock_(ctx_, zmq::socket_type::req),
    addr_()
{
    addr_ = host + ':' + name(port);

    sock_.connect(word("tcp://" + addr_));

}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::zmq_socket::~zmq_socket()
{
    sock_.close();
}


Foam::word Foam::zmq_socket::read()
{
    zmq::message_t z_in;
    sock_.recv(z_in);
    word answer(z_in.to_string());
    return answer;
}

void Foam::zmq_socket::write(word w)
{
    zmq::message_t z_out(w);
    sock_.send(z_out,zmq::send_flags::none);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// ************************************************************************* //
