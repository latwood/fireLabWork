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

	Info<< "Setting x,y, and z Thresh values" << endl;
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
	label southPatchID = mesh.boundaryMesh().findPatchID("south_face");
	const polyPatch& southPatch = mesh.boundaryMesh()[southPatchID];

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
	label westPatchID = mesh.boundaryMesh().findPatchID("west_face");
	const polyPatch& westPatch = mesh.boundaryMesh()[westPatchID];

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
	
	scalar Az = minZpatch.size();
	scalar Ay = westPatch.size();
	scalar Ax = southPatch.size();
	std::cout << "Ax = " << Ax << ", Ay = " << Ay << ", Az = " << Az << "\n";
	scalar xMax = std::sqrt(Az*Ax/Ay);
	scalar yMax = Ay*xMax/Ax;
	scalar zMax = Ax/xMax;
	std::cout << "zMax = " << zMax << ", yMax = " << yMax << ", xMax = " << xMax << "\n";

	std::cout << "listed cellI: \n";
	for(double k = 0; k < zMax; k++)
	{
		for(double i = 0; i < yMax; i++)
		{
			for(double j = 0; j < xMax; j++)
			{
				scalar cellI = Az*k+xMax*i+j;	
				std::cout << " " << cellI;
				xThresh[cellI] = i;
				yThresh[cellI] = j;
				zThresh[cellI] = k;
			}
			std::cout << "\n";
		}
	}
	xThresh.write();
	yThresh.write();
	zThresh.write();

	Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
