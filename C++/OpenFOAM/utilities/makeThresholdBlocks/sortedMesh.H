#ifndef sortedMesh_H
#define sortedMesh_H

#include <iostream>
#include <vector>

#include "fvCFD.H"

#include "foamPoint.H"
#include "meshPoint.H"
#include "meshCell.H"

class sortedMesh
{
public:

	/*
		Okay I need to figure out how I'm going to do this.
		My guess is that I need to take in the original list of all points, faces, owners
		neighbours, and boudaries, and I'm going to make a list of sorted points, 
		where each sorted point contains a position difference index to know how many places
		it has moved from the old list of points. Somehow this will give the right way for
		creating a sorted list of faces, owners, neighbours, and boundaries.
			
		The new list of points will be sorted in such a way that the cell indexing will go
		filling in the x direction all the way first, then increment one in the y direction,
		filling that x layer, incrementing in the y till all x layers are filled for a given
		z layer, then going up in the z layer and starting again

		Because the number of points/cells in a given layer will be changing because of refine
		mesh, a separate vector of x,y,z point layers will be needed

		I don't really see a way around assuming that the first point is the bottom left corner
		of the mesh. Especially since the mesh can go up or down from the first point.
		So to reorder the mesh, going to have to look at the x and y positions at a given z
		Maybe even use some kind of cell height or something, hm could sort first by cell height
		okay first it needs to increase in the x direction, so find everything of a single x
		direction and sort them by z. Or sort all points with a single x or y position by z
	
		#########
		Another important clue is that each face is connected to its neighbor and owner
		So should actually start by sorting the faces, by finding which faces have shared points
		So if two faces share the same z coordinate, they are connected at that point
		If the two faces share the same x coordinate, they are connected at that point
		Actually two faces should share two points. Depending on whether x, y, or z is the most
		large, it shows the orientation of the face or which layer it belongs to.

		Okay each two faces share two points, and the location of these two points in a given
		face show the face orientation. So if in one face the shared points are indices 1 and 2
		where the shared points are indices 3 and 4 (or something like this) then we know the
		face belongs to x layer (or maybe y layer or z layer, still need to check this).
		Once we have all the faces in a given layer direction, then we just sort by smallest
		to largest of one of the coordinates to get all the faces organized in order of layer
		then we just have to figure out the cell index number for the faces
		#########
	*/

    //constructor
	sortedMesh(const pointField& thePoints,
				const faceList& theFaces,
				const labelList& theOwners,
				const labelList& theNeighbors,
				const polyBoundaryMesh& theBoundaries);
	
	void message();

private:

	std::vector<foamPoint> sortedPoints;

	// end result values
	std::vector< std::vector<meshPoint> > xPointLayers;
	std::vector< std::vector<meshPoint> > yPointLayers;
	std::vector< std::vector<meshPoint> > zPointLayers;
	std::vector< std::vector<meshCell> > xCellLayers;
	std::vector< std::vector<meshCell> > yCellLayers;
	std::vector< std::vector<meshCell> > zCellLayers;

};

#endif // SORTEDMESH_H
