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
    Foam::coupledWallHeatFluxTemperatureFvPatchScalarField

Group
    grpThermoBoundaryConditions grpWallBoundaryConditions

Description
    This boundary condition applies a heat flux condition to temperature
    on an external wall in one of three modes:

      - fixed power: supply Q
      - fixed heat flux: supply q
      - fixed heat transfer coefficient: supply h and Ta

    where:
    \vartable
        Q  | Power [W]
        q  | Heat flux [W/m^2]
        h  | Heat transfer coefficient [W/m^2/K]
        Ta | Ambient temperature [K]
    \endvartable

    For heat transfer coefficient mode optional thin thermal layer resistances
    can be specified through thicknessLayers and kappaLayers entries.

    The thermal conductivity \c kappa can either be retrieved from various
    possible sources, as detailed in the class temperatureCoupledBase.

    The ambient temperature Ta is specified as a Foam::Function1 of time but
    uniform in space.

Usage
    \table
    Property    | Description                              | Required | Default
    mode        | 'power', 'flux' or 'coefficient'         | yes |
    Q           | Power [W]                                | for mode 'power' |
    q           | Heat flux [W/m^2]                        | for mode 'flux' |
    QName      | input Name of the heatflux                | for mode 'power' |
    h           | Heat transfer coefficient [W/m^2/K]      | for mode 'coefficient' |
    hName_      | input Name of the coefficient            | for mode 'coefficient' |
    Ta          | Ambient temperature [K]                  | for mode 'coefficient' |
    TaName      | input Name of the ambient temperature    | for mode 'coefficient' |
    thicknessLayers | Layer thicknesses [m]                | no  |
    kappaLayers | Layer thermal conductivities [W/m/K]     | no  |
    relaxation  | Relaxation for the wall temperature      | no  | 1
    emissivity  | Surface emissivity for radiative flux to ambient | no | 0
    qr          | Name of the radiative field              | no  | none
    qrRelaxation | Relaxation factor for radiative field   | no  | 1
    kappaMethod | Inherited from temperatureCoupledBase    | inherited |
    kappa       | Inherited from temperatureCoupledBase    | inherited |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            externalWallHeatFluxTemperature;

        mode            coefficient;

        Ta              constant 300.0;
        h               constant 10.0;
        thicknessLayers (0.1 0.2 0.3 0.4);
        kappaLayers     (1 2 3 4);

        kappaMethod     fluidThermo;

        value           $internalField;
    }
    \endverbatim

Note
    Quantities that are considered "global" (eg, power, ambient temperature)
    can be specified as Function1 types.
    Quantities that may have local variations (eg, htc, heat-flux)
    can be specified as PatchFunction1 types.

See also
    Foam::temperatureCoupledBase
    Foam::mixedFvPatchScalarField

SourceFiles
    coupledWallHeatFluxTemperatureFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef coupledWallHeatFluxTemperatureFvPatchScalarField_H
#define coupledWallHeatFluxTemperatureFvPatchScalarField_H

#include "mixedFvPatchFields.H"
#include "temperatureCoupledBase.H"
#include "PatchFunction1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
      Class coupledWallHeatFluxTemperatureFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class coupledWallHeatFluxTemperatureFvPatchScalarField
:
    public mixedFvPatchScalarField,
    public temperatureCoupledBase
{
public:

    // Public Data

        //- Operation mode enumeration
        enum operationMode
        {
            fixedPower,             //!< Heat power [W]
            fixedHeatTransferCoeff, //!< Heat transfer coefficient [W/m^2/K]
        };

        static const Enum<operationMode> operationModeNames;


private:

    // Private Data

        //- Operation mode
        operationMode mode_;

        //- Heat power [W]
        scalar Q_;

        bool flipQ_;

        //- Q name in dataInterface
        word QName_;

        //- Heat transfer coefficient [W/m2K]
        scalar h_;

        //- h name in dataInterface
        word hName_;

        //- Ambient temperature [K]
        scalar Ta_;

        //- Ta name in dataInterface
        word TaName_;


public:

    //- Runtime type information
    TypeName("coupledWallHeatFluxTemperature");


    // Constructors

        //- Construct from patch and internal field
        coupledWallHeatFluxTemperatureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        coupledWallHeatFluxTemperatureFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  coupledWallHeatFluxTemperatureFvPatchScalarField
        //  onto a new patch
        coupledWallHeatFluxTemperatureFvPatchScalarField
        (
            const coupledWallHeatFluxTemperatureFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        coupledWallHeatFluxTemperatureFvPatchScalarField
        (
            const coupledWallHeatFluxTemperatureFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new coupledWallHeatFluxTemperatureFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        coupledWallHeatFluxTemperatureFvPatchScalarField
        (
            const coupledWallHeatFluxTemperatureFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new coupledWallHeatFluxTemperatureFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Allow manipulation of the boundary values
            virtual bool fixesValue() const
            {
                return false;
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchScalarField&,
                const labelList&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        // I-O

            //- Write
            void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
