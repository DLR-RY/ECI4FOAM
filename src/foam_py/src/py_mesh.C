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

#include "py_mesh.H"
#include "volFields.H"


namespace Foam
{

Foam::instantList selectTimes
( 
    Time& runTime,
    const std::vector<std::string>& args
)
{
    int argc = args.size();
    char ** argv = new char*[argc];
    for(int i = 0; i < argc; i++)
    {
        argv[i] = new char[args[i].size() + 1];
        strcpy(argv[i], args[i].c_str());
    }
    Foam::argList list_arg(argc, argv);
    return Foam::timeSelector::select0(runTime, list_arg);
}


Time* createTime(std::string rootPath,std::string caseName)
{
    return new Time(rootPath,caseName);
}

fvMesh* createMesh(const Time& time)
{
    fvMesh* mesh( 
        new fvMesh
        (
            IOobject
            (
                "region0",
                time.timeName(),
                time,
                IOobject::MUST_READ
            ),
            false
        )
    );
    mesh->init(true);
    return mesh;
}

}

void AddPyMesh(pybind11::module& m)
{
    namespace py = pybind11;

    m.def("selectTimes",&Foam::selectTimes);

    py::class_<Foam::Time>(m, "Time")
        .def(py::init(&Foam::createTime),py::return_value_policy::take_ownership)
        .def("setTime", []
        (
            Foam::Time& self,
            const Foam::instant& inst,
            const Foam::label newIndex
        )
        {
            self.setTime(inst,newIndex);
        })
    ;

    py::class_<Foam::fvMesh>(m, "fvMesh")
        .def(py::init(&Foam::createMesh),py::return_value_policy::take_ownership)
        .def("C",&Foam::fvMesh::C,py::return_value_policy::reference)
        // .def("Cf",&Foam::fvMesh::Cf,py::return_value_policy::reference)
    ;

}
