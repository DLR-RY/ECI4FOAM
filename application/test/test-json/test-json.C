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

\*---------------------------------------------------------------------------*/

#include <json.H>
#include <fstream>

#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
using json = nlohmann::json;

// basically checks if installed correctly
TEST_CASE( "test json", "[json]" )
{
    // create an empty structure (null)
    json j;

    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["pi"] = 3.141;

    REQUIRE(j["pi"] == 3.141);

    std::ofstream os("test.json");
    

    os << j;
}



// ************************************************************************* //
