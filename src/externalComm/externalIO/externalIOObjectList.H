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
    Foam::externalIOObjectList

Description
    List of function objects with start(), execute() and end() functions


See also
    Foam::externalIOObject
    Foam::functionObjects::timeControl

SourceFiles
    externalIOObjectList.C

\*---------------------------------------------------------------------------*/

#ifndef externalIOObjectList_H
#define externalIOObjectList_H

#include "PtrList.H"
#include "Enum.H"
#include "externalIOObject.H"
#include "SHA1Digest.H"
#include "HashTable.H"
#include "IOdictionary.H"
#include "HashSet.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


/*---------------------------------------------------------------------------*\
                     Class externalIOObjectList Declaration
\*---------------------------------------------------------------------------*/

class externalIOObjectList
:
    private PtrList<externalIOObject::externalIOObject>
{

        IOdictionary parentDict_;

        const Time& time_;
    // Private Member Functions

        //- No copy construct
        externalIOObjectList(const externalIOObjectList&) = delete;

        //- No copy assignment
        void operator=(const externalIOObjectList&) = delete;


public:

    // Static data members

    // Constructors

        //- Construct from Time, a dictionary with a "functions" entry
        //- and the execution setting.
        //  \param[in]  runTime - the other Time instance to construct from
        //  \param[in]  parentDict - the parent dictionary containing
        //    a "functions" sub-dictionary of externalIOObject specifications.
        //  \param[in]  execution - whether the function objects should execute
        //    or not. Default: true.
        externalIOObjectList
        (
            const Time& runTime,
            const bool execution=true
        );

    //- Destructor
    ~externalIOObjectList();


    // Member Functions

        //- Return the number of elements in the List.
        using PtrList<externalIOObject::externalIOObject>::size;

        //- Return true if the List is empty (ie, size() is zero).
        using PtrList<externalIOObject::externalIOObject>::empty;

        //- Access to the functionObjects
        using PtrList<externalIOObject::externalIOObject>::operator[];

        //- List of functions
        const PtrList<externalIOObject::externalIOObject>& functions() const { return *this; }

        //- List of functions
        PtrList<externalIOObject::externalIOObject>& functions() { return *this; }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
