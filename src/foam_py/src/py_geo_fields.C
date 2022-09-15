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

#include "py_geo_fields.H"



namespace Foam
{

// namespace py = pybind11;

template<class Type>
const Field<Type>& field(const VolField<Type>& vf,const fvMesh& mesh, const std::string& name)
{
    if (name == "internalField")
    {
        return vf.primitiveField();
    }
    else
    {
        label patchId = mesh.boundaryMesh().findPatchID(name);
        if (patchId == -1)
        {
            FatalErrorInFunction
                << "patch not found " << nl
                << exit(FatalError);
        }
        return vf.boundaryField()[patchId];
    }
}

template<class Type>
void field(VolField<Type>& vf,const fvMesh& mesh, const std::string& name,const Field<Type>& f)
{
    if (name == "internalField")
    {
        vf.primitiveFieldRef() = f;
    }
    else
    {
        label patchId = mesh.boundaryMesh().findPatchID(name);
        if (patchId == -1)
        {
            FatalErrorInFunction
                << "patch not found " << nl
                << exit(FatalError);
        }
        vf.boundaryFieldRef()[patchId] = f;
    }
}

template<class Type>
VolField<Type> read_volField
(
    const std::string& name,
    const fvMesh& mesh
)
{
    return GeometricField<Type, fvPatchField, volMesh>
    (
        IOobject
        (
            name,
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    );
}

template<class Type>
py::class_< VolField<Type> > declare_fields(py::module &m, std::string className) {
    auto geofieldClass = py::class_< Foam::VolField<Type>>(m, className.c_str())
    .def(py::init([](const std::string& name,const Foam::fvMesh& mesh) {
        return Foam::read_volField<Type>(name,mesh);
    }))
    .def("internalField", [](const Foam::VolField<Type>& self, const std::string& name) {
        // const Field<Type>& values = this->primitiveField();
        return self.primitiveField();
    })
    .def("__getitem__", [](const Foam::VolField<Type>& self, const std::string& name) {
        return Foam::field(self,self.mesh(),name);
    })
    .def("__setitem__", [](Foam::VolField<Type>& self, const std::string& name,const Foam::Field<Type>& f) {
        Foam::field(self,self.mesh(),name,f);
    })
    .def("__add__", [](Foam::VolField<Type>& self, const Foam::VolField<Type>& vf2) {return Foam::VolField<Type>(self + vf2);})

    .def("__sub__", [](Foam::VolField<Type>& self, const Foam::VolField<Type>& vf2) {return Foam::VolField<Type>(self - vf2);})

    .def("__mul__", [](Foam::VolField<Type>& self, const Foam::VolField<Foam::scalar>& vf2) {return Foam::VolField<Type>(self * vf2);})
    .def("__truediv__", [](Foam::VolField<Type>& self, const Foam::VolField<Foam::scalar>& vf2) {return Foam::VolField<Type>(self / vf2);})

    ;
    return geofieldClass;
}

}


void AddPyGeoFields(py::module& m)
{
    namespace py = pybind11;

    auto sf = Foam::declare_fields<Foam::scalar>(m, std::string("volScalarField"));

    auto vf = Foam::declare_fields<Foam::vector>(m, std::string("volVectorField"));

    auto tf = Foam::declare_fields<Foam::tensor>(m, std::string("volTensorField"));
}
