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
    Foam::externalIOObject::extPatch

Group
    grpFieldFunctionObjects

Description
   

Usage
    

SourceFiles
    extPatch.C

\*---------------------------------------------------------------------------*/

#ifndef externalIOObject_extPatch_H
#define externalIOObject_extPatch_H

#include "externalIOObject.H"
#include "meshSearch.H"
#include "vector.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{

/*---------------------------------------------------------------------------*\
                          Class extPatch Declaration
\*---------------------------------------------------------------------------*/

class extPatch
:
    public externalIOObject
{
    // Private Data

    word outputName_;

    word fieldName_;

    word patchName_;

    const fvMesh& mesh_;


public:

    //- Runtime type information
    TypeName("extPatch");


    // Constructors

        //- Construct from Time and dictionary
        extPatch
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        extPatch(const extPatch&) = delete;

        //- No copy assignment
        void operator=(const extPatch&) = delete;


    //- Destructor
    virtual ~extPatch() = default;


    // Member Functions

        //- Read the randomise data
        virtual bool read(const dictionary&);

        virtual bool execute();

        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace externalIOObject
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
