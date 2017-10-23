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
	
	/*
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
	*/
	
	findFaceLayers(theFaces,theOwners,theNeighbors.size());	//assumes that all boundary faces are held after all the internalField faces!!!
	// generateNewPointsList();
}

void sortedMesh::findFaceLayers(const faceList& theFaces, 
	const labelList& theOwners, double nInternalFaces)
{
	std::cout << "running findFaceLayers in sortedMesh class\n";
	// find what faces belong to each type of layer. Another function will sort the faces in a given layer
	
	std::vector<double> xFaceLayers_unsorted;
	std::vector<double> yFaceLayers_unsorted;
	std::vector<double> zFaceLayers_unsorted;
	
	double faceIndex = 0;
	double pastFaceIndex = 0;
	for(double j = 0; j < nInternalFaces-1; j++)
	{
		if(theOwners[j] == theOwners[j+1])
		{
			faceIndex++;
		} else
		{
			if(faceIndex == 2)
			{
				xFaceLayers_unsorted.push_back(j-2);
				yFaceLayers_unsorted.push_back(j-1);
				zFaceLayers_unsorted.push_back(j);
			} else if(faceIndex == 1)
			{
				if(pastFaceIndex == 2)
				{
					yFaceLayers_unsorted.push_back(j-1);
					zFaceLayers_unsorted.push_back(j);
				} else if(pastFaceIndex == 1)
				{
					xFaceLayers_unsorted.push_back(j-1);
					zFaceLayers_unsorted.push_back(j);
				} else if(pastFaceIndex == 0)
				{
					xFaceLayers_unsorted.push_back(j-1);
					yFaceLayers_unsorted.push_back(j);
				}
			} else if(faceIndex == 0)
			{
				if(pastFaceIndex == 1)
				{
					zFaceLayers_unsorted.push_back(j);
				} else if(pastFaceIndex == 0)
				{
					xFaceLayers_unsorted.push_back(j);
					xFaceLayers_unsorted.push_back(j+1);
				}
			}
			pastFaceIndex = faceIndex;
			faceIndex = 0;
		}
	}
	
	std::cout << "finished separating out x, y, and z layers\n";
	std::cout << "xFaceLayers_unsorted.size() = " << xFaceLayers_unsorted.size() << "\n";
	std::cout << "yFaceLayers_unsorted.size() = " << yFaceLayers_unsorted.size() << "\n";
	std::cout << "zFaceLayers_unsorted.size() = " << zFaceLayers_unsorted.size() << "\n";


	std::cout << "xFaceLayers_unsorted:\n";
	for(double j = 0; j < xFaceLayers_unsorted.size(); j++)
	{
		std::cout << " (" << theFaces[xFaceLayers_unsorted[j]][0] << "," 
			<< theFaces[xFaceLayers_unsorted[j]][1] << ","
			<< theFaces[xFaceLayers_unsorted[j]][2] << ","
			<< theFaces[xFaceLayers_unsorted[j]][3] << ")";
	}
	std::cout << "\n";
	
	std::cout << "yFaceLayers_unsorted:\n";
	for(double j = 0; j < yFaceLayers_unsorted.size(); j++)
	{
		std::cout << " (" << theFaces[yFaceLayers_unsorted[j]][0] << ","
			<< theFaces[yFaceLayers_unsorted[j]][1] << ","
			<< theFaces[yFaceLayers_unsorted[j]][2] << ","
			<< theFaces[yFaceLayers_unsorted[j]][3] << ")";
	}
	std::cout << "\n";
	
	std::cout << "zFaceLayers_unsorted:\n";
	for(double j = 0; j < zFaceLayers_unsorted.size(); j++)
	{
		std::cout << " (" << theFaces[zFaceLayers_unsorted[j]][0] << ","
			<< theFaces[zFaceLayers_unsorted[j]][1] << ","
			<< theFaces[zFaceLayers_unsorted[j]][2] << ","
			<< theFaces[zFaceLayers_unsorted[j]][3] << ")";
	}
	std::cout << "\n";
	
}
