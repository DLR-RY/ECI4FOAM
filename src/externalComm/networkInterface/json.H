/*---------------------------------------------------------------------------*\
            Copyright (c) 2021, German Aerospace Center (DLR)
-------------------------------------------------------------------------------
License
    This file is part of the VoFLibrary source code library, which is an
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
    Foam::websocket

Description

Author
    Henning Scheufler, DLR, all rights reserved.

SourceFiles


\*---------------------------------------------------------------------------*/
#include <nlohmann/json.hpp>
#include "registeredObject.H"

using json = nlohmann::json;

namespace Foam
{
    void to_json(json& j, const registeredObject<scalar>& s)
    {
        j[s.name()] = s.ref();
    }

    void from_json(const json& j, registeredObject<scalar>& s)
    {
        s.ref() = j[s.name()];
    }

    void to_json(json& j, const registeredObject<vector>& s)
    {
        const vector& sref = s.ref();
        j[s.name()] = {sref.x(),sref.y(),sref.z()};
    }

    void from_json(const json& j, registeredObject<vector>& s)
    {
        vector& sref = s.ref();
        sref.x() = j[s.name()][0];
        sref.y() = j[s.name()][1];
        sref.z() = j[s.name()][2];
    }


}
