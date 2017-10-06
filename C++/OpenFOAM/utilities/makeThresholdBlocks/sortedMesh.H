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
	//creates all needed information for later functions
	
	double colorByXlayers(double faceI);
	double colorByYlayers(double faceI);
	double colorByZlayers(double faceI);

private:
	
	void findFaceLayers(const faceList& theFaces,double nInternalFaces);

	std::vector<double> xFaceLayers;
	std::vector<double> yFaceLayers;
	std::vector<double> zFaceLayers;
	
	/*
		color schemes for internal faceColors
		xlayers have AB, DC (CD), DA (AD), CB (BC)	give end value of 0
		ylayers have DA (AD), CB (BC), BA (AB), CD
		zlayers have BA (AB), CD, CB (BC), DA (AD)

		color scheme for external faceColors-hmm, for x and y layers, it is the same for both sides, just a different order of the points. Maybe should just sort them all together, and if the index is greater than the boundary, use that to tell them apart? Later yes, just do if statement on indices to determine if it is internal or external, then if statement to determine which face
		depending on the values of the points, it should be possible to sort by height above the mesh, or simply by index above the mesh in a given layer. Maybe have a separate value kept that is the index in the layer, then just do a search of that to determine which value to output
		interesting, the same thing appears in the zlayers

		xlayers west face have AD, BC, BA (AB), CD  give end value of 
		xlayers east face have AB, DC (CD), DA (AD), CB (BC)  give end value of
		ylayers south face have BA (AB), CD, CB (BC), DA (AD)
		ylayers north face have DA (AD), CB (BC), BA (AB), CD
		zlayers minZ have CB (BC), DA (AD), BA (AB), CD
		zlayers maxZ have BA (AB), CD, CB (BC), DA (AD)

		okay, lets rearrange to make it easier to organize into my if statements
		xlayers have AB, AD, BC, CD 
		ylayers have AB, AD, BC, CD
		zlayers have AB, AD, BC, CD
		um, they all match the same way!
		So there aren't any AC or BD?
		
		So I guess this means it would be easier to sort them all by smallest of 1st, 2nd, 3rd, or 4th points. No this wouldn't work because each point actually is a set of x,y,z
		need something a bit more complex
		
		
		okay, lets not do any of the boundaries and just have all those be zero gradient
		assuming the first point is the bottom left corner, the faces are xlayers in a
		given zlayer 
			if 1 == B and 4 == C
		a given xlayer face is above another xlayer face 
			if 3 == b and 4 == a
		
		the faces are ylayers in a given zlayer
			if 3 == B and 4 == A
		a given ylayer face is above another ylayer face
			if 2 == a and 3 == d
		
		the faces are zlayers
			if 2 == A and 3 == D or (3 == b and 4 == a) watch this last part, it is the same as ylayers
		a given zlayer face is above another zlayer face, or in the same zlayer
			if 3 == b and 4 == a
		*/
		
	/*
	so a face is an xlayer if at any time, it is found that another internal face satisfies the following if statements, in which case both faces are xlayers
		if 1 == B and 4 == C or B == 1 and C == 4
		or 3 == b and 4 == a or b == 3 and a == 4
	a face is a ylayer if at any time, it is found that another internal face satisfies the following if statements, in which case both faces are ylayer faces
		if 3 == B and 4 == A or B == 3 and A == 4
		or 2 == a and 3 == d or a == 2 and d == 3
	a face is a zlayer if at any time, it is found that another internal face satisfies the following if statements, in which case both faces are zlayer faces
		if 2 == A and 3 == D or A == 2 and D == 3
		or 3 == b and 4 == a or b == 3 and a == 4
		
	hmm, since stuff is shared, it would be best to first search by the first line if statements, since those are all unique, then in a given layer do some kind of search with the second line if statement to determine if they are in the same zlayer, or in differing x or y layers. Then sort by smallest to largest of different point location criteria of some sort
	hmm, just realized that while it appeared that the first statement for zlayers was independent, it is actually the same one as the first statement for x.
	guess that means that I should use the first statement, but that it actually gets me xlayers with some zlayers, and ylayers with some zlayers. Then I need to repeat the process with some more if statements to determine if they are an xlayer or zlayer, or if they are a ylayer or a zlayer.
	*/
	
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
