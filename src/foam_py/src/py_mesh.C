/*---------------------------------------------------------------------------*\
            Copyright (c) 20212, Henning Scheufler
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

#include "py_dict.H"
#include "fvMesh.H"
#include "polyMesh.H"
#include <vector>
#include <pybind11/stl.h>




class FoamMesh {
    private:
        Foam::Time time_;
        Foam::fvMesh mesh_;


    public:

        FoamMesh()
        :
            time_(".","."),
            mesh_
            (
                Foam::IOobject
                (
                    "region0",
                    time_.timeName(),
                    time_,
                    Foam::IOobject::MUST_READ
                ),
                false
            )
        {
            mesh_.init(true);   // Initialise all (lower levels and current)
        }

        double time()
        {
            return mesh_.time().timeOutputValue();
        }

        // void time(double t)
        // {
        //     return mesh_.time().set();
        // }
    
};


void AddPyMesh(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<FoamMesh>(m, "FoamMesh")
        .def(py::init<>())
        .def("time", &FoamMesh::time)
        // .def("value", &Dict::value)
        ;
}
