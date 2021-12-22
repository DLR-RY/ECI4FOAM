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

#include "fvCFD.H"
#include "commDataLayer.H"
#include "registeredObject.H"
#include "Field.H"

#define CATCH_CONFIG_MAIN 
#include <catch2/catch.hpp>
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

TEST_CASE( "commDataLayer store and erase multiple registries", "[commDataLayer]" )
{
    int argc = 1;
    char **argv = static_cast<char**>(malloc(sizeof(char*)));
    char executable[] = {'m','a','i','n'};
    argv[0] = executable;
    Foam::argList args(argc, argv,false,false,false);

    // #include "setRootCase.H"
    #include "createTime.H" // create runTime


    commDataLayer& data = commDataLayer::New(runTime);

    data.storeObj(1,"test_in",commDataLayer::causality::in);
    data.storeObj(1,"test_out",commDataLayer::causality::out);
    data.storeObj(1,"test_parameter",commDataLayer::causality::parameter);

    const auto& in = data.getRegistry(commDataLayer::causality::in);
    const auto& out = data.getRegistry(commDataLayer::causality::out);
    const auto& parameter = data.getRegistry(commDataLayer::causality::parameter);
    
    REQUIRE( 1 == in.classes().size());
    REQUIRE( 1 == out.classes().size());
    REQUIRE( 1 == parameter.classes().size());

    data.eraseObj("test_in",commDataLayer::causality::in);
    data.eraseObj("test_out",commDataLayer::causality::out);
    data.eraseObj("test_parameter",commDataLayer::causality::parameter);

    REQUIRE( 0 == in.classes().size());
    REQUIRE( 0 == out.classes().size());
    REQUIRE( 0 == parameter.classes().size());

}

TEST_CASE( "commDataLayer store, get and mod", "[commDataLayer]" )
{
    int argc = 1;
    char **argv = static_cast<char**>(malloc(sizeof(char*)));
    char executable[] = {'m','a','i','n'};
    argv[0] = executable;
    Foam::argList args(argc, argv,false,false,false);

    // #include "setRootCase.H"
    #include "createTime.H" // create runTime


    commDataLayer& data = commDataLayer::New(runTime);

    // base types
    bool test_b = false; // FMI 2.0
    word test_w = "asd"; // FMI 2.0
    label test_l = 0; // FMI 2.0
    scalar test_s = 0.0; // FMI 2.0
    vector test_v = Zero;
    symmTensor test_st = Zero;
    sphericalTensor test_spt = Zero;
    tensor test_t = Zero;

    // // Fields
    Field<scalar> test_fs(2,Zero);
    Field<vector> test_fv(2,Zero);
    Field<symmTensor> test_fst(2,Zero);
    Field<sphericalTensor> test_fsp(2,Zero);
    Field<tensor> test_ft(2,Zero);

    data.storeObj(test_b,"test_b",commDataLayer::causality::in);
    data.storeObj(test_w,"test_w",commDataLayer::causality::in);
    data.storeObj(test_l,"test_l",commDataLayer::causality::in);
    data.storeObj(test_s,"test_s",commDataLayer::causality::in);
    data.storeObj(test_v,"test_v",commDataLayer::causality::in);
    data.storeObj(test_st,"test_st",commDataLayer::causality::in);
    data.storeObj(test_spt,"test_spt",commDataLayer::causality::in);
    data.storeObj(test_t,"test_t",commDataLayer::causality::in);
    data.storeObj(test_fs,"test_fs",commDataLayer::causality::in);
    data.storeObj(test_fv,"test_fv",commDataLayer::causality::in);
    data.storeObj(test_fst,"test_fst",commDataLayer::causality::in);
    data.storeObj(test_fsp,"test_fsp",commDataLayer::causality::in);
    data.storeObj(test_ft,"test_ft",commDataLayer::causality::in);

    const auto& in = data.getRegistry(commDataLayer::causality::in);
    
    REQUIRE( 13 == in.classes().size());

    scalar& s = data.getObj<scalar>("test_s",commDataLayer::causality::in);
    s = 100;
    scalar changeVal = data.getObj<scalar>("test_s",commDataLayer::causality::in);
    REQUIRE( 100 == changeVal);

    scalarField& sf = data.getObj<Field<scalar> >("test_fs",commDataLayer::causality::in);
    sf = 100;
    scalarField changeValField = data.getObj<scalarField>("test_fs",commDataLayer::causality::in);
    REQUIRE( 100 == average(changeValField));

}



// ************************************************************************* //
