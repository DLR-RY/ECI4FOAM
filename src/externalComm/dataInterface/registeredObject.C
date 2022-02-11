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

#include "registeredObject.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
template<class Type>
Foam::registeredObject<Type>::registeredObject
(
    const Type& obj,
    const word name,
    const objectRegistry& db
)
:    
    regIOobject
    (
        IOobject
        (
            name,
            name, // filename is irrelevant
            db,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false  //register object
        )
    ),
    name_(name),
    T_(obj)
{

}

template<class Type>
Foam::registeredObject<Type>::registeredObject
(
    Tuple2<word,Type> obj,
    const objectRegistry& db
)
:    
    regIOobject
    (
        IOobject
        (
            obj.first(),
            obj.first(), // filename is irrelevant
            db,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false  //register object
        )
    ),
    name_(obj.first()),
    T_(obj.second())
{

}

namespace Foam
{
    defineTemplateTypeNameWithName(registeredObject<bool>,"regBool");
    defineTemplateTypeNameWithName(registeredObject<word>,"regWord");
    defineTemplateTypeNameWithName(registeredObject<label>,"regLabel");
    defineTemplateTypeNameWithName(registeredObject<scalar>,"regScalar");
    defineTemplateTypeNameWithName(registeredObject<vector>,"regVector");
    defineTemplateTypeNameWithName(registeredObject<symmTensor>,"regSymmTensor");
    defineTemplateTypeNameWithName(registeredObject<sphericalTensor>,"regSphericalTensor");
    defineTemplateTypeNameWithName(registeredObject<tensor>,"regTensor");

    defineTemplateTypeNameWithName(registeredObject<Field<scalar>>,"regScalarField");
    defineTemplateTypeNameWithName(registeredObject<Field<vector>>,"regVectorField");
    defineTemplateTypeNameWithName(registeredObject<Field<symmTensor>>,"regSymmTensorField");
    defineTemplateTypeNameWithName(registeredObject<Field<sphericalTensor>>,"regSphericalTensorField");
    defineTemplateTypeNameWithName(registeredObject<Field<tensor>>,"regTensorField");
}

// base types
template class Foam::registeredObject<bool>; // FMI 2.0
template class Foam::registeredObject<Foam::word>; // FMI 2.0
template class Foam::registeredObject<Foam::label>; // FMI 2.0
template class Foam::registeredObject<Foam::scalar>; // FMI 2.0
template class Foam::registeredObject<Foam::vector>;
template class Foam::registeredObject<Foam::symmTensor>;
template class Foam::registeredObject<Foam::sphericalTensor>;
template class Foam::registeredObject<Foam::tensor>;

// Fields
template class Foam::registeredObject<Foam::Field<Foam::scalar>>;
template class Foam::registeredObject<Foam::Field<Foam::vector>>;
template class Foam::registeredObject<Foam::Field<Foam::symmTensor>>;
template class Foam::registeredObject<Foam::Field<Foam::sphericalTensor>>;
template class Foam::registeredObject<Foam::Field<Foam::tensor>>;




// ************************************************************************* //
