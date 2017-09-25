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


	// need to make a patch minZ and get the face centers of the patch
	// then in the loop going through the internal cells, if the x and y coordinates
	// match the x and y coordinates of a minZ face center, divide the z coordinate of the cell
	// by the z coordinate of the minZ face center.

	// hm, this didn't work quite well enough. If I get three patches, 
	// one for each x,y,z faceI's, then do a triple for loop using these guys
	// then I use a summation of the three counter variables to represent the cellI to
	// give the right location the value, then use the x,y,or z counter
	// to give it the cell index counter in the x,y,or z direction!

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

	label minZpatchID = mesh.boundaryMesh().findPatchID("minZ");
	const polyPatch& minZpatch = mesh.boundaryMesh()[minZpatchID];
	
	volScalarField internalField_x(mesh.C().component(0));
	volScalarField internalField_y(mesh.C().component(1));
	volScalarField internalField_z(mesh.C().component(2));
	//volScalarField internalField_z = wallDist(mesh).y();

	forAll(internalField_z,cellI)
	{
		scalar inField_x = internalField_x[cellI];
		inField_x = round(inField_x*1000.0)/1000.0;
		scalar inField_y = internalField_y[cellI];
		inField_y = round(inField_y*1000.0)/1000.0;
		forAll(minZpatch,faceI)
		{
			scalar minZ_x = mesh.C().boundaryField()[minZpatchID][faceI].component(0);
			minZ_x = round(minZ_x*1000.0)/1000.0;	//rounds to the nearest 3rd decimal place
			scalar minZ_y = mesh.C().boundaryField()[minZpatchID][faceI].component(1);
			minZ_y = round(minZ_y*1000.0)/1000.0;
			if(minZ_x == inField_x && minZ_y == inField_y)
			{
				scalar minZ_z = mesh.Cf().boundaryField()[minZpatchID][faceI].component(2);
				zThresh[cellI] = internalField_z[cellI]/minZ_z;
				break;
			}
		}
	}
	zThresh.write();

	//const volVectorField& meshC = mesh.C();

	//scalar minZ_x = mesh.C().boundaryField()[minZpatchID][1].component(0);
/*
	volScalarField minZ_x(minZpatch.mesh.C().component(0));
	volScalarField minZ_y(minZpatch.mesh.C().component(1));
	volScalarField minZ_z(minZpatch.mesh.C().component(2));
	
	volScalarField internalField_x(mesh.C().component(0));
	volScalarField internalField_y(mesh.C().component(1));
	volScalarField internalField_z(mesh.C().component(2));

	forAll(internalField_z,cellI)
	{
		forAll(minZpatch,faceI)
		{
			if(minZ_x[faceI] == internalField_x[cellI] && minZ_y[faceI] == internalField_y[cellI])
			{
				zThresh[cellI] = internalField_z[cellI]/minZ_z[faceI];
			}
		}
	}
	zThresh.write();
*/

/*	label minZpatchID = mesh.boundaryMesh().findPatchID("minZ");
	const polyPatch& minZpatch = mesh.boundaryMesh()[minZpatchID];
	volScalarField minZ_x(minZpatch.faceCentres().component(0));
	volScalarField minZ_y(minZpatch.faceCentres().component(1));
	volScalarField minZ_z(minZpatch.faceCentres().component(2));
	
	volScalarField internalField_x(mesh.C().component(0));
	volScalarField internalField_y(mesh.C().component(1));
	volScalarField internalField_z(mesh.C().component(2));
	
	forAll(internalField_z,cellI)
	{
		forAll(minZpatch,faceI)
		{
			if(minZ_x[faceI] == internalField_x[cellI] && minZ_y[faceI] == internalField_y[cellI])
			{
				zThresh[cellI] = internalField_z[cellI]/minZ_z[faceI];
			}
		}
	}
	zThresh.write();
*/


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
