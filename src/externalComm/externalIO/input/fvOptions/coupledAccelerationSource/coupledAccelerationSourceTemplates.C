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

#include "coupledAccelerationSource.H"
#include "fvMesh.H"
#include "fvMatrices.H"
#include "uniformDimensionedFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class RhoFieldType>
void Foam::fv::coupledAccelerationSource::addSup
(
    const RhoFieldType& rho,
    fvMatrix<vector>& eqn,
    const label fieldi
)
{
    commDataLayer& data = commDataLayer::New(time_);

    dimensionedVector Acceleration
    (
        "acceleration",
        dimAcceleration,
        data.getObj<vector>(accelerationName_,commDataLayer::causality::in)
    );

    dimensionedVector Omega
    (
        "Omega",
        dimensionSet(0, 0, -1, 0, 0),
        data.getObj<vector>(omegaName_,commDataLayer::causality::in)
    );

    dimensionedVector dOmegaDT
    (
        "dOmegaDT",
        dimensionSet(0, 0, -2, 0, 0),
        data.getObj<vector>(dOmegaDTName_,commDataLayer::causality::in)
    );



    // If gravitational force is present combine with the linear acceleration
    if (mesh_.time().foundObject<uniformDimensionedVectorField>("g"))
    {
        auto& g =
            mesh_.time().lookupObjectRef<uniformDimensionedVectorField>("g");

        const auto& hRef =
            mesh_.lookupObject<uniformDimensionedScalarField>("hRef");

        g = g0_ - Acceleration;

        dimensionedScalar ghRef
        (
            mag(g.value()) > SMALL
          ? g & (cmptMag(g.value())/mag(g.value()))*hRef
          : dimensionedScalar("ghRef", g.dimensions()*dimLength, 0)
        );

        mesh_.lookupObjectRef<volScalarField>("gh")
            = (g & mesh_.C()) - ghRef;

        mesh_.lookupObjectRef<surfaceScalarField>("ghf")
            = (g & mesh_.Cf()) - ghRef;
    }
    // ... otherwise include explicitly in the momentum equation
    else
    {
        eqn -= rho*Acceleration;
    }

    eqn -=
    (
        rho*(2*Omega ^ eqn.psi())         // Coriolis force
      + rho*(Omega ^ (Omega ^ mesh_.C())) // Centrifugal force
      + rho*(dOmegaDT ^ mesh_.C())        // Angular tabulatedAcceleration force
    );
}


// ************************************************************************* //
