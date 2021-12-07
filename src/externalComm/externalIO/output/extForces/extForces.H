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
    Foam::externalIOObject::extForces

Description
    Computes the natural forces and makes accesible for exchange

Usage
    Minimal example by using \c system/externalCouplingDict:
    \verbatim
    extForces
    {
        type extForces;
        patches (".*");
        CofR  (0 0 0);
        forceName force;
        momentName moment;
    }
    \endverbatim

        Where the entries comprise:
    \table
        Property     | Description             | Required    | Default value
        type         | Type name: forces       | yes         |
        patches      | Patches included in the forces calculation | yes |
        CofR         | Centre of rotation (see below) | no   |
    \endtable



SourceFiles
    extForces.C

\*---------------------------------------------------------------------------*/

#ifndef externalIOObject_extForces_H
#define externalIOObject_extForces_H

#include "externalIOObject.H"
#include "vector.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{

/*---------------------------------------------------------------------------*\
                          Class extForces Declaration
\*---------------------------------------------------------------------------*/

class extForces
:
    public externalIOObject
{
    // Private Data


    word forceName_;

    word momentName_;

    word CofRName_;
    
    vector CofR_;



public:

    //- Runtime type information
    TypeName("extForces");


    // Constructors

        //- Construct from Time and dictionary
        extForces
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        extForces(const extForces&) = delete;

        //- No copy assignment
        void operator=(const extForces&) = delete;


    //- Destructor
    virtual ~extForces() = default;


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
