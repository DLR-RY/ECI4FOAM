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
#include "registeredObject.H"
#include "scalar.H"
#include "vector.H"


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

namespace Foam
{
    defineTemplateTypeNameWithName(registeredObject<scalar>,"regScalar");
    defineTemplateTypeNameWithName(registeredObject<vector>,"regVector");
    defineTemplateTypeNameWithName(registeredObject<tensor>,"regTensor");
    defineTemplateTypeNameWithName(registeredObject<symmTensor>,"regSymmTensor");
    defineTemplateTypeNameWithName(registeredObject<sphericalTensor>,"regSphericalTensor");
}

template class Foam::registeredObject<Foam::scalar>;
template class Foam::registeredObject<Foam::vector>;
template class Foam::registeredObject<Foam::symmTensor>;
template class Foam::registeredObject<Foam::sphericalTensor>;
template class Foam::registeredObject<Foam::tensor>;



// ************************************************************************* //
