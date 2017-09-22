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
	applyThresholdBlockValues

Description
	Apply a value for every layer so that someone can take a threshold of the whole mesh, 
	taking the threshold of that value. This will split the mesh into blocks for visualization

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



	Info<< "Setting xThresh values" << endl;
	volScalarField xThresh
	(
		IOobject
		(
			"xThresh",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::AUTO_WRITE
		),
		mesh
	);
	
	volScalarField x(mesh.C().component(0));
	
	forAll(x,cellI)
	{
		scalar AGL = x[cellI];
		xThresh[cellI] = AGL;
	}
	xThresh.write();



	Info<< "Setting yThresh values" << endl;
	volScalarField yThresh
	(
		IOobject
		(
			"yThresh",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::AUTO_WRITE
		),
		mesh
	);
	
	volScalarField y(mesh.C().component(1));
	
	forAll(y,cellI)
	{
		scalar AGL = y[cellI];
		yThresh[cellI] = AGL;
	}
	yThresh.write();



	Info<< "Setting zThresh values" << endl;
	volScalarField zThresh
	(
		IOobject
		(
			"zThresh",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::AUTO_WRITE
		),
		mesh
	);
	
	volScalarField z(mesh.C().component(2));
	
	forAll(z,cellI)
	{
		scalar AGL = z[cellI];
		zThresh[cellI] = AGL;
	}
	zThresh.write();



//	volScalarField zMin = wallDist(mesh).y();	//distance to nearest wall zmin. So height above z0 for each z point. Beware if there are other walls than minZ
//	volScalarField zMax(mesh.C().component(2)); //z-component of cell center. So height above lowest z point, not height above z0 for each z point.
	//looks like you can replace wallDist with patchDistMethod for other options

//	forAll(zMin, cellI)
//	{
		// relative height from ground for face lists
//		scalar AGLmin = zMin[cellI];	//I think that AGL is the height of the cell
//		scalar AGLmax = zMax[cellI];

		//Apply formula to get profile. Make sure that the AGL is corrected for the minZ height
		//ThreshCalc = (AGLmax-AGLmin)*ThresholdValueIncrementer;
		//ThreshCalc = AGLmin*ThresholdValueIncrementer;
//		ThreshCalc = AGLmax/(AGLmax-AGLmin)*ThresholdValueIncrementer;
 //   	zThresh[cellI] = ThreshCalc;
//	}
//	zThresh.write();

	Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
