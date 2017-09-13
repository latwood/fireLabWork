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

\*---------------------------------------------------------------------------*/

#include "lapseRateProfileTemperatureInletFvPatchScalarField.H"
#include "incompressible/turbulenceModel/turbulenceModel.H"
#include "fvPatchFieldMapper.H"
#include "volFields.H"
#include "addToRunTimeSelectionTable.H"
#include "wallFvPatch.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace incompressible
{

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void lapseRateProfileTemperatureInletFvPatchScalarField::checkType()
{
    if (isA<wallFvPatch>(patch()))
    {
        FatalErrorIn("lapseRateProfileTemperatureInletFvPatchScalarField::checkType()")
            << "Invalid function specification" << nl
            << "    Patch type for patch " << patch().name()
            << " must not be wall" << nl
            << "    Current patch type is " << patch().type() << nl << endl
            << abort(FatalError);
    }
}


void lapseRateProfileTemperatureInletFvPatchScalarField::writeLocalEntries(Ostream& os) const
{
    os.writeKeyword("TsurfMax") << TsurfMax_ << token::END_STATEMENT << nl;
    os.writeKeyword("TlapseRate") << TlapseRate_ << token::END_STATEMENT << nl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

lapseRateProfileTemperatureInletFvPatchScalarField::lapseRateProfileTemperatureInletFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedInternalValueFvPatchField<scalar>(p, iF),
    TsurfMax_(310),
    TlapseRate_(0.0098)
{
    checkType();
}


lapseRateProfileTemperatureInletFvPatchScalarField::lapseRateProfileTemperatureInletFvPatchScalarField
(
    const lapseRateProfileTemperatureInletFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedInternalValueFvPatchField<scalar>(ptf, p, iF, mapper),
    TsurfMax_(ptf.TsurfMax_),
    TlapseRate_(ptf.TlapseRate_)
{
    checkType();
}


lapseRateProfileTemperatureInletFvPatchScalarField::lapseRateProfileTemperatureInletFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, volMesh>& iF,
    const dictionary& dict
)
:
    fixedInternalValueFvPatchField<scalar>(p, iF, dict),
    TsurfMax_(dict.lookupOrDefault<scalar>("TsurfMax", 310)),
    TlapseRate_(dict.lookupOrDefault<scalar>("TlapseRate", 0.0098))
{
    checkType();
}


lapseRateProfileTemperatureInletFvPatchScalarField::lapseRateProfileTemperatureInletFvPatchScalarField
(
    const lapseRateProfileTemperatureInletFvPatchScalarField& ewfpsf
)
:
    fixedInternalValueFvPatchField<scalar>(ewfpsf),
    TsurfMax_(ewfpsf.TsurfMax_),
    TlapseRate_(ewfpsf.TlapseRate_)
{
    checkType();
}


lapseRateProfileTemperatureInletFvPatchScalarField::lapseRateProfileTemperatureInletFvPatchScalarField
(
    const lapseRateProfileTemperatureInletFvPatchScalarField& ewfpsf,
    const DimensionedField<scalar, volMesh>& iF
)
:
    fixedInternalValueFvPatchField<scalar>(ewfpsf, iF),
    TsurfMax_(ewfpsf.TsurfMax_),
    TlapseRate_(ewfpsf.TlapseRate_)
{
    checkType();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void lapseRateProfileTemperatureInletFvPatchScalarField::updateCoeffs()
{
    if (updated())
    {
        return;
    }

    const label patchI = patch().index();

    // Set T
    for(int i = 0; i < faceI; i++)
    {
        label faceCellI = patch().faceCells()[i];

        T[faceCellI] = TsurfMax_ - TlapseRate_;
    }

    fixedInternalValueFvPatchField<scalar>::updateCoeffs();

    // TODO: perform averaging for cells sharing more than one boundary face
}


void lapseRateProfileTemperatureInletFvPatchScalarField::evaluate
(
    const Pstream::commsTypes commsType
)
{
    fixedInternalValueFvPatchField<scalar>::evaluate(commsType);
}


void lapseRateProfileTemperatureInletFvPatchScalarField::write(Ostream& os) const
{
    fixedInternalValueFvPatchField<scalar>::write(os);
    writeLocalEntries(os);
    writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    lapseRateProfileTemperatureInletFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace incompressible
} // End namespace Foam

// ************************************************************************* //
