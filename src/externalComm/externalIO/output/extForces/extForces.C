/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2019 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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
#include "extForces.H"
#include "forces.H"
#include "commDataLayer.H"
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{
    defineTypeNameAndDebug(extForces, 0);
    addToRunTimeSelectionTable(externalIOObject, extForces, dictionary);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::externalIOObject::extForces::extForces
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    externalIOObject(name, runTime, dict),
    forceName_(),
    momentName_()
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::externalIOObject::extForces::read(const dictionary& dict)
{
    commDataLayer& data = commDataLayer::New(time_);
    if (!dict_.found("forceName"))
    {
        FatalError << "forceName not found in dict" << abort(FatalError);
    }
    forceName_ = dict_.get<word>("forceName");
    if (!dict_.found("momentName"))
    {
        FatalError << "momentName not found in dict" << abort(FatalError);
    }
    momentName_ = dict_.get<word>("momentName");
    data.storeObj(vector::zero,forceName_,commDataLayer::causality::out);
    data.storeObj(vector::zero,momentName_,commDataLayer::causality::out);
    return false;
}

bool Foam::externalIOObject::extForces::execute()
{
    commDataLayer& data = commDataLayer::New(time_);
    
    functionObjects::forces f("forces", time_, dict_);
    f.calcForcesMoment();

    vector& F = data.getObj<vector>(forceName_,commDataLayer::causality::out);
    F = f.forceEff();
    vector& M = data.getObj<vector>(momentName_,commDataLayer::causality::out);
    M = f.momentEff();

    return false;
}

bool Foam::externalIOObject::extForces::write()
{
    return false;
}

// ************************************************************************* //
