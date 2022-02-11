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

\*---------------------------------------------------------------------------*/

#include "commDataLayer.H"
#include "IOmanip.H"

namespace Foam
{
    defineTypeNameAndDebug(commDataLayer, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
Foam::commDataLayer::commDataLayer(const Time& time)
:    
    regIOobject
    (
        IOobject
        (
            "commDataLayer",
            time.timeName(),
            time,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false  //register object
        )
    ),
    time_(time),
    in_
    (
        IOobject
        (
            "in",
            time.timeName(),
            time,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        )
    ),
    out_
    (
        IOobject
        (
            "out",
            time.timeName(),
            time,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        )
    ),
    parameters_
    (
        IOobject
        (
            "parameters",
            time.timeName(),
            time,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        )
    )

{

}

Foam::commDataLayer& Foam::commDataLayer::New(const Time& time)
{
    commDataLayer* ptr = time.getObjectPtr<commDataLayer>
    (
        commDataLayer::typeName
    );

    if (!ptr)
    {
        ptr = new commDataLayer(time);

        ptr->store();
    }

    return *ptr;
}

Foam::objectRegistry&
Foam::commDataLayer::getRegistry(commDataLayer::causality causa)
{
    if (causa == commDataLayer::causality::in)
    {
        return in_;
    }
    else if (causa == commDataLayer::causality::out)
    {
        return out_;
    }
    else if (causa == commDataLayer::causality::parameter)
    {
        return parameters_;
    }   
    else
    {
        FatalError << "only the options in out parameter"
                    << abort(FatalError);
        return parameters_; 
    }
}   

const Foam::objectRegistry& 
Foam::commDataLayer::getRegistry(commDataLayer::causality causa) const
{
    if (causa == commDataLayer::causality::in)
    {
        return in_;
    }
    else if (causa == commDataLayer::causality::out)
    {
        return out_;
    }
    else if (causa == commDataLayer::causality::parameter)
    {
        return parameters_;
    }   
    else
    {
        FatalError << "only the options in out parameter"
                    << abort(FatalError);
        return parameters_;
    }
}


void Foam::commDataLayer::eraseObj(word objName,causality causa)
{
    auto& reg = getRegistry(causa);
    reg.erase(objName);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// ************************************************************************* //
