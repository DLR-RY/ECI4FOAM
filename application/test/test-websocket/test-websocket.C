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

#include "zmq_socket.H"


#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>

// basically checks if installed correctly
TEST_CASE( "test websocket", "[boost socket]" )
{
    // create and connect socket
    Foam::zmq_socket sock("127.0.0.1",8000);

    Foam::word send = "Hello";
    sock.write(send);
        

    Foam::word recv = sock.read();
    REQUIRE(recv == "World");
}
