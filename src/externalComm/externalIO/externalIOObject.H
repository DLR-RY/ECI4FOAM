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


Namespace
    Foam::externalIOObjects

Description
    similar to functionObject but with externalIO

Class
    Foam::externalIOObject

Description
    Abstract base-class for Time/database function objects.

See also

SourceFiles
    externalIOObject.C

\*---------------------------------------------------------------------------*/

#ifndef externalIOObject_H
#define externalIOObject_H

#include "typeInfo.H"
#include "autoPtr.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward Declarations
class Time;
class polyMesh;
class mapPolyMesh;

namespace externalIOObject
{
/*---------------------------------------------------------------------------*\
                       Class externalIOObject Declaration
\*---------------------------------------------------------------------------*/

class externalIOObject
{
    // Private Data


protected:
        //- Name
        const word name_;

        const Time& time_;

        const dictionary& dict_;

    // Protected Member Functions

        //- Return a scoped name, e.g. used to construct local field names
        word scopedName(const word& name) const;


public:

    // Forward declarations
    class unavailableexternalIOObject;

    //- Runtime type information
    virtual const word& type() const = 0;

    //- Flag to execute debug content
    static int debug;

    //- Directory prefix
    static word outputPrefix;

    //- Flag to write log into Info
    bool log;


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            externalIOObject,
            dictionary,
            (const word& name, const Time& runTime, const dictionary& dict),
            (name, runTime, dict)
        );


    // Constructors

        //- Return clone
        autoPtr<externalIOObject> clone() const
        {
            NotImplemented;
            return nullptr;
        }

    //- Construct from components
        externalIOObject
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    // Selectors

        //- Select from dictionary, based on its "type" entry
        static autoPtr<externalIOObject> New
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


        //- Destructor
        virtual ~externalIOObject() = default;


    // Member Functions

        //- Return the name of this externalIOObject
        const word& name() const;

        //- Read and set the function object if its data have changed
        virtual bool read(const dictionary& dict);

        //- Called at each ++ or += of the time-loop.
        virtual bool execute() = 0;

        //- Execute using the specified subIndex.
        //  The base implementation is a no-op.
        //  \param subIndex an execution sub-index corresponding to a
        //      sub-cycle or something similar.
        virtual bool execute(const label subIndex);

        //- Called at each ++ or += of the time-loop.
        virtual bool write() = 0;

        //- Called when Time::run() determines that the time-loop exits.
        //  The base implementation is a no-op.
        virtual bool end();

        //- Called at the end of Time::adjustDeltaT() if adjustTime is true
        virtual bool adjustTimeStep();

        //- Did any file get changed during execution?
        virtual bool filesModified() const;

        //- Update for changes of mesh
        //  The base implementation is a no-op.
        virtual void updateMesh(const mapPolyMesh& mpm);

        //- Update for changes of mesh
        //  The base implementation is a no-op.
        virtual void movePoints(const polyMesh& mesh);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace externalIOObject
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
