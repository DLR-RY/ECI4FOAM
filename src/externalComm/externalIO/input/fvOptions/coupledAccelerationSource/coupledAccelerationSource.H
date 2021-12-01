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
    Foam::fv::coupledAccelerationSource


Description
    Solid-body 6-DoF acceleration source from external data.

Usage
    Minimal example by using \c constant/fvOptions:
    \verbatim
    coupledAccelerationSource1
    {
        // Mandatory entries (unmodifiable)
        type             coupledAccelerationSource;

        accelerationName acc;
        omegaName omega;
        dOmegaDTName dOmegaDT;

        // Optional entries (unmodifiable)
        U                U;

        // Mandatory/Optional (inherited) entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property  | Description                            | Type  | Reqd | Dflt
      type      | Type name: coupledAccelerationSource | word  | yes  | -
      accelerationName | name of the acceleration vector | word  | yes  | -
      omegaName | name of the angular velocity vector | word  | yes  | -
      dOmegaDTName | name of the angular acceleration vector | word  | yes  | -
      U         | Name of operand velocity field         | word  | no   | U
    \endtable

    The inherited entries are elaborated in:
     - \link fvOption.H \endlink

SourceFiles
    coupledAccelerationSource.C
    coupledAccelerationSourceTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef coupledAccelerationSource_H
#define coupledAccelerationSource_H

#include "fvOption.H"
#include "dimensionedTypes.H"
#include "Time.H"
#include "commDataLayer.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                 Class coupledAccelerationSource Declaration
\*---------------------------------------------------------------------------*/

class coupledAccelerationSource
:
    public option
{
private: 

        const Time& time_;
protected:

    // Protected Data

        //- name of acceleration vector
        word accelerationName_;

        //- name of angular velocity vector
        word omegaName_;

        //- name of angular acceleration vector
        word dOmegaDTName_;

        //- Name of operand velocity field
        word UName_;

        //- Gravitational accelaration
        dimensionedVector g0_;


private:

    // Private Member Functions

        //- Source term to momentum equation
        template<class RhoFieldType>
        void addSup
        (
            const RhoFieldType& rho,
            fvMatrix<vector>& eqn,
            const label fieldi
        );


public:

    //- Runtime type information
    TypeName("coupledAccelerationSource");


    // Constructors

        //- Construct from components
        coupledAccelerationSource
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );

        //- No copy construct
        coupledAccelerationSource
        (
            const coupledAccelerationSource&
        ) = delete;

        //- No copy assignment
        void operator=(const coupledAccelerationSource&) = delete;


    //- Destructor
    virtual ~coupledAccelerationSource() = default;


    // Member Functions

        //- Add explicit contribution to momentum equation
        virtual void addSup
        (
            fvMatrix<vector>& eqn,
            const label fieldi
        );

        //- Add explicit contribution to compressible momentum equation
        virtual void addSup
        (
            const volScalarField& rho,
            fvMatrix<vector>& eqn,
            const label fieldi
        );


        //- Read source dictionary
        virtual bool read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "coupledAccelerationSourceTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
