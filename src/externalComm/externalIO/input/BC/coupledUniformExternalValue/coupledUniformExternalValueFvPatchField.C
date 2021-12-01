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

#include "coupledUniformExternalValueFvPatchField.H"
#include "commDataLayer.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::coupledUniformExternalValueFvPatchField<Type>::coupledUniformExternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(p, iF),
    inputName_()
{}


template<class Type>
Foam::coupledUniformExternalValueFvPatchField<Type>::coupledUniformExternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const Field<Type>& fld
)
:
    fixedValueFvPatchField<Type>(p, iF, fld),
    inputName_()
{}


template<class Type>
Foam::coupledUniformExternalValueFvPatchField<Type>::coupledUniformExternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF, dict, false),
    inputName_(dict.get<word>("inputName"))
{
    // write input value in external coupling registry
    const Time& runTime = this->db().time();
    Type initValue = dict.get<Type>("initValue");
    commDataLayer& data = commDataLayer::New(runTime);
    data.storeObj(initValue,inputName_,commDataLayer::causality::in);

    this->evaluate();
}


template<class Type>
Foam::coupledUniformExternalValueFvPatchField<Type>::coupledUniformExternalValueFvPatchField
(
    const coupledUniformExternalValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(p, iF),
    inputName_(ptf.inputName_)
{
    if (mapper.direct() && !mapper.hasUnmapped())
    {
        // Use mapping instead of re-evaluation
        this->map(ptf, mapper);
    }
    else
    {
        // Evaluate since value not mapped
        this->evaluate();
    }
}


template<class Type>
Foam::coupledUniformExternalValueFvPatchField<Type>::coupledUniformExternalValueFvPatchField
(
    const coupledUniformExternalValueFvPatchField<Type>& ptf
)
:
    fixedValueFvPatchField<Type>(ptf),
    inputName_(ptf.inputName_)
{}


template<class Type>
Foam::coupledUniformExternalValueFvPatchField<Type>::coupledUniformExternalValueFvPatchField
(
    const coupledUniformExternalValueFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF),
    inputName_(ptf.inputName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::coupledUniformExternalValueFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& mapper
)
{
    fixedValueFvPatchField<Type>::autoMap(mapper);

}


template<class Type>
void Foam::coupledUniformExternalValueFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    fixedValueFvPatchField<Type>::rmap(ptf, addr);
}


template<class Type>
void Foam::coupledUniformExternalValueFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const Time& runTime = this->db().time();
    commDataLayer& data = commDataLayer::New(runTime);

    const Type& value =
        data.getObj<Type>(inputName_,commDataLayer::causality::in);

    fvPatchField<Type>::operator==(value);
    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void Foam::coupledUniformExternalValueFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    this->writeEntry("value", os);
    os.writeEntry("inputName", inputName_);
}


// ************************************************************************* //
