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
    Foam::commDataLayer

Description
    Database for registeredObjects allows to store, update and get variables

Usage
    commDataLayer& data = commDataLayer::New(time_);

    // get scalar called a "var_a" from registry input
    scalar var_a = data.getObj<scalar>("var_a",commDataLayer::causality::in);

    // store scalar called a "var_a" in registry input
    data.storeObj(scalar(0.0),"var_a",commDataLayer::causality::in);

    // update 
    vector& var_a = data.getObj<vector>("var_a",commDataLayer::causality::out);
    var_a = 10;

    options for causality are:
    enum causality
    {
        in,
        out,
        parameter
    };

Author
    Henning Scheufler, DLR, all rights reserved.

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef commDataLayer_H
#define commDataLayer_H

#include "typeInfo.H"
#include "fvMesh.H"
#include "Time.H"
#include "regIOobject.H"
#include "registeredObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam 
{

class commDataLayer
:  
    public regIOobject // register to the regIO
{
public:

    //  Public enumeration

        //- similar to FMI standard
        enum causality
        {
            in,
            out,
            parameter
        };

private:

    const Time& time_;

    //- data required by openfoam
    objectRegistry in_;

    //- data required by external programm
    objectRegistry out_;

    //- are constant for the simulation
    //- similar to FMI standard 2.0+ internal or parameter
    objectRegistry parameters_; 


public:

    //- Runtime type information
    TypeName("commDataLayer");

    // Constructors
    commDataLayer(const Time& time);

    // Selectors
    static commDataLayer& New(const Time& time);

    // Member Functions 

    objectRegistry& getRegistry(commDataLayer::causality causa);


    const objectRegistry& getRegistry(commDataLayer::causality causa) const;


    void eraseObj(word objName,causality causa);


    template<class Type>
    void storeObj(const Type& T,word objName,causality causa);


    template<class Type>
    Type& getObj(word objName,causality causa);


    template<class Type>
    const Type& getObj(word objName,causality causa) const;

    // IO
        virtual bool writeData(Ostream&) const
        {
            return true;
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

#ifdef NoRepository
    #include "commDataLayerTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
