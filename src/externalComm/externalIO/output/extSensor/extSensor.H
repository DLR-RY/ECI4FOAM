/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2018-2019 OpenFOAM Foundation
    Copyright (C) 2020 OpenCFD Ltd.
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

Class
    Foam::externalIOObject::extSensor

Group
    grpFieldFunctionObjects

Description
    Computes the natural extSensorarithm of an input \c volScalarField.

    \f[
        f = s \ln(max(f_0, a)) + t
    \f]

    where
    \vartable
      f     | Output volScalarField
      f_0   | Input volScalarField
      \ln   | Natural extSensorarithm operator
      a     | Clip scalar
      s     | Scaling factor
      t     | Offset factor
    \endvartable

    \table
      Operand       | Type           | Location
      input         | volScalarField | $FOAM_CASE/\<time\>/\<inpField\>
      output file   | -              | -
      output field  | volScalarField | $FOAM_CASE/\<time\>/\<outField\>
    \endtable

Usage
    Minimal example by using \c system/controlDict.functions:
    \verbatim
    extSensor1
    {
        // Mandatory entries (unmodifiable)
        type            extSensor;
        libs            (fieldFunctionObjects);

        // Mandatory (inherited) entry (runtime modifiable)
        field           <inpField>;

        // Optional entries (runtime modifiable)
        clip            1e-3;
        checkDimensions false;
        scale           1.0;
        offset          0.0;

        // Optional (inherited) entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property     | Description                        | Type | Req'd | Dflt
      type         | Type name: extSensor                     | word |  yes  | -
      libs         | Library name: fieldFunctionObjects | word |  yes  | -
      field        | Name of the operand field          | word |  yes  | -
      clip         | Value to clip the operand field values <!--
                 --> to prevent zero or negative input  | scalar | no  | SMALL
      checkDimensions | Flag to check dimensions of the operand field <!--
                                                    --> | bool   | no  | true
      scale        | Scaling factor - \c s above        | scalar | no  | 1.0
      offset       | Offset factor - \c t above         | scalar | no  | 0.0
    \endtable

    The inherited entries are elaborated in:
     - \link functionObject.H \endlink
     - \link fieldExpression.H \endlink

    Minimal example by using the \c postProcess utility:
    \verbatim
        postProcess -func "extSensor(<inpField>)" -scale 1.0 -offset 0.0
    \endverbatim

Note
    - Performs \f$\ln(max(x, a))\f$ where \f$x\f$ is a \c volScalarField, and
    \f$a\f$ a clip scalar, equals to \c SMALL by default. This prevents zero or
    negative input \f$x\f$, hence the domain error in the natural extSensorarithm.
    - Dimension checking can optionally be suspended if \f$x\f$ is dimensioned.

See also
    - Foam::functionObject
    - Foam::externalIOObject::fieldExpression
    - Foam::externalIOObject::fvMeshFunctionObject
    - ExtendedCodeGuide::externalIOObject::field::extSensor

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
