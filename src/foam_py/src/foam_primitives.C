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

#include "foam_primitives.H"

#include "instant.H"


void AddFoamPrimitives(pybind11::module& m)
{
    namespace py = pybind11;

    
    py::class_<Foam::instant>(m, "instant")
        .def("__str__",[](const Foam::instant& self){return std::string(self.name());})
    ;


    // primitive classes
    py::class_<Foam::word>(m, "Word")
        .def(py::init<std::string>())
        .def("__eq__",[](const Foam::word& self, std::string w){
            return bool(self == w);
        })
        .def("__str__",[](const Foam::word& self){
            return std::string(self);
        })
        ;


    py::class_<Foam::vector>(m, "Vector")
        .def(py::init<const Foam::vector>())
        .def(py::init<Foam::scalar,Foam::scalar,Foam::scalar>())
        .def("__eq__",[](const Foam::vector& self, const std::array<Foam::scalar, 3>& v){
            return self[0] == v[0] && self[1] == v[1] && self[2] == v[2];
        })
        .def("__getitem__", [](const Foam::vector& self, const Foam::label idx) {
            return self[idx];
        })
        .def("__setitem__", [](Foam::vector& self, const Foam::label idx,const Foam::scalar s) {
            self[idx] = s;
        })
        .def("__len__", [](const Foam::vector& self) -> int {return 3;})
        .def("__add__", &Foam::add<Foam::vector>)
        .def("__sub__", &Foam::subtract<Foam::vector>)
        .def("__mul__", &Foam::multiply_scalar<Foam::vector>)
        .def("__and__", &Foam::inner_product<Foam::vector>)
        .def("__eq__", &Foam::is_equal<Foam::vector>)
        .def("__ne__", &Foam::is_notequal<Foam::vector>)
        ;


    py::class_<Foam::tensor>(m, "Tensor")
        .def(py::init<const Foam::tensor>())
        .def(py::init<  Foam::scalar,Foam::scalar,Foam::scalar,
                        Foam::scalar,Foam::scalar,Foam::scalar,
                        Foam::scalar,Foam::scalar,Foam::scalar>())
        .def("__eq__",[](const Foam::tensor& self, const std::array<Foam::scalar, 9>& v){
            for(int i = 0;i<9;i++ )
            {
                if (self[i] != v[i])
                {
                    return false;
                }
            }

            return true;
        })
        .def("__getitem__", [](const Foam::tensor& self, const Foam::label idx) {
            return self[idx];
        })
        .def("__setitem__", [](Foam::tensor& self, const Foam::label idx,const Foam::scalar s) {
            self[idx] = s;
        })
        .def("__len__", [](const Foam::tensor& self) -> int {return 9;})
        .def("__add__", &Foam::add<Foam::tensor>)
        .def("__sub__", &Foam::subtract<Foam::tensor>)
        .def("__mul__", &Foam::multiply_scalar<Foam::tensor>)
        .def("__eq__", &Foam::is_equal<Foam::tensor>)
        .def("__ne__", &Foam::is_notequal<Foam::tensor>)
        ;

    // functions
    m.def("mag", [](Foam::scalar t){return Foam::mag(t);});
    m.def("mag", [](Foam::vector t){return Foam::mag(t);});
    m.def("mag", [](Foam::tensor t){return Foam::mag(t);});
}
