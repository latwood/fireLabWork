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
	
	faceColor_internal.resize(theNeighbors.size(),0);
	std::cout << "faceColor_internal.size() = " << faceColor_internal.size() << "\n";
	faceColor_boundary.resize(theFaces.size()-theNeighbors.size(),0);
	std::cout << "faceColor_boundary.size() = " << faceColor_boundary.size() << "\n";
	
	findFaceSharedPoints(theFaces,theNeighbors.size());
	// generateNewPointsList();
}

void sortedMesh::findFaceSharedPoints(const faceList& theFaces,double nInternalFaces)
{
	// first lets do the internal faces
	//okay need some kind of recursion. The first one needs compared with all others but itself
	// the second one needs compared with all others but the first one and itself, since the first one was already compared with it
	// the third one needs compared with all others but the first and second ones and itself, since the first and second ones have already compared themselves with it
	// the last one will not need to check on any others, since the second to last will have compared itself with the last one
	for(double j = 0; j < nInternalFaces-1; j++)	// all but the last one
	{
		for(double i = j+1; i < nInternalFaces; i++)	// all but the current one and those before
		{
			std::vector<double> matchingFaces;
			matchingFaces.push_back(j);
			matchingFaces.push_back(i);
			if(theFaces[j][0] == theFaces[i][1] || theFaces[j][1] == theFaces[i][0])
			{
				facesSharingABpoints_internal.push_back(matchingFaces);
				/*if(faceColor_internal[j] == -1)
				{
					faceColor_internal[j] = 0;
				} else if(faceColor_internal[j] == 0)
				{
				}
					faceColor_internal[i] = 0;*/
			}
			if(theFaces[j][0] == theFaces[i][2])
			{
				facesSharingACpoints_internal.push_back(matchingFaces);
			}
			if(theFaces[j][0] == theFaces[i][3])
			{
				facesSharingADpoints_internal.push_back(matchingFaces);
			}
			if(theFaces[j][1] == theFaces[i][2])
			{
				facesSharingBCpoints_internal.push_back(matchingFaces);
			}
			if(theFaces[j][1] == theFaces[i][3])
			{
				facesSharingBDpoints_internal.push_back(matchingFaces);
			}
			if(theFaces[j][2] == theFaces[i][3])
			{
				facesSharingCDpoints_internal.push_back(matchingFaces);
			}
		}
	}
	std::cout << "facesSharingABpoints_internal.size() = " 
			<< facesSharingABpoints_internal.size() << "\n";
	std::cout << "facesSharingACpoints_internal.size() = " 
			<< facesSharingACpoints_internal.size() << "\n";
	std::cout << "facesSharingADpoints_internal.size() = " 
			<< facesSharingADpoints_internal.size() << "\n";
	std::cout << "facesSharingBCpoints_internal.size() = " 
			<< facesSharingBCpoints_internal.size() << "\n";
	std::cout << "facesSharingBDpoints_internal.size() = " 
			<< facesSharingBDpoints_internal.size() << "\n";
	std::cout << "facesSharingCDpoints_internal.size() = " 
			<< facesSharingCDpoints_internal.size() << "\n";

	
	// now repeat the same for the boundaries
	for(double j = nInternalFaces; j < theFaces.size()-1; j++)	// all but the last one
	{
		for(double i = j+1; i < theFaces.size(); i++)	// all but the current one and those before
		{
			std::vector<double> matchingFaces;
			matchingFaces.push_back(j);
			matchingFaces.push_back(i);
			if(theFaces[j][0] == theFaces[i][1])
			{
				facesSharingABpoints_boundary.push_back(matchingFaces);
			}
			if(theFaces[j][0] == theFaces[i][2])
			{
				facesSharingACpoints_boundary.push_back(matchingFaces);
			}
			if(theFaces[j][0] == theFaces[i][3])
			{
				facesSharingADpoints_boundary.push_back(matchingFaces);
			}
			if(theFaces[j][1] == theFaces[i][2])
			{
				facesSharingBCpoints_boundary.push_back(matchingFaces);
			}
			if(theFaces[j][1] == theFaces[i][3])
			{
				facesSharingBDpoints_boundary.push_back(matchingFaces);
			}
			if(theFaces[j][2] == theFaces[i][3])
			{
				facesSharingCDpoints_boundary.push_back(matchingFaces);
			}
		}
	}
	std::cout << "facesSharingABpoints_boundary.size() = " 
			<< facesSharingABpoints_boundary.size() << "\n";
	std::cout << "facesSharingACpoints_boundary.size() = " 
			<< facesSharingACpoints_boundary.size() << "\n";
	std::cout << "facesSharingADpoints_boundary.size() = " 
			<< facesSharingADpoints_boundary.size() << "\n";
	std::cout << "facesSharingBCpoints_boundary.size() = " 
			<< facesSharingBCpoints_boundary.size() << "\n";
	std::cout << "facesSharingBDpoints_boundary.size() = " 
			<< facesSharingBDpoints_boundary.size() << "\n";
	std::cout << "facesSharingCDpoints_boundary.size() = " 
			<< facesSharingCDpoints_boundary.size() << "\n";

}
