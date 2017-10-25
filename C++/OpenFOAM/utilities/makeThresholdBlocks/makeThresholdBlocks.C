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

#include "sortedMesh.H"

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

	std::cout << "mesh size = " << mesh.C().size() << "\n";
	//const unallocLabelList & theOwners( mesh.owner() );	// const is on the left, so unallocLabelList is the constant, not the & reference. If const were on the right of &, the reference would be the const
	//const unallocLabelList & theNeighbors( mesh.neighbour() );
	//std::vector<label> uniqueNeighbors(mesh.C().size(),99999);

	sortedMesh betterMesh(mesh.points(),mesh.faces(),mesh.faceOwner(),mesh.faceNeighbour(),mesh.boundaryMesh(),mesh.C().size());	// problem I had here was that somehow the new .C file was not included in some kind of building list. So add the .C file to the files list not the exe list in the make/files file!
	
	Info<< "Setting coloredFaces using sortedMesh techniques" << endl;
	volScalarField coloredFaces
	(
		IOobject
		(
			"coloredFaces",
			runTime.timeName(),
			mesh,
			IOobject::MUST_READ,
			IOobject::AUTO_WRITE
		),
		mesh
	);

	std::cout << "mesh.faceNeighbour().size() = " << mesh.faceNeighbour().size() << "\n";
	std::cout << "coloredFaces.size() = " << coloredFaces.size() << "\n";
/*
	for(double neighborFaceI = 0; neighborFaceI < mesh.faceNeighbour().size(); neighborFaceI++)
	{
		std::cout << neighborFaceI << ", ";
		coloredFaces[neighborFaceI] = 1;	//going to have to use some kind of getFace function where it just wants the face index, and it returns the value it is supposed to get. So colorBySortedValue
		//or we need something to return what is needed of the faces information or sorted information and do some kind of comparison. I almost think it would be better if it uses a face index to give a value like colorByZlayers or colorByYlayers or colorByXlayers
	}
	for(double neighborFaceI = mesh.faceNeighbour().size(); neighborFaceI < mesh.faces().size(); neighborFaceI++)
	{
		std::cout << neighborFaceI << ", ";
		coloredFaces[neighborFaceI] = 1;
	}
	coloredFaces.write();*/ 	//problem here is that it requires faces and I'm doing cells
	// guess I need to do a separate one for each patch, using the cell indices for the standard inside stuff

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
	

	// okay, I finally realized that even the z height above the mesh doesn't even get a
	// uniform value along a given z layer, and in fact the mesh isn't even boxes anymore,
	// causing the x,y points to be all funky if you use cell centers
	// the true way to convert to the locations is that I need what cell number in a given
	// direction the current coordinate is for, then I can divide out the point coordinate
	// and multiply by the cell number it is in a given x,y,z direction.
	// but OpenFoam does not appear to store the mesh in this way at all. They just care about
	// going from one cell to the next in calculations! So they don't care at all about 
	// a given cell number in a given x,y,z direction which is why stuff goes relatively fast!
	// So I either need to find something that does give me this information, like maybe the
	// foam to vtk stuff, or I need to get the number of cells using the points, neighbors,
	// owners, and boundaries which is basically writing a mesh class.
	// This will be tricky because even cell centers are off since none of the cells are true
	// boxes anymore, or at least they skew off in different directions.
	// you can get cell center info by running writeCellCentres.
	
	// another trick would be figuring out how to force renumberMesh to actually give nice
	// structured organization, even with the refined cells at the bottom of the mesh
	// I guess this would be turning those cells into some kind of separate block or something
	// I had errors just trying a straight renumberMesh to structured mesh, it wanted me to
	// go the other way and undo a renumberMesh!

/*
	Info<< "Setting x,y, and z Thresh values using structured i,j,k indices (which don't work after the mesh is refined and especially not after renumbering! Can get close if you put zeroes in to fill in the remaining cells, copying over the mesh from before refining)" << endl;
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
	zThresh.write();*/

	/*Info<< "Setting x,y, and z Thresh values as the cell centers of the internal mesh. Can also be adjusted to write out zThresh to be the cell center height above the zMin patch instead of referencing 0 height" << endl;
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

	volScalarField internalField_x(mesh.C().component(0));
	volScalarField internalField_y(mesh.C().component(1));
	//volScalarField internalField_z(mesh.C().component(2));
	volScalarField internalField_z = wallDist(mesh).y();	// adding a false here does not change things much

	forAll(internalField_z,cellI)
	{
		xThresh[cellI] = internalField_x[cellI];
		yThresh[cellI] = internalField_y[cellI];
		zThresh[cellI] = internalField_z[cellI];
	}
	xThresh.write();
	yThresh.write();
	zThresh.write();*/

/*
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
	zThresh.write();*/
	
/*
	Info<< "Setting z Thresh values to biggest unique neighbor face ID" << endl;

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
	
	const unallocLabelList & theOwners( mesh.owner() );
	const unallocLabelList & theNeighbors( mesh.neighbour() );
	std::vector<label> uniqueNeighbors(mesh.C().size(),99999);
	
	label neighborCellI = 0;
	uniqueNeighbors[0] = theNeighbors[0];
	for(label faceI = 1; faceI < mesh.nInternalFaces(); faceI++)
	{
		if(theOwners[faceI] == theOwners[faceI-1])
		{
			if(theNeighbors[faceI] < uniqueNeighbors[neighborCellI])
			{
				uniqueNeighbors[neighborCellI] = theNeighbors[faceI];
			}
		} else
		{
			if(uniqueNeighbors[neighborCellI] == 99999)
			{
				uniqueNeighbors[neighborCellI] = theNeighbors[faceI];
			}
			neighborCellI++;
		}
	}

	for(label cellI = 0; cellI < uniqueNeighbors.size(); cellI++)
	{
		zThresh[cellI] = uniqueNeighbors[cellI];
	}
	zThresh.write();*/

	Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
