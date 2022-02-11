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

#include "addToRunTimeSelectionTable.H"
#include "extPatch.H"
#include "commDataLayer.H"
#include "volFields.H"
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{
    defineTypeNameAndDebug(extPatch, 0);
    addToRunTimeSelectionTable(externalIOObject, extPatch, dictionary);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::externalIOObject::extPatch::extPatch
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    externalIOObject(name, runTime, dict),
    outputName_(),
    fieldName_(dict.get<word>("fieldName")),
    patchName_(dict.get<word>("patchName")),
    mesh_
    (
        refCast<const fvMesh>
        (
            time_.lookupObject<objectRegistry>
            (
                dict.getOrDefault("region", polyMesh::defaultRegion)
            )
        )
    )
{
    read(dict);
    execute();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::externalIOObject::extPatch::read(const dictionary& dict)
{
    commDataLayer& data = commDataLayer::New(time_);
    if (!dict_.found("outputName"))
    {
        FatalError << "outputName not found in dict" << abort(FatalError);
    }
    outputName_ = dict_.get<word>("outputName");

    label patchI = mesh_.boundary().findPatchID(patchName_);
    if (patchI == -1)
    {
        FatalError << "patchName_ " << patchName_ << " not found in dict" << abort(FatalError);
    }

    data.storeObj(0.0,outputName_,commDataLayer::causality::out);
    return false;
}

bool Foam::externalIOObject::extPatch::execute()
{
    commDataLayer& data = commDataLayer::New(time_);
    
    scalar& out = data.getObj<scalar>(outputName_,commDataLayer::causality::out);


    const volScalarField& field = mesh_.lookupObject<volScalarField>(fieldName_);
    label patchI = mesh_.boundary().findPatchID(patchName_);

    out = gAverage(field.boundaryField()[patchI]);

    Info << name_  << " value := " << out << endl;

    return false;
}

bool Foam::externalIOObject::extPatch::write()
{
    return false;
}

// ************************************************************************* //
