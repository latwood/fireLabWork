/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
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

Application
	applyInitialTemperatureField

Description
	Apply a simple temperature field initial condition to all inner cells.
	Based on the simple correlation that temperature drops by 0.0098 K/m in the z direction
	for dry air when in the planetary boundary layer of the troposphere.

    Apply a simplified boundary-layer model to the velocity and
    turbulence fields based on the 1/7th power-law.

    The uniform boundary-layer thickness is either provided via the -ybl option
    or calculated as the average of the distance to the wall scaled with
    the thickness coefficient supplied via the option -Cbl.  If both options
    are provided -ybl is used.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "RASModel.H"
#include "wallDist.H"
#include "scalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// turbulence constants - file-scope


int main(int argc, char *argv[])
{

    #include "setRootCase.H"

	#include "createTime.H"
    #include "createMesh.H"

	volScalarField T
	(
		IOobject
		(
			"T",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::AUTO_WRITE
		),
		mesh
	);
	
	Info<< "Calculating innerField profile for T" << endl;

	volScalarField z_innerField = wallDist(mesh).y();	//distance to nearest wall zmin. So height above z0 for each z point. Beware if there are other walls than minZ. Notice that this is only for the inner field!
	
	scalar phi_s = 280; // K
	scalar gamma = 0.0032;	// K/m
	scalar deltaphi = 5;	// K
	scalar beta = 0.002;	// 1/m
	
	
	// Loop over all the faces in the patch
	// and initialize the temperature profile
	// find Tmin for setting minZ temperature
	scalar Tmin = 99999999;	// set to huge value initially
	forAll(z_innerField, cellI)
	{
		// relative height from ground for face lists
		scalar z = z_innerField[cellI];	//I think that AGL is the height of the cell

		//Apply formula to get profile. Make sure that the AGL is corrected for the minZ height
		scalar Tcalc = phi_s + gamma*z + deltaphi*(1 - std::exp(-beta*z));
		if(Tcalc < Tmin)
		{
			Tmin = Tcalc;
		}
    	T[cellI] = Tcalc;
	}
	T.write();

	Info<< "Set innerField profile for T" << endl;
	std::cout << "Tmin = " << Tmin << "\n";
	Info<< "Setting minZ profile using Tmin" << endl;
	
	label minZpatchID = mesh.boundaryMesh().findPatchID("minZ");
	scalarField& minZTpatch = refCast<scalarField>(T.boundaryField()[minZpatchID]);
	forAll(minZTpatch, faceI)
	{
		minZTpatch[faceI] = Tmin;
	}
	T.write();
	

	// this is what I used to do. I like this, but it has some issues
    /*
	#include "setRootCase.H"

	#include "createTime.H"
    #include "createMesh.H"

	volScalarField T
	(
		IOobject
		(
			"T",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::AUTO_WRITE
		),
		mesh
	);
	
	Info<< "Calculating log profile for T" << endl;

	label minZpatchID = mesh.boundaryMesh().findPatchID("minZ");
	const scalarField& minZTvalues = T.boundaryField()[minZpatchID];
	scalar TsurfMax = 0;
	forAll(minZTvalues,facei)
	{
		scalar Tvalue = minZTvalues[facei];
		if(Tvalue > TsurfMax)
		{
			TsurfMax = Tvalue;
		}
	} 

	std::cout << "TsurfMax = " << TsurfMax << "\n";
	if(TsurfMax == 0)
	{
		std::cout << "changing TsurfMax to value of 310\n";
		TsurfMax = 310;
	}
	scalar Tcalc(0.0);
	scalar TlapseRate = 0.0098;
	std::cout << "TlapseRate = " << TlapseRate << "\n";

	volScalarField z = wallDist(mesh).y();	//distance to nearest wall zmin. So height above z0 for each z point. Beware if there are other walls than minZ
	//volScalarField z(mesh.C().component(2)); //z-component of cell center. So height above lowest z point, not height above z0 for each z point.
	//looks like you can replace wallDist with patchDistMethod for other options

	// Loop over all the faces in the patch
	// and initialize the log profile
	forAll(z, cellI)
	{
		// relative height from ground for face lists
		scalar AGL = z[cellI];	//I think that AGL is the height of the cell

		//Apply formula to get profile. Make sure that the AGL is corrected for the minZ height
		Tcalc = TsurfMax - TlapseRate*AGL;
    	T[cellI] = Tcalc;
	}
	T.write();*/

	Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
