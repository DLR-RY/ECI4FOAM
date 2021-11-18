/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
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
    Foam::solidBodyMotionFunctions::coupledTranslationMotion

Description
    Tabulated 6DoF motion function.

    Obtained by interpolating tabulated data for surge (x-translation),
    sway (y-translation), heave (z-translation), roll (rotation about x),
    pitch (rotation about y) and yaw (rotation about z).

SourceFiles
    coupledTranslationMotion.C

\*---------------------------------------------------------------------------*/

#ifndef coupledTranslationMotion_H
#define coupledTranslationMotion_H

#include "solidBodyMotionFunction.H"
#include "primitiveFields.H"
#include "Vector2D.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace solidBodyMotionFunctions
{

/*---------------------------------------------------------------------------*\
                          Class coupledTranslationMotion Declaration
\*---------------------------------------------------------------------------*/

class coupledTranslationMotion
:
    public solidBodyMotionFunction
{
    // Private data

        //- postion vector
        vector pos_;

        //- name ofpostion vector
        word posName_;

    // Private Member Functions

        //- No copy construct
        coupledTranslationMotion(const coupledTranslationMotion&) = delete;

        //- No copy assignment
        void operator=(const coupledTranslationMotion&) = delete;


public:

    //- Runtime type information
    TypeName("coupledTranslationMotion");


    // Constructors

        //- Construct from components
        coupledTranslationMotion
        (
            const dictionary& SBMFCoeffs,
            const Time& runTime
        );

        //- Construct and return a clone
        virtual autoPtr<solidBodyMotionFunction> clone() const
        {
            return autoPtr<solidBodyMotionFunction>
            (
                new coupledTranslationMotion
                (
                    SBMFCoeffs_,
                    time_
                )
            );
        }


    //- Destructor
    virtual ~coupledTranslationMotion() = default;


    // Member Functions

        //- Return the solid-body motion transformation septernion
        virtual septernion transformation() const;

        //- Update properties from given dictionary
        virtual bool read(const dictionary& SBMFCoeffs);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace solidBodyMotionFunctions
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
