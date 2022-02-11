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

Class
    Foam::externalIOObject::extSensor

Description
    auto a single probes data

Usage
    Minimal example by using \c system/externalCouplingDict:
    \verbatim
    extSensor
    {
        type extSensor;
        sensorName Tout;
        fieldName T;
        sensorPosition (-0.01 0 0);
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Req'd | Dflt
      type         | Type name: extFunction                     | word |  yes  | -
      fieldName     | name of the measurement field | word |  yes  | -
      sensorName      | name of the sensor         | word |  yes  | -
      sensorPosition      | position of the sensor        | vector |  yes  | -
    \endtable

SourceFiles
    extSensor.C

\*---------------------------------------------------------------------------*/

#ifndef externalIOObject_extSensor_H
#define externalIOObject_extSensor_H

#include "externalIOObject.H"
#include "meshSearch.H"
#include "vector.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace externalIOObject
{

/*---------------------------------------------------------------------------*\
                          Class extSensor Declaration
\*---------------------------------------------------------------------------*/

class extSensor
:
    public externalIOObject
{
    // Private Data

    word sensorName_;

    word fieldName_;

    const fvMesh& mesh_;

    vector sensorPosition_;

    label cellID_;


public:

    //- Runtime type information
    TypeName("extSensor");


    // Constructors

        //- Construct from Time and dictionary
        extSensor
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        extSensor(const extSensor&) = delete;

        //- No copy assignment
        void operator=(const extSensor&) = delete;


    //- Destructor
    virtual ~extSensor() = default;


    // Member Functions

        //- Read the randomise data
        virtual bool read(const dictionary&);

        virtual bool execute();

        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace externalIOObject
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
