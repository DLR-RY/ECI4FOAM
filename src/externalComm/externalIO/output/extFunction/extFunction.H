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
    Foam::externalIOObject::extFunction

Description
    auto put a time varying function based on function 1

Usage
    Minimal example by using \c system/externalCouplingDict:
    \verbatim
    extFunction
    {
        type extFunction;
        function constant 0;
        varName dTout;
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Req'd | Dflt
      type         | Type name: extFunction                     | word |  yes  | -
      function         | timevarying function | Function1<scalar> |  yes  | -
      varName        | name of the variable         | word |  yes  | -
    \endtable

SourceFiles
    extFunction.C

\*---------------------------------------------------------------------------*/

#ifndef externalIOObject_extFunction_H
#define externalIOObject_extFunction_H

#include "externalIOObject.H"
#include "Function1.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{

/*---------------------------------------------------------------------------*\
                          Class extFunction Declaration
\*---------------------------------------------------------------------------*/

class extFunction
:
    public externalIOObject
{
    // Private Data

    //- Function
    autoPtr<Function1<scalar>> func_;

    word varName_;


public:

    //- Runtime type information
    TypeName("extFunction");


    // Constructors

        //- Construct from Time and dictionary
        extFunction
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        extFunction(const extFunction&) = delete;

        //- No copy assignment
        void operator=(const extFunction&) = delete;


    //- Destructor
    virtual ~extFunction() = default;


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
