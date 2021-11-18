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
#include "commDataLayer.H"


template<class Type>
void Foam::commDataLayer::storeObj(const Type& T,word objName,causality causa)
{
    auto& reg = getRegistry(causa);
    registeredObject<Type>* regObj =
        new registeredObject<Type>(T,objName,reg);
    reg.checkIn(regObj);
}

template<class Type>
Type& Foam::commDataLayer::getObj(word objName,causality causa)
{
    objectRegistry& reg = getRegistry(causa);
    auto& regObj = reg.lookupObjectRef<registeredObject<Type>>(objName);
    return regObj.ref();
}

template<class Type>
const Type& Foam::commDataLayer::getObj(word objName,causality causa) const
{
    const objectRegistry& reg = getRegistry(causa);
    const auto& regObj = reg.lookupObject<registeredObject<Type>>(objName);
    return regObj.ref();
}
