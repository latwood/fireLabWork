/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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
    Foam::incompressible::lapseRateProfileTemperatureInletFvPatchScalarField

Group
    grpIcoWallFunctions

Description
    This boundary condition provides a temperature profile wall function

    The condition can be applied to non-wall boundaries, whereby it
    - calculates \c T and \c 
    - inserts near temperature values directly into the energy equation
        to act as a constraint

    where

    \vartable
        T 		| temperature field
    \endvartable

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        TsurfMax     | minZ max temperature    | no          | 310
        TlapseRate   | lapse rate coefficient  | no          | 0.0098
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            lapseRateProfileTemperatureInlet;
    }
    \endverbatim

SeeAlso
    Foam::fixedInternalValueFvPatchField

SourceFiles
    lapseRateProfileTemperatureInletFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef lapseRateProfileTemperatureInletFvPatchScalarField_H_
#define lapseRateProfileTemperatureInletFvPatchScalarField_H_

#include "fixedInternalValueFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{

/*---------------------------------------------------------------------------*\
           Class lapseRateProfileTemperatureInletFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class lapseRateProfileTemperatureInletFvPatchScalarField
:
    public fixedInternalValueFvPatchField<scalar>
{
protected:

    // Protected data

		//- the maximum temperature of minZ
        scalar TsurfMax_;

		//- Lapse rate coefficient
		scalar TlapseRate_;

    // Protected Member Functions

        //- Check the type of the patch
        virtual void checkType();

        //- Write local wall function variables
        virtual void writeLocalEntries(Ostream&) const;


public:

    //- Runtime type information
    TypeName("lapseRateProfileTemperatureInlet");


    // Constructors

        //- Construct from patch and internal field
        lapseRateProfileTemperatureInletFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        lapseRateProfileTemperatureInletFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  lapseRateProfileTemperatureInletFvPatchScalarField
        //  onto a new patch
        lapseRateProfileTemperatureInletFvPatchScalarField
        (
            const lapseRateProfileTemperatureInletFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        lapseRateProfileTemperatureInletFvPatchScalarField
        (
            const lapseRateProfileTemperatureInletFvPatchScalarField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchScalarField> clone() const
        {
            return tmp<fvPatchScalarField>
            (
                new lapseRateProfileTemperatureInletFvPatchScalarField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        lapseRateProfileTemperatureInletFvPatchScalarField
        (
            const lapseRateProfileTemperatureInletFvPatchScalarField&,
            const DimensionedField<scalar, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, volMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new lapseRateProfileTemperatureInletFvPatchScalarField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

            //- Evaluate the patchField
            virtual void evaluate(const Pstream::commsTypes);


        // I-O

            //- Write
            virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace incompressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif /* lapseRateProfileTemperatureInletFvPatchScalarField_H_ */
