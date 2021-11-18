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

Class
    Foam::registeredObject

Description

Author
    Henning Scheufler, DLR, all rights reserved.

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef registeredObject_H
#define registeredObject_H

#include "typeInfo.H"
#include "fvMesh.H"
#include "regIOobject.H"
#include <typeinfo>
#include "autoPtr.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam 
{

template<class Type>
class registeredObject
:  
    public regIOobject
{

    word name_;
    Type T_;

public:

    TypeName("registeredObject");

    // Constructors
    registeredObject
    (
        const Type& obj,
        const word name,
        const objectRegistry& db
    );


    // Member Functions    

    // IO
        virtual bool writeData(Ostream& os) const
        {
            os << T_;
            return true;
        }

        const word& name() const
        {
            return name_;
        }

        // word& name()
        // {
        //     return name_;
        // }

        Type& ref()
        {
            return T_;
        }

        const Type& ref() const
        {
            return T_;
        }


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
