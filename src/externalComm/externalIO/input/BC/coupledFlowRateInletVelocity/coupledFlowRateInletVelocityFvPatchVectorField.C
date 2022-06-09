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

#include "coupledFlowRateInletVelocityFvPatchVectorField.H"
#include "addToRunTimeSelectionTable.H"
#include "volFields.H"
#include "one.H"
#include "commDataLayer.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::coupledFlowRateInletVelocityFvPatchVectorField::
coupledFlowRateInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(p, iF),
    flowRateName_(),
    rhoName_("rho"),
    rhoInlet_(0.0),
    flowInit_(0.0),
    volumetric_(false),
    extrapolateProfile_(false)
{}


Foam::coupledFlowRateInletVelocityFvPatchVectorField::
coupledFlowRateInletVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<vector>(p, iF, dict, false),
    rhoName_("rho"),
    rhoInlet_(dict.getOrDefault<scalar>("rhoInlet", -VGREAT)),
    flowInit_(0.0),
    volumetric_(false),
    extrapolateProfile_
    (
        dict.getOrDefault<Switch>("extrapolateProfile", false)
    )
{
    const Time& runTime = this->db().time();
    commDataLayer& data = commDataLayer::New(runTime);
    
    if (dict.found("volumetricFlowRate"))
    {
        volumetric_ = true;
        flowRateName_ = dict.get<word>("volumetricFlowRate");
        flowInit_ = dict.get<scalar>("vFlowInit");
        data.storeObj(flowInit_,flowRateName_,commDataLayer::causality::in);
    }
    else if (dict.found("massFlowRate"))
    {
        volumetric_ = false;
        flowRateName_ = dict.get<word>("massFlowRate");
        flowInit_ = dict.get<scalar>("mFlowInit");
        data.storeObj(flowInit_,flowRateName_,commDataLayer::causality::in);
        rhoName_ = dict.getOrDefault<word>("rho", "rho");
    }
    else
    {
        FatalIOErrorInFunction(dict)
            << "Please supply either 'volumetricFlowRate' or"
            << " 'massFlowRate' and 'rho'" << nl
            << exit(FatalIOError);
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


Foam::coupledFlowRateInletVelocityFvPatchVectorField::
coupledFlowRateInletVelocityFvPatchVectorField
(
    const coupledFlowRateInletVelocityFvPatchVectorField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<vector>(ptf, p, iF, mapper),
    flowRateName_(ptf.flowRateName_),
    rhoName_(ptf.rhoName_),
    rhoInlet_(ptf.rhoInlet_),
    flowInit_(ptf.flowInit_),
    volumetric_(ptf.volumetric_),
    extrapolateProfile_(ptf.extrapolateProfile_)
{}


Foam::coupledFlowRateInletVelocityFvPatchVectorField::
coupledFlowRateInletVelocityFvPatchVectorField
(
    const coupledFlowRateInletVelocityFvPatchVectorField& ptf
)
:
    fixedValueFvPatchField<vector>(ptf),
    flowRateName_(ptf.flowRateName_),
    rhoName_(ptf.rhoName_),
    rhoInlet_(ptf.rhoInlet_),
    flowInit_(ptf.flowInit_),
    volumetric_(ptf.volumetric_),
    extrapolateProfile_(ptf.extrapolateProfile_)
{}


Foam::coupledFlowRateInletVelocityFvPatchVectorField::
coupledFlowRateInletVelocityFvPatchVectorField
(
    const coupledFlowRateInletVelocityFvPatchVectorField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fixedValueFvPatchField<vector>(ptf, iF),
    flowRateName_(ptf.flowRateName_),
    rhoName_(ptf.rhoName_),
    rhoInlet_(ptf.rhoInlet_),
    flowInit_(ptf.flowInit_),
    volumetric_(ptf.volumetric_),
    extrapolateProfile_(ptf.extrapolateProfile_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class RhoType>
void Foam::coupledFlowRateInletVelocityFvPatchVectorField::updateValues
(
    const RhoType& rho
)
{
    const Time& runTime = this->db().time();
    commDataLayer& data = commDataLayer::New(runTime);

    const scalar flowRate =
        data.getObj<scalar>(flowRateName_,commDataLayer::causality::in);

    const vectorField n(patch().nf());

    if (extrapolateProfile_)
    {
        vectorField Up(this->patchInternalField());

        // Patch normal extrapolated velocity
        scalarField nUp(n & Up);

        // Remove the normal component of the extrapolate patch velocity
        Up -= nUp*n;

        // Remove any reverse flow
        nUp = min(nUp, scalar(0));

        const scalar estimatedFlowRate = -gSum(rho*(this->patch().magSf()*nUp));

        if (estimatedFlowRate/flowRate > 0.5)
        {
            nUp *= (mag(flowRate)/mag(estimatedFlowRate));
        }
        else
        {
            nUp -= ((flowRate - estimatedFlowRate)/gSum(rho*patch().magSf()));
        }

        // Add the corrected normal component of velocity to the patch velocity
        Up += nUp*n;

        // Correct the patch velocity
        this->operator==(Up);
    }
    else
    {
        const scalar avgU = -flowRate/gSum(rho*patch().magSf());
        operator==(avgU*n);
    }
}


void Foam::coupledFlowRateInletVelocityFvPatchVectorField::updateCoeffs()
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
            if (rhoInlet_ < 0)
            {
                FatalErrorInFunction
                    << "Did not find registered density field " << rhoName_
                    << " and no constant density 'rhoInlet' specified"
                    << exit(FatalError);
            }

            updateValues(rhoInlet_);
        }
    }

    fixedValueFvPatchVectorField::updateCoeffs();
}


void Foam::coupledFlowRateInletVelocityFvPatchVectorField::write(Ostream& os) const
{
    fvPatchField<vector>::write(os);
    if (!volumetric_)
    {
        os.writeEntry("massFlowRate", flowRateName_);
        os.writeEntry("mFlowInit", flowInit_);
        os.writeEntryIfDifferent<word>("rho", "rho", rhoName_);
        os.writeEntryIfDifferent<scalar>("rhoInlet", -VGREAT, rhoInlet_);
    }
    else
    {
        os.writeEntry("volumetricFlowRate", flowRateName_);
        os.writeEntry("vFlowInit", flowInit_);
    }
    os.writeEntry("extrapolateProfile", extrapolateProfile_);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
   makePatchTypeField
   (
       fvPatchVectorField,
       coupledFlowRateInletVelocityFvPatchVectorField
   );
}


// ************************************************************************* //
