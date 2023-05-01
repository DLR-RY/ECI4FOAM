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
    momentName_(),
    CofRName_(),
    CofR_()
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

    if (dict_.found("CofRName"))
    {

        CofRName_ = dict_.get<word>("CofRName");
        CofR_ = dict_.getOrDefault<vector>("CofR",Zero);
        data.storeObj(CofR_,CofRName_,commDataLayer::causality::in);
    }
    else
    {
        CofRName_ = "";
        CofR_ = dict_.getOrDefault<vector>("CofR",Zero);
    }

    return false;
}

bool Foam::externalIOObject::extForces::execute()
{
    commDataLayer& data = commDataLayer::New(time_);

    dictionary forceDict = dict_;
    if (CofRName_ != "")
    {
        CofR_ = data.getObj<vector>(CofRName_,commDataLayer::causality::in);
        forceDict.set("CofR",CofR_);
    }
    
    functionObjects::forces f("forces", time_, forceDict);
    f.calcForcesMoments();

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
