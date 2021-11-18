/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2015-2020 OpenCFD Ltd.
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

#include "coupledWallHeatFluxTemperatureFvPatchScalarField.H"
#include "addToRunTimeSelectionTable.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "physicoChemicalConstants.H"
#include "commDataLayer.H"

using Foam::constant::physicoChemical::sigma;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const Foam::Enum
<
    Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::operationMode
>
Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::operationModeNames
({
    { operationMode::fixedPower, "power" },
    { operationMode::fixedHeatTransferCoeff, "coefficient" },
});


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::
coupledWallHeatFluxTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(p, iF),
    temperatureCoupledBase
    (
        patch(),
        "undefined",
        "undefined",
        "undefined-K",
        "undefined-alpha"
    ),
    mode_(fixedPower),
    Q_(0),
    h_(0),
    Ta_(0)
{
    refValue() = 0;
    refGrad() = 0;
    valueFraction() = 1;
}


Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::
coupledWallHeatFluxTemperatureFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchScalarField(p, iF),
    temperatureCoupledBase(patch(), dict),
    mode_(operationModeNames.get("mode", dict)),
    Q_(0),
    flipQ_(dict.get<bool>("flipQ")),
    QName_(),
    h_(0),
    hName_(),
    Ta_(0),
    TaName_()
{
    const Time& runTime = this->db().time();
    commDataLayer& data = commDataLayer::New(runTime);
    
    switch (mode_)
    {
        case fixedPower:
        {
            QName_ = dict.get<word>("QName");
            Q_ = dict.get<scalar>("Qinit");
            data.storeObj(Q_,QName_,commDataLayer::causality::in);
            break;
        }
        case fixedHeatTransferCoeff:
        {
            h_ = dict.get<scalar>("hinit");;
            Ta_ = dict.get<scalar>("Tainit");;
            hName_ = dict.get<word>("hName");
            TaName_ = dict.get<word>("TaName");
            data.storeObj(Ta_,TaName_,commDataLayer::causality::in);
            data.storeObj(h_,hName_,commDataLayer::causality::in);
        }
    }

    fvPatchScalarField::operator=(scalarField("value", dict, p.size()));

    if (dict.found("refValue"))
    {
        // Full restart
        refValue() = scalarField("refValue", dict, p.size());
        refGrad() = scalarField("refGradient", dict, p.size());
        valueFraction() = scalarField("valueFraction", dict, p.size());
    }
    else
    {
        // Start from user entered data. Assume fixedValue.
        refValue() = *this;
        refGrad() = 0;
        valueFraction() = 1;
    }
}


Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::
coupledWallHeatFluxTemperatureFvPatchScalarField
(
    const coupledWallHeatFluxTemperatureFvPatchScalarField& rhs,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchScalarField(rhs, p, iF, mapper),
    temperatureCoupledBase(patch(), rhs),
    mode_(rhs.mode_),
    Q_(rhs.Q_),
    flipQ_(false),
    QName_(rhs.QName_),
    h_(rhs.h_),
    hName_(rhs.hName_),
    Ta_(rhs.Ta_),
    TaName_(rhs.TaName_)
{

}


Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::
coupledWallHeatFluxTemperatureFvPatchScalarField
(
    const coupledWallHeatFluxTemperatureFvPatchScalarField& rhs
)
:
    mixedFvPatchScalarField(rhs),
    temperatureCoupledBase(rhs),
    mode_(rhs.mode_),
    Q_(rhs.Q_),
    flipQ_(false),
    QName_(rhs.QName_),
    h_(rhs.h_),
    hName_(rhs.hName_),
    Ta_(rhs.Ta_),
    TaName_(rhs.TaName_)
{}


Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::
coupledWallHeatFluxTemperatureFvPatchScalarField
(
    const coupledWallHeatFluxTemperatureFvPatchScalarField& rhs,
    const DimensionedField<scalar, volMesh>& iF
)
:
    mixedFvPatchScalarField(rhs, iF),
    temperatureCoupledBase(patch(), rhs),
    mode_(rhs.mode_),
    Q_(rhs.Q_),
    flipQ_(false),
    QName_(rhs.QName_),
    h_(rhs.h_),
    hName_(rhs.hName_),
    Ta_(rhs.Ta_),
    TaName_(rhs.TaName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::autoMap
(
    const fvPatchFieldMapper& mapper
)
{
    mixedFvPatchScalarField::autoMap(mapper);

}


void Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::rmap
(
    const fvPatchScalarField& ptf,
    const labelList& addr
)
{
    mixedFvPatchScalarField::rmap(ptf, addr);

}


void Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const scalarField& Tp(*this);

    const scalarField valueFraction0(valueFraction());
    const scalarField refValue0(refValue());

    const Time& runTime = this->db().time();
    commDataLayer& data = commDataLayer::New(runTime);

    switch (mode_)
    {
        case fixedPower:
        {
            scalar heatPower = 
                data.getObj<scalar>(QName_,commDataLayer::causality::in);

            if (flipQ_)
            {
                heatPower *= -1;
            }
            Info << "heatPower " << heatPower << endl;

            refGrad() = (heatPower/gSum(patch().magSf()))/kappa(Tp);
            refValue() = 0;
            valueFraction() = 0;

            break;
        }
        case fixedHeatTransferCoeff:
        {
            h_ = data.getObj<scalar>(hName_,commDataLayer::causality::in);
            scalarField htcCoeff(patch().size(), h_ + VSMALL);

            scalarField hp(1/(1/htcCoeff));

            Ta_ = data.getObj<scalar>(TaName_,commDataLayer::causality::in);

            const scalarField kappaDeltaCoeffs
            (
                this->kappa(Tp)*patch().deltaCoeffs()
            );

            scalarField hpTa(hp*Ta_);

            refGrad() = 0;

            forAll(Tp, i)
            {
                refValue()[i] = (hpTa[i])/hp[i];
                valueFraction()[i] = hp[i]/(hp[i] + kappaDeltaCoeffs[i]);
            }

            break;
        }
    }

    mixedFvPatchScalarField::updateCoeffs();

    DebugInfo
        << patch().boundaryMesh().mesh().name() << ':' << patch().name() << ':'
        << internalField().name() << " :"
        << " heat transfer rate:" << gSum(kappa(Tp)*patch().magSf()*snGrad())
        << " wall temperature "
        << " min:" << gMin(*this)
        << " max:" << gMax(*this)
        << " avg:" << gAverage(*this) << nl;
}


void Foam::coupledWallHeatFluxTemperatureFvPatchScalarField::write
(
    Ostream& os
) const
{
    fvPatchScalarField::write(os);

    os.writeEntry("mode", operationModeNames[mode_]);
    temperatureCoupledBase::write(os);

    // if (Q_)
    // {
    //     Q_->writeData(os);
    // }
    // if (h_)
    // {
    //     h_->writeData(os);
    // }
    // if (Ta_)
    // {
    //     Ta_->writeData(os);
    // }

    switch (mode_)
    {
        case fixedHeatTransferCoeff:
        {
            break;
        }

        default:
            break;
    }
    refValue().writeEntry("refValue", os);
    refGrad().writeEntry("refGradient", os);
    valueFraction().writeEntry("valueFraction", os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makePatchTypeField
    (
        fvPatchScalarField,
        coupledWallHeatFluxTemperatureFvPatchScalarField
    );
}

// ************************************************************************* //
