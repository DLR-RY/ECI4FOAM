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

#include "addToRunTimeSelectionTable.H"
#include "extFunction.H"
#include "forces.H"
#include "commDataLayer.H"
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{
    defineTypeNameAndDebug(extFunction, 0);
    addToRunTimeSelectionTable(externalIOObject, extFunction, dictionary);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::externalIOObject::extFunction::extFunction
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    externalIOObject(name, runTime, dict),
    func_(Function1<scalar>::New("function", dict)),
    varName_(dict.get<word>("varName"))
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::externalIOObject::extFunction::read(const dictionary& dict)
{
    commDataLayer& data = commDataLayer::New(time_);

    data.storeObj(func_->value(time_.value()),varName_,commDataLayer::causality::out);
    return false;
}

bool Foam::externalIOObject::extFunction::execute()
{
    commDataLayer& data = commDataLayer::New(time_);
 
    scalar& var = data.getObj<scalar>(varName_,commDataLayer::causality::out);
    var = func_->value(time_.value());

    return false;
}

bool Foam::externalIOObject::extFunction::write()
{
    return false;
}

// ************************************************************************* //
