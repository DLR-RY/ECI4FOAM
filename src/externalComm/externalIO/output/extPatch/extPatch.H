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

Class
    Foam::externalIOObject::extPatch

Group
    grpFieldFunctionObjects

Description
    Computes the natural extPatcharithm of an input \c volScalarField.

    \f[
        f = s \ln(max(f_0, a)) + t
    \f]

    where
    \vartable
      f     | Output volScalarField
      f_0   | Input volScalarField
      \ln   | Natural extPatcharithm operator
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
    Minimal example by using \c system/externalCouplingDict:
    \verbatim
    extPatch1
    {
        // Mandatory entries (unmodifiable)
        type            extPatch;
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
      type         | Type name: extPatch                     | word |  yes  | -
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
        postProcess -func "extPatch(<inpField>)" -scale 1.0 -offset 0.0
    \endverbatim

Note
    - Performs \f$\ln(max(x, a))\f$ where \f$x\f$ is a \c volScalarField, and
    \f$a\f$ a clip scalar, equals to \c SMALL by default. This prevents zero or
    negative input \f$x\f$, hence the domain error in the natural extPatcharithm.
    - Dimension checking can optionally be suspended if \f$x\f$ is dimensioned.

See also
    - Foam::functionObject
    - Foam::externalIOObject::fieldExpression
    - Foam::externalIOObject::fvMeshFunctionObject
    - ExtendedCodeGuide::externalIOObject::field::extPatch

SourceFiles
    extPatch.C

\*---------------------------------------------------------------------------*/

#ifndef externalIOObject_extPatch_H
#define externalIOObject_extPatch_H

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
                          Class extPatch Declaration
\*---------------------------------------------------------------------------*/

class extPatch
:
    public externalIOObject
{
    // Private Data

    word outputName_;

    word fieldName_;

    word patchName_;

    const fvMesh& mesh_;


public:

    //- Runtime type information
    TypeName("extPatch");


    // Constructors

        //- Construct from Time and dictionary
        extPatch
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        extPatch(const extPatch&) = delete;

        //- No copy assignment
        void operator=(const extPatch&) = delete;


    //- Destructor
    virtual ~extPatch() = default;


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
