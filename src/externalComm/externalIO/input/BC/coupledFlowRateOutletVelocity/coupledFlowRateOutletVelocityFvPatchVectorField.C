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

#include "coupledFlowRateOutletVelocityFvPatchVectorField.H"
#include "volFields.H"
#include "one.H"
#include "addToRunTimeSelectionTable.H"
#include "commDataLayer.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::coupledFlowRateOutletVelocityFvPatchVectorField::
coupledFlowRateOutletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(p, iF),
    flowRateName_(),
    volumetric_(false),
    rhoName_("rho"),
    rhoOutlet_(0.0)
{}


Foam::coupledFlowRateOutletVelocityFvPatchVectorField::
coupledFlowRateOutletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<vector>(p, iF, dict, false),
    rhoOutlet_(dict.getOrDefault<scalar>("rhoOutlet", -VGREAT))
{
    const Time& runTime = this->db().time();
    commDataLayer& data = commDataLayer::New(runTime);
    
    if (dict.found("volumetricFlowRate"))
    {
        volumetric_ = true;
        flowRateName_ = dict.get<word>("volumetricFlowRate");
        scalar mdotInit = dict.get<scalar>("vFlowInit");
        data.storeObj(mdotInit,flowRateName_,commDataLayer::causality::in);
    }
    else if (dict.found("massFlowRate"))
    {
        volumetric_ = false;
        flowRateName_ = dict.get<word>("massFlowRate");
        scalar vdotInit = dict.get<scalar>("mFlowInit");
        data.storeObj(vdotInit,flowRateName_,commDataLayer::causality::in);
        rhoName_ = dict.getOrDefault<word>("rho", "rho");
    }
    else
    {
        FatalIOErrorInFunction(dict)
            << "Please supply either 'volumetricFlowRate' or"
            << " 'massFlowRate' and 'rho'" << exit(FatalIOError);
    }

    // Value field require if mass based
    if (dict.found("value"))
    {
        fvPatchField<vector>::operator=
        (
            vectorField("value", dict, p.size())
        );
    }
    else
    {
        evaluate(Pstream::commsTypes::blocking);
    }
}


Foam::coupledFlowRateOutletVelocityFvPatchVectorField::
coupledFlowRateOutletVelocityFvPatchVectorField
(
    const coupledFlowRateOutletVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
    flowRateName_(ptf.flowRateName_),
    volumetric_(ptf.volumetric_),
    rhoName_(ptf.rhoName_),
    rhoOutlet_(ptf.rhoOutlet_)
{}


Foam::coupledFlowRateOutletVelocityFvPatchVectorField::
coupledFlowRateOutletVelocityFvPatchVectorField
(
    const coupledFlowRateOutletVelocityFvPatchVectorField& ptf
)
:
    fixedValueFvPatchField<vector>(ptf),
    flowRateName_(ptf.flowRateName_),
    volumetric_(ptf.volumetric_),
    rhoName_(ptf.rhoName_),
    rhoOutlet_(ptf.rhoOutlet_)
{}


Foam::coupledFlowRateOutletVelocityFvPatchVectorField::
coupledFlowRateOutletVelocityFvPatchVectorField
(
    const coupledFlowRateOutletVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(ptf, iF),
    flowRateName_(ptf.flowRateName_),
    volumetric_(ptf.volumetric_),
    rhoName_(ptf.rhoName_),
    rhoOutlet_(ptf.rhoOutlet_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class RhoType>
void Foam::coupledFlowRateOutletVelocityFvPatchVectorField::updateValues
(
    const RhoType& rho
)
{
    const Time& runTime = this->db().time();
    commDataLayer& data = commDataLayer::New(runTime);

    const vectorField n(patch().nf());

    // Extrapolate patch velocity
    vectorField Up(this->patchInternalField());

    // Patch normal extrapolated velocity
    scalarField nUp(n & Up);

    // Remove the normal component of the extrapolate patch velocity
    Up -= nUp*n;

    // Remove any reverse flow
    nUp = max(nUp, scalar(0));

    const scalar flowRate =
        data.getObj<scalar>(flowRateName_,commDataLayer::causality::in);
    const scalar estimatedFlowRate = gSum(rho*(this->patch().magSf()*nUp));

    if (estimatedFlowRate/flowRate > 0.5)
    {
        nUp *= (mag(flowRate)/mag(estimatedFlowRate));
    }
    else
    {
        nUp += ((flowRate - estimatedFlowRate)/gSum(rho*patch().magSf()));
    }

    // Add the corrected normal component of velocity to the patch velocity
    Up += nUp*n;

    // Correct the patch velocity
    this->operator==(Up);
}


void Foam::coupledFlowRateOutletVelocityFvPatchVectorField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    if (volumetric_ || rhoName_ == "none")
    {
        updateValues(one{});
    }
    else
    {
        // Mass flow-rate
        if (db().foundObject<volScalarField>(rhoName_))
        {
            const fvPatchField<scalar>& rhop =
                patch().lookupPatchField<volScalarField, scalar>(rhoName_);

            updateValues(rhop);
        }
        else
        {
            // Use constant density
            if (rhoOutlet_ < 0)
            {
                FatalErrorInFunction
                    << "Did not find registered density field " << rhoName_
                    << " and no constant density 'rhoOutlet' specified"
                    << exit(FatalError);
            }

            updateValues(rhoOutlet_);
        }
    }

    fixedValueFvPatchVectorField::updateCoeffs();
}


void Foam::coupledFlowRateOutletVelocityFvPatchVectorField::write(Ostream& os) const
{
    fvPatchField<vector>::write(os);
    writeEntry("volumetricFlowRate", os);
    if (!volumetric_)
    {
        os.writeEntryIfDifferent<word>("rho", "rho", rhoName_);
        os.writeEntryIfDifferent<scalar>("rhoOutlet", -VGREAT, rhoOutlet_);
    }
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
   makePatchTypeField
   (
       fvPatchVectorField,
       coupledFlowRateOutletVelocityFvPatchVectorField
   );
}


// ************************************************************************* //
