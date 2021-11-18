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


Author
    Henning Scheufler, DLR, all rights reserved.

Class
    Foam::functionObjects::externalController

Group
    grpUtilitiesFunctionObjects

Description


Usage


Note


See also
    Foam::functionObject
    Foam::functionObjects::timeControl

SourceFiles
    externalController.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_externalController_H
#define functionObjects_externalController_H

#include "functionObject.H"
#include "stringList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                         Class externalController Declaration
\*---------------------------------------------------------------------------*/

class externalController
:
    public functionObject
{
protected:

    // Private data


    // Protected Member Functions

        //- No copy construct
        externalController(const externalController&) = delete;

        //- No copy assignment
        void operator=(const externalController&) = delete;


public:

    //- Runtime type information
    TypeName("externalController");


    // Constructors

        //- Construct from Time and dictionary
        externalController
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~externalController() = default;


    // Member Functions

        //- Read the system calls
        virtual bool read(const dictionary& dict);

        //- Execute the "executeCalls" at each time-step
        virtual bool execute();

        //- Write, execute the "writeCalls"
        virtual bool write();

        //- Execute the "endCalls" at the final time-loop
        virtual bool end();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
