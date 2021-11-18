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

#include "externalIOObject.H"
#include "dictionary.H"
#include "dlLibraryTable.H"
#include "Time.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineDebugSwitchWithName(externalIOObject::externalIOObject, "externalIOObject", 0);
    defineRunTimeSelectionTable(externalIOObject::externalIOObject, dictionary);
}



// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

Foam::word Foam::externalIOObject::externalIOObject::scopedName(const word& name) const
{
    return name_ + ":" + name;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


Foam::externalIOObject::externalIOObject::externalIOObject
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    name_(name),
    time_(runTime),
    dict_(dict)
{}

// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::externalIOObject::externalIOObject> Foam::externalIOObject::externalIOObject::New
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
{
    const word externalIOType(dict.get<word>("type"));

    DebugInfo
        << "Selecting externalIO " << externalIOType << endl;

    
    // Load any additional libraries
    {
        const auto finder =
            dict.csearchCompat("libs", {{"externalIOObjectLibs", 1612}});

        if (finder.found())
        {
            runTime.libs().open
            (
                dict,
                finder.ref().keyword(),
                dictionaryConstructorTablePtr_
            );
        }
    }

    // This is the simplified version without compatibility messages
    runTime.libs().open
    (
        dict,
        "libs",
        dictionaryConstructorTablePtr_
    );

    if (!dictionaryConstructorTablePtr_)
    {
        FatalErrorInFunction
            << "Cannot load function type " << externalIOType << nl << nl
            << "Table of externalIOObjects is empty" << endl
            << exit(FatalError);
    }

    auto cstrIter = dictionaryConstructorTablePtr_->cfind(externalIOType);

    if (!cstrIter.found())
    {
        FatalIOErrorInLookup
        (
            dict,
            "externalIO",
            externalIOType,
            *dictionaryConstructorTablePtr_
        ) << abort(FatalIOError);

    }


    return autoPtr<externalIOObject>(cstrIter()(name, runTime, dict));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::word& Foam::externalIOObject::externalIOObject::name() const
{
    return name_;
}


bool Foam::externalIOObject::externalIOObject::read(const dictionary& dict)
{
    return true;
}


bool Foam::externalIOObject::externalIOObject::execute(const label)
{
    return true;
}


bool Foam::externalIOObject::externalIOObject::end()
{
    return true;
}


bool Foam::externalIOObject::externalIOObject::adjustTimeStep()
{
    return false;
}


bool Foam::externalIOObject::externalIOObject::filesModified() const
{
    return false;
}


void Foam::externalIOObject::externalIOObject::updateMesh(const mapPolyMesh&)
{}


void Foam::externalIOObject::externalIOObject::movePoints(const polyMesh&)
{}


// // * * * * * * * * * * * * unavailableexternalIOObject * * * * * * * * * * * * //

// Foam::functionObjects::externalIOObject::unavailableexternalIOObject::unavailableexternalIOObject
// (
//     const word& name
// )
// :
//     externalIOObject(name)
// {}


// void Foam::functionObjects::externalIOObject::unavailableexternalIOObject::carp
// (
//     std::string message
// ) const
// {
//     FatalError
//         << "####" << nl
//         << "    " << type() << " not available" << nl
//         << "####" << nl;

//     if (message.size())
//     {
//         FatalError
//             << message.c_str() << nl;
//     }

//     FatalError
//         << exit(FatalError);
// }


// bool Foam::functionObjects::externalIOObject::unavailableexternalIOObject::execute()
// {
//     return true;
// }


// bool Foam::functionObjects::externalIOObject::unavailableexternalIOObject::write()
// {
//     return true;
// }


// ************************************************************************* //
