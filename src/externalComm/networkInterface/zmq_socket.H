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

Class
    Foam::zmq_socket

Description
    zmq req client connects to address via tcp
    
    currently only send and receives strings/json

Author
    Henning Scheufler, DLR, all rights reserved.

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef zmq_socket_H
#define zmq_socket_H

#include "typeInfo.H"
#include "dictionary.H"

#include "zmq_addon.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


namespace Foam 
{

class zmq_socket
{
private:

    //- zmq context
    zmq::context_t ctx_;

    //- zmq req socket 
    zmq::socket_t sock_;

    //- server address
    word addr_;


public:

    //- Runtime type information
    TypeName("zmq_socket");

    // Constructors
    zmq_socket
    (
        const dictionary& dict
    );

    // construct from components
    zmq_socket
    (
        word host,
        label port
    );

    //- Destructor
    virtual ~zmq_socket();

    word read();

    void write(word w);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
