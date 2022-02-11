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

#include "externalIOObjectList.H"
#include "Time.H"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


Foam::externalIOObjectList::externalIOObjectList
(
    const Time& runTime,
    const bool execution
)
:
    PtrList<externalIOObject::externalIOObject>(),
    parentDict_
    (
        IOobject
        (
            "externalCouplingDict",
            runTime.system(),
            runTime,
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        )
    ),
    time_(runTime)
{
    if (!parentDict_.found("externalFunctions"))
    {   
        FatalError << "dict entry externalFunctions not found" << abort(FatalError);
    }

    const dictionary& funcDict = parentDict_.subDict("externalFunctions");

     for (const entry& dEntry : funcDict)
    {
        const word& key = dEntry.keyword();

        if (!dEntry.isDict())
        {
            FatalIOErrorInFunction(funcDict)
                << "Entry " << key << " is not a dictionary" << endl
                << exit(FatalError);
        }

        const dictionary& subdict = dEntry.dict();

        this->append(externalIOObject::externalIOObject::New(key,time_, subdict));

    }
}

Foam::externalIOObjectList::~externalIOObjectList()
{
    this->clear();
    // loopProfiling_.reset(nullptr);

    // forAllReverse(controlDict_.watchIndices(), i)
    // {
    //     fileHandler().removeWatch(controlDict_.watchIndices()[i]);
    // }

    // // Destroy function objects first


    // // Clean up profiling
    // profiling::stop(*this);

    // // Ensure all owned objects are also cleaned up now
    // objectRegistry::clear();
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// ************************************************************************* //
