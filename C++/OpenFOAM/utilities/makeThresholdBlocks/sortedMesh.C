#include "sortedMesh.H"

sortedMesh::sortedMesh(const pointField& thePoints,
						const faceList& theFaces,
						const labelList& theOwners,
						const labelList& theNeighbors,
						const polyBoundaryMesh& theBoundaries)
{
	std::cout << "constructing for sortedMesh class from polyMesh points,faces,owners,neighbours, and boundaries\n";
	std::cout << "thePoints.size() = " << thePoints.size() << "\n";
	std::cout << "theFaces.size() = " << theFaces.size() << "\n";
	std::cout << "theOwners.size() = " << theOwners.size() << "\n";
	std::cout << "theNeighbors.size() = " << theNeighbors.size() << "\n";
	std::cout << "theBoudaries.size() = " << theBoundaries.size() << "\n";
	
	std::cout << "thePoints[0][0] = " << thePoints[0][0] << "\n";
	std::cout << "thePoints[0][1] = " << thePoints[0][1] << "\n";
	std::cout << "thePoints[0][2] = " << thePoints[0][2] << "\n";
	std::cout << "theFaces[0][0] = " << theFaces[0][0] << "\n";
	std::cout << "theFaces[0][1] = " << theFaces[0][1] << "\n";
	std::cout << "theFaces[0][2] = " << theFaces[0][2] << "\n";
	std::cout << "theFaces[0][3] = " << theFaces[0][3] << "\n";
	std::cout << "theOwners[0] = " << theOwners[0] << "\n";
	std::cout << "theNeighbors[0] = " << theNeighbors[0] << "\n";
	std::cout << "theBoundaries[0][0][0] = " << theBoundaries[0][0][0] << "\n";
	
	findFaceSharedPoints(theFaces,theNeighbors.size()-theBoundaries.size());
	// generateNewPointsList();
}

void sortedMesh::findFaceSharedPoints(const faceList& theFaces,double nInternalFaces)
{

}
