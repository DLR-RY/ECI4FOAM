/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
-------------------------------------------------------------------------------
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

#include "coupledRotationMotion.H"
#include "addToRunTimeSelectionTable.H"
#include "commDataLayer.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace solidBodyMotionFunctions
{
    defineTypeNameAndDebug(coupledRotationMotion, 0);
    addToRunTimeSelectionTable
    (
        solidBodyMotionFunction,
        coupledRotationMotion,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::solidBodyMotionFunctions::coupledRotationMotion::coupledRotationMotion
(
    const dictionary& SBMFCoeffs,
    const Time& runTime
)
:
    solidBodyMotionFunction(SBMFCoeffs, runTime),
    rollPitchYaw_(SBMFCoeffs_.get<vector>("rollPitchYaw")),
    CofR_(SBMFCoeffs_.get<vector>("CofR")),
    rollPitchYawName_(SBMFCoeffs_.get<word>("rollPitchYawName")),
    CofRName_(SBMFCoeffs_.get<word>("CofRName"))
{
    read(SBMFCoeffs);


}




// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::septernion
Foam::solidBodyMotionFunctions::coupledRotationMotion::transformation() const
{
    scalar t = time_.value();

    commDataLayer& data = commDataLayer::New(time_);

    vector rollPitchYawName =
        data.getObj<vector>(rollPitchYawName_,commDataLayer::causality::in);

    vector CofR =
        data.getObj<vector>(CofRName_,commDataLayer::causality::in);

    quaternion R(quaternion::XYZ, rollPitchYawName);
    septernion TR(septernion(-CofR)*R*septernion(CofR));

    DebugInFunction << "Time = " << t << " transformation: " << TR << endl;

    return TR;
}


bool Foam::solidBodyMotionFunctions::coupledRotationMotion::read
(
    const dictionary& SBMFCoeffs
)
{
    solidBodyMotionFunction::read(SBMFCoeffs);

    commDataLayer& data = commDataLayer::New(time_);

    data.storeObj(rollPitchYaw_,rollPitchYawName_,commDataLayer::causality::in);
    data.storeObj(CofR_,CofRName_,commDataLayer::causality::in);

    return true;
}


// ************************************************************************* //
