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

#include <vector>
#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "RASModel.H"
#include "wallDist.H"
#include "scalar.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// turbulence constants - file-scope

double rounder(double valToRound,double roundVal = 1.0)	//roundVal = 1000.0 means round to 3 decimals
{
	if(roundVal == 0)
	{
		std::cout << "Error in Rounder function! Can't use 0 as the input " 
				<< "since divide by 0! Changing it to 1.0 as default!\n";
		roundVal = 1.0;
	}
	double roundedValue = round(valToRound*roundVal)/roundVal;
	return roundedValue;
}

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
	
	// hmm, this worked up until the mesh started to get refined. I could counter it
	// by copying over the file and adding zeroes to the end, up until renumber mesh happens
	// then all the cell numbers are all messed up. There is no way to do this without
	// figuring out how to access a cell index, which it looks like the indices are not in
	// in order of 0 to max from left to right, down and up in the mesh.
	// So I need to go through openfoam and figure out what data stuff I can know and think
	// of how geometrically I can manipulate what I can get to always acheive the 0,1,2,3 . . .
	// values for each layer

	// So I know a cell center x,y,z coordinates for the inner field,
	// the same coordinates, but with z subtracting the z coordinate of the lowest cell center
	// the face center x,y,z coordinates for each patch (so the z of minZ patch is actually
	// smaller than the lowest z of the inner field since it is a face and not a patch)

	// the cell centers by themselves are useless. Need the cell centers based off of the
	// height from the ground, so taking away the smallest value
	// but I need to somehow get the smallest value back to divide in to get my actual values

/*
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
	const polyPatch& minZpatch = mesh.boundaryMesh()[minZpatchID];*/
	
/*
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
	zThresh.write();*/



	//find all unique x,y coordinates of the inner fields
	// then find the smallest minZ values for each x,y coordinates
	// then go through the z values and divide it by the minZ values of the same x,y coordinates
	// for this to work even with refined mesh, since the coordinates are not the same for the inner field as for the walls, this needs to be done using the inner field
	
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
	
	std::cout << "Finding unique xcoords\n";	//would totally do this just with patch faces, except the mesh doesn't keep the same coordinates as the side walls throughout the mesh	
	std::vector<double> xcoords;
	//notice that these are the nearest to the wall values for each inner field cell centers
	volScalarField internalField_x(mesh.C().component(0));
	forAll(internalField_x,cellI)
	{
		bool repeatedXcoord = false;
		double xcoordsSize = xcoords.size();
		for(double i = 0; i < xcoordsSize; i++)
		{
			if(rounder(internalField_x[cellI]) == rounder(xcoords[i]))
			{
				repeatedXcoord = true;
				break;
			}
		}
		if(repeatedXcoord == false)
		{
			xcoords.push_back(internalField_x[cellI]);
			std::cout << internalField_x[cellI] << " ";
		}
	}
	std::cout << "\nxcoords.size = " << xcoords.size() << "\n";

	
	std::cout << "Finding unique ycoords\n";	//would totally do this just with patch faces, except the mesh doesn't keep the same coordinates as the side walls throughout the mesh
	std::vector<double> ycoords;
	//notice that these are the nearest to the wall values for each inner field cell centers
	volScalarField internalField_y(mesh.C().component(1));
	forAll(internalField_y,cellI)
	{
		bool repeatedYcoord = false;
		double ycoordsSize = ycoords.size();
		for(double j = 0; j < ycoordsSize; j++)
		{
			if(rounder(internalField_y[cellI]) == rounder(ycoords[j]))
			{
				repeatedYcoord = true;
				break;
			}
		}
		if(repeatedYcoord == false)
		{
			ycoords.push_back(internalField_y[cellI]);
			std::cout << internalField_y[cellI] << " ";
		}
	}
	std::cout << "\nycoords.size = " << ycoords.size() << "\n";

	std::cout << "Finding smallest minZ values for each x & y coords\n";
	//std::vector< std::vector<int> > zMin(4, std::vector<int>(4,4));
	std::vector< std::vector<double> > zMin(xcoords.size(), std::vector<double>(ycoords.size(),9999999));	// this will have to get filled based off of the other two
	volScalarField z = wallDist(mesh).y();	// this is the height above the smallest cell in the mesh
	forAll(z, cellI)
	{
		double ycoordsSize = ycoords.size();
		for(double j = 0; j < ycoordsSize; j++)
		{
			double xcoordsSize = xcoords.size();
			for(double i = 0; i < xcoordsSize; i++)
			{
				if(rounder(internalField_x[cellI]) == rounder(xcoords[i])
				 && rounder(internalField_y[cellI]) == rounder(ycoords[j]))
				{
					if(z[cellI] < zMin[i][j])
					{
						zMin[i][j] = z[cellI];
					}
					i = xcoordsSize;
					j = ycoordsSize;
				}
			}
		}
	}

	std::cout << "setting zThresh values\n";
	forAll(z, cellI)
	{
		double ycoordsSize = ycoords.size();
		for(double j = 0; j < ycoordsSize; j++)
		{
			double xcoordsSize = xcoords.size();
			for(double i = 0; i < xcoordsSize; i++)
			{
				if(rounder(internalField_x[cellI]) == rounder(xcoords[i])
				 && rounder(internalField_y[cellI]) == rounder(ycoords[j]))
				{
					zThresh[cellI] = (z[cellI]/zMin[i][j]-1)/2.0;
					i = xcoordsSize;
					j = ycoordsSize;
				}
			}
		}
	}
	std::cout << "writing zThresh values\n";
	zThresh.write();

	Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
