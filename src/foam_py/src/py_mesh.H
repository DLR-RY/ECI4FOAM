/*---------------------------------------------------------------------------*\
            Copyright (c) 2022, Henning Scheufler
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
    Foam::pyInterp

Description

Author
    Henning Scheufler, all rights reserved.

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef py_mesh
#define py_mesh

// System includes
#include <pybind11/pybind11.h>
#include "fvMesh.H"
#include "polyMesh.H"
#include <vector>
#include <pybind11/stl.h>
// #include "py_geo_fields.H"



namespace Foam
{
// template<class Type>
// class volField<Type>;

class FvMesh {
    private:
        Time time_;
        fvMesh mesh_;


    public:

        FvMesh()
        :
            time_(".","."),
            mesh_
            (
                IOobject
                (
                    "region0",
                    time_.timeName(),
                    time_,
                    IOobject::MUST_READ
                ),
                false
            )
        {
            mesh_.init(true);   // Initialise all (lower levels and current)
        }

        const Time& time() const
        {
            return time_;
        }

        const fvMesh& mesh() const
        {
            return mesh_;
        }
 
};

}


void  AddPyMesh(pybind11::module& m);


#endif // py_dict  defined 