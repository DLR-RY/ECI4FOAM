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
    Foam::coupledFlowRateOutletVelocityFvPatchVectorField

Group
    grpOutletBoundaryConditions

Description
    Velocity outlet boundary condition which corrects the extrapolated velocity
    to match the specified flow rate.

    For a mass-based flux:
    - the flow rate should be provided in kg/s
    - if \c rho is "none" the flow rate is in m^3/s
    - otherwise \c rho should correspond to the name of the density field
    - if the density field cannot be found in the database, the user must
      specify the outlet density using the \c rhoOutlet entry

    For a volumetric-based flux:
    - the flow rate is in m^3/s

Usage
    \table
        Property     | Description             | Required    | Default value
        massFlowRate | mass flow rate [kg/s]   | no          |
        volumetricFlowRate | volumetric flow rate [m^3/s]| no |
        rho          | density field name      | no          | rho
        rhoOutlet    | outlet density          | no          |
    \endtable

    Example of the boundary condition specification for a volumetric flow rate:
    \verbatim
    <patchName>
    {
        type                flowRateOutletVelocity;
        volumetricFlowRate  0.2;
        flowRateName        mdot_out;
        value               uniform (0 0 0);
    }
    \endverbatim

    Example of the boundary condition specification for a mass flow rate:
    \verbatim
    <patchName>
    {
        type                flowRateOutletVelocity;
        massFlowRate        0.2;
        rhoOutlet           1.0;
        flowRateName        mdot_out;
        value               uniform (0 0 0);
    }
    \endverbatim

    The \c flowRate entry is a \c Function1 of time, see Foam::Function1Types.

Note
    - \c rhoOutlet is required for the case of a mass flow rate, where the
      density field is not available at start-up
    - The value is positive out of the domain (as an outlet)
    - May not work correctly for transonic outlets
    - Strange behaviour with potentialFoam since the U equation is not solved

See also
    Foam::fixedValueFvPatchField
    Foam::Function1Types
    Foam::flowRateInletVelocityFvPatchVectorField

SourceFiles
    coupledFlowRateOutletVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef coupledFlowRateOutletVelocityFvPatchVectorField_H
#define coupledFlowRateOutletVelocityFvPatchVectorField_H

#include "fixedValueFvPatchFields.H"
#include "Function1.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
           Class coupledFlowRateOutletVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class coupledFlowRateOutletVelocityFvPatchVectorField
:
    public fixedValueFvPatchVectorField
{
    // Private data

        //- Outlet integral flow rate
        word flowRateName_;

        //- Is volumetric?
        bool volumetric_;

        //- Name of the density field used to normalize the mass flux
        word rhoName_;

        //- Rho initialisation value (for start; if value not supplied)
        scalar rhoOutlet_;


    // Private member functions

        //- Update the patch values given the appropriate density type and value
        template<class RhoType>
        void updateValues(const RhoType& rho);


public:

   //- Runtime type information
   TypeName("coupledFlowRateOutletVelocity");


   // Constructors

        //- Construct from patch and internal field
        coupledFlowRateOutletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        coupledFlowRateOutletVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  coupledFlowRateOutletVelocityFvPatchVectorField
        //  onto a new patch
        coupledFlowRateOutletVelocityFvPatchVectorField
        (
            const coupledFlowRateOutletVelocityFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        coupledFlowRateOutletVelocityFvPatchVectorField
        (
            const coupledFlowRateOutletVelocityFvPatchVectorField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new coupledFlowRateOutletVelocityFvPatchVectorField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        coupledFlowRateOutletVelocityFvPatchVectorField
        (
            const coupledFlowRateOutletVelocityFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new coupledFlowRateOutletVelocityFvPatchVectorField(*this, iF)
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
