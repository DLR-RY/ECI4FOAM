/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019-2020 OpenCFD Ltd.
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

#include "coupledTranslationMotion.H"
#include "addToRunTimeSelectionTable.H"
#include "commDataLayer.H"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace solidBodyMotionFunctions
{
    defineTypeNameAndDebug(coupledTranslationMotion, 0);
    addToRunTimeSelectionTable
    (
        solidBodyMotionFunction,
        coupledTranslationMotion,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::solidBodyMotionFunctions::coupledTranslationMotion::coupledTranslationMotion
(
    const dictionary& SBMFCoeffs,
    const Time& runTime
)
:
    solidBodyMotionFunction(SBMFCoeffs, runTime),
    pos_(SBMFCoeffs.get<vector>("initPos")),
    relative_(SBMFCoeffs.getOrDefault<vector>("relative",Zero)),
    posName_(SBMFCoeffs.get<word>("posName"))

{
    read(SBMFCoeffs);


}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::septernion
Foam::solidBodyMotionFunctions::coupledTranslationMotion::transformation() const
{
    scalar t = time_.value();

    commDataLayer& data = commDataLayer::New(time_);

    vector displacement =
        data.getObj<vector>(posName_,commDataLayer::causality::in);

    displacement -= relative_;
    quaternion R(1);
    septernion TR(septernion(-displacement)*R);

    DebugInFunction << "Time = " << t << " transformation: " << TR << endl;

    return TR;
}


bool Foam::solidBodyMotionFunctions::coupledTranslationMotion::read
(
    const dictionary& SBMFCoeffs
)
{
    commDataLayer& data = commDataLayer::New(time_);

    data.storeObj(pos_,posName_,commDataLayer::causality::in);

    return true;
}


// ************************************************************************* //
