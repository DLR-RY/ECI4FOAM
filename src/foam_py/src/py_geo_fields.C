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
#include <type_traits>

namespace Foam
{

template<typename Type>
pybind11::array_t<scalar> Foam::FieldToNumpy(const Field<Type>& values)
{
    label nElements = values.size();
    label nComps = pTraits<Type>::nComponents;

    pybind11::array_t<scalar, pybind11::array::c_style> arr({nElements, nComps});
    auto ra = arr.mutable_unchecked();

    forAll(values,celli)
    {
        for (label i = 0; i < pTraits<Type>::nComponents; ++i)
        {
            ra(celli, i) = values[celli].component(i);
        }
    }

    return arr;
}

template< >
pybind11::array_t<scalar> FieldToNumpy<scalar>(const Field<scalar>& values)
{
    label nElements = values.size();

    pybind11::array_t<scalar, pybind11::array::c_style> arr(nElements);
    auto ra = arr.mutable_unchecked();

    forAll(values,celli)
    {
        ra(celli) = values[celli];
    }

    return arr;
}


template<typename Type>
void NumpyToField(Field<Type>& values,const pybind11::array_t<scalar> np_arr)
{
    label nComps = pTraits<Type>::nComponents;
    label nElements = values.size();

    if (np_arr.shape(1) != nComps)
    {
        FatalErrorInFunction
            << "dimensions do not match " << nl
            << "the expected value: " << nComps << nl
            << "the provided value: " << np_arr.shape(1) << nl
            << exit(FatalError);
    }

    if (np_arr.shape(0) != nElements)
    {
        FatalErrorInFunction
            << "length of numpy array does not match " << nl
            << "the expected value: " << nElements<< nl
            << "the provided value: " << np_arr.size() << nl
            << exit(FatalError);
    }
    const auto ra = np_arr.unchecked();

    forAll(values,celli)
    {
        for (label i = 0; i < pTraits<Type>::nComponents; ++i)
        {
            values[celli].component(i) = ra(celli, i); 
        }
    }
}

template<>
void NumpyToField<scalar>(Field<scalar>& values,const pybind11::array_t<scalar> np_arr)
{
    if (np_arr.ndim() != 1)
    {
        FatalErrorInFunction
            << "numpy array is not onedimensional " << nl
            << "provided array has ndims : " << np_arr.ndim() << nl
            << exit(FatalError);
    }

    if (np_arr.size() != values.size())
    {
        FatalErrorInFunction
            << "length of numpy array does not match " << nl
            << "the expected value: " << values.size()<< nl
            << "the provided value: " << np_arr.size() << nl
            << exit(FatalError);
    }

    const auto ra = np_arr.unchecked();

    forAll(values,celli)
    {
        values[celli] = ra(celli); 
    }
}

}
void AddPyGeoFields(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<Foam::VolField<Foam::scalar>>(m, "volScalarField")
    .def(py::init<const std::string&,const Foam::FvMesh&>())
    .def_property("internalField", &Foam::VolField<Foam::scalar>::internalField, &Foam::VolField<Foam::scalar>::setInternalField)
    .def("bField", &Foam::VolField<Foam::scalar>::bField)
    .def("bField", &Foam::VolField<Foam::scalar>::setBField)
    .def("__getitem__", [](const Foam::VolField<Foam::scalar>& vf, const std::string& name) {
        if (name == "internalField")
        {
            return vf.internalField();
        }
        else
        {
            return vf.bField(name);
        }
    })
    .def("__setitem__", [](Foam::VolField<Foam::scalar>& vf, const std::string& name,const pybind11::array_t<Foam::scalar> np_arr) {
        if (name == "internalField")
        {
            vf.setInternalField(np_arr);
        }
        else
        {
            vf.setBField(name,np_arr);
        }
    })

    ;

    py::class_<Foam::VolField<Foam::vector>>(m, "volVectorField")
    .def(py::init<const std::string&,const Foam::FvMesh&>())
    .def_property("internalField", &Foam::VolField<Foam::vector>::internalField, &Foam::VolField<Foam::vector>::setInternalField)
    .def("bField", &Foam::VolField<Foam::vector>::bField)
    .def("bField", &Foam::VolField<Foam::vector>::setBField)
    .def("__getitem__", [](const Foam::VolField<Foam::scalar>& vf, const std::string& name) {
        if (name == "internalField")
        {
            return vf.internalField();
        }
        else
        {
            return vf.bField(name);
        }
    })
    .def("__setitem__", [](Foam::VolField<Foam::scalar>& vf, const std::string& name,const pybind11::array_t<Foam::scalar> np_arr) {
        if (name == "internalField")
        {
            vf.setInternalField(np_arr);
        }
        else
        {
            vf.setBField(name,np_arr);
        }
    })
    ;
}
