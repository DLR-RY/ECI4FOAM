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

#ifndef py_geo_fields
#define py_geo_fields

// System includes
#include <pybind11/pybind11.h>

#include "py_mesh.H"
#include "volFields.H"
#include <vector>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

// namespace py = pybind11;

namespace Foam
{


template<typename Type>
pybind11::array_t<scalar> FieldToNumpy(const Field<Type>& values);

template< >
pybind11::array_t<scalar> FieldToNumpy<scalar>(const Field<scalar>& values);

template<typename Type>
void NumpyToField(Field<Type>& values,const pybind11::array_t<scalar> np_arr);

template<>
void NumpyToField<scalar>(Field<scalar>& values,const pybind11::array_t<scalar> np_arr);

template<class Type>
class VolField
:
    public GeometricField<Type, fvPatchField, volMesh>
{
    

    public:
    VolField<Type>
    (
        const tmp< GeometricField< Type, fvPatchField, volMesh > > &tgf
    )
    :
        GeometricField<Type, fvPatchField, volMesh>
    (
        tgf
    )
    {

    }

    VolField<Type>
    (
        const GeometricField<Type, fvPatchField, volMesh>& vlf
    )
    :
        GeometricField<Type, fvPatchField, volMesh>
    (
        vlf
    )
    {
        
    }
    VolField<Type>
    (
        const std::string& name,
        const fvMesh& mesh
    )
    :
        GeometricField<Type, fvPatchField, volMesh>
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
    )
    {
        // Info << "internalField " << this->internalField() << endl;
    }

    VolField<Type>
    (
        const std::string& name,
        const FvMesh& FvMesh
    )
    :
        GeometricField<Type, fvPatchField, volMesh>
    (
        IOobject
        (
            name,
            FvMesh.time().timeName(),
            FvMesh.mesh(),
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        FvMesh.mesh()
    )
    {
        // Info << "internalField " << this->internalField() << endl;
    }


    // template<class Type>
    pybind11::array_t<scalar> internalField() const
    {
        const Field<Type>& values = this->primitiveField();
        return FieldToNumpy<Type>(values);
    }


    void setInternalField(const pybind11::array_t<scalar> np_arr)
    {
        auto& values = this->primitiveFieldRef();
        NumpyToField(values,np_arr);
    }

    pybind11::array_t<scalar> bField(std::string patchName) const
    {
        label patchId = this->mesh().boundaryMesh().findPatchID(patchName);
        if (patchId == -1)
        {
            FatalErrorInFunction
                << "patch not found " << nl
                << exit(FatalError);
        }
        const auto& bField = this->boundaryField()[patchId];
        return FieldToNumpy<Type>(bField); 
    }

    void setBField(std::string patchName,const pybind11::array_t<scalar> np_arr)
    {
        label patchId = this->mesh().boundaryMesh().findPatchID(patchName);
        if (patchId == -1)
        {
            FatalErrorInFunction
                << "patch not found " << nl
                << exit(FatalError);
        }
        auto& bField = this->boundaryFieldRef()[patchId];
        NumpyToField(bField,np_arr);
    }
};

}

// Foam::FieldToNumpy<Foam::scalar>(const Foam::Field<Foam::scalar>& values);
// Foam::NumpyToField<Foam::scalar>(Foam::Field<double>& values,const pybind11::array_t<Foam::scalar> np_arr);




void  AddPyGeoFields(pybind11::module& m);


#endif // py_geo_fields  defined 