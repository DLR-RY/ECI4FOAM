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
#include "extSensor.H"
#include "commDataLayer.H"
#include "volFields.H"
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{
    defineTypeNameAndDebug(extSensor, 0);
    addToRunTimeSelectionTable(externalIOObject, extSensor, dictionary);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::externalIOObject::extSensor::extSensor
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    externalIOObject(name, runTime, dict),
    sensorName_(),
    fieldName_(dict.get<word>("fieldName")),
    mesh_
    (
        refCast<const fvMesh>
        (
            time_.lookupObject<objectRegistry>
            (
                dict.getOrDefault("region", polyMesh::defaultRegion)
            )
        )
    ),
    sensorPosition_(),
    cellID_(-1)
{
    read(dict);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::externalIOObject::extSensor::read(const dictionary& dict)
{
    commDataLayer& data = commDataLayer::New(time_);
    if (!dict_.found("sensorName"))
    {
        FatalError << "sensorName not found in dict" << abort(FatalError);
    }
    sensorName_ = dict_.get<word>("sensorName");
    sensorPosition_ = dict_.get<vector>("sensorPosition");

    meshSearch ms(mesh_);

    cellID_ = ms.findCell(sensorPosition_);

    data.storeObj(0.0,sensorName_,commDataLayer::causality::out);
    return false;
}

bool Foam::externalIOObject::extSensor::execute()
{
    commDataLayer& data = commDataLayer::New(time_);
    
    scalar& sensor = data.getObj<scalar>(sensorName_,commDataLayer::causality::out);
    const volScalarField& field = mesh_.lookupObject<volScalarField>(fieldName_);
    sensor = field[cellID_];
    Info << "sensor value := " << sensor << endl;

    return false;
}

bool Foam::externalIOObject::extSensor::write()
{
    return false;
}

// ************************************************************************* //
