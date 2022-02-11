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

#include "coupledAccelerationSource.H"
#include "fvMesh.H"
#include "fvMatrices.H"
#include "geometricOneField.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{
    defineTypeNameAndDebug(coupledAccelerationSource, 0);
    addToRunTimeSelectionTable(option, coupledAccelerationSource, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::fv::coupledAccelerationSource::coupledAccelerationSource
(
    const word& name,
    const word& modelType,
    const dictionary& dict,
    const fvMesh& mesh
)
:
    option(name, modelType, dict, mesh),
    time_(mesh.time()),
    accelerationName_(),
    omegaName_(),
    dOmegaDTName_(),
    UName_(coeffs_.getOrDefault<word>("U", "U")),
    g0_("g0", dimAcceleration, Zero)
{
    fieldNames_.setSize(1, UName_);
    applied_.setSize(1, false);

    if (mesh.time().foundObject<uniformDimensionedVectorField>("g"))
    {
        g0_ = mesh.time().lookupObject<uniformDimensionedVectorField>("g");
    }

    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::fv::coupledAccelerationSource::addSup
(
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    addSup<geometricOneField>(geometricOneField(), eqn, fieldi);
}


void Foam::fv::coupledAccelerationSource::addSup
(
    const volScalarField& rho,
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    addSup<volScalarField>(rho, eqn, fieldi);
}


bool Foam::fv::coupledAccelerationSource::read(const dictionary& dict)
{

    // read acceleration name
    if (!dict_.found("accelerationName"))
    {
        FatalError << "accelerationName not found in dict" << abort(FatalError);
    }
    accelerationName_ = dict_.get<word>("accelerationName");

    // read angular velocity name
    if (!dict_.found("omegaName"))
    {
        FatalError << "omegaName not found in dict" << abort(FatalError);
    }
    omegaName_ = dict_.get<word>("omegaName");

    // read angular acceleration name
    if (!dict_.found("dOmegaDTName"))
    {
        FatalError << "dOmegaDTName not found in dict" << abort(FatalError);
    }
    dOmegaDTName_ = dict_.get<word>("dOmegaDTName");

    // store in commDataLayer
    commDataLayer& data = commDataLayer::New(time_);

    data.storeObj(vector::zero,accelerationName_,commDataLayer::causality::in);
    data.storeObj(vector::zero,omegaName_,commDataLayer::causality::in);
    data.storeObj(vector::zero,dOmegaDTName_,commDataLayer::causality::in);

    return false;
}


// ************************************************************************* //
