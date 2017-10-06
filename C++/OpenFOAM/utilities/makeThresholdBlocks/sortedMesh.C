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
	
	findFaceLayers(theFaces,theNeighbors.size());	//assumes that all boundary faces are held after all the internalField faces!!!
	// generateNewPointsList();
}

void sortedMesh::findFaceLayers(const faceList& theFaces,double nInternalFaces)
{
	// find what faces belong to each type of layer. Another function will sort the faces in a given layer

	//okay need some kind of recursion. The first one needs compared with all others but itself
	// the second one needs compared with all others but the first one and itself, since the first one was already compared with it
	// the third one needs compared with all others but the first and second ones and itself, since the first and second ones have already compared themselves with it
	// the last one will not need to check on any others, since the second to last will have compared itself with the last one
	// should have a bool for each face telling whether it has already been assigned to a layer or not. Still need to check if only one of the faces has been assigned, but if both have, we are done and need to move on
	
	std::vector<double> xFaceLayers_unsorted;
	std::vector<double> yFaceLayers_unsorted;
	std::vector<double> zFaceLayers_unsorted;
	
	//impossible to separate the zlayers from the x or y layers, so first sort by x and y layers, throwing in the zlayers
	
	std::vector<bool> assignedToLayer(nInternalFaces,false);
	std::vector<double> xzFaceLayers;
	std::vector<double> yzFaceLayers;

	for(double j = 0; j < nInternalFaces-1; j++)	// all but the last one
	{
		for(double i = j+1; i < nInternalFaces; i++)	// all but the current one and those before
		{
			if(assignedToLayer[j] != true && assignedToLayer[i] != true)
			{
				if((theFaces[j][0] == theFaces[i][1]
					&& theFaces[j][3] == theFaces[i][2])
					|| (theFaces[j][1] == theFaces[i][0]
					&& theFaces[j][2] == theFaces[i][3]))
				{
					if(assignedToLayer[j] == false)
					{
						xzFaceLayers.push_back(j);
						assignedToLayer[j] = true;
					}
					if(assignedToLayer[i] == false)
					{
						xzFaceLayers.push_back(i);
						assignedToLayer[i] = true;
					}
				} else if((theFaces[j][2] == theFaces[i][1]
					&& theFaces[j][3] == theFaces[i][0])
					|| (theFaces[j][1] == theFaces[i][2]
					&& theFaces[j][0] == theFaces[i][3]))
				{
					if(assignedToLayer[j] == false)
					{
						yzFaceLayers.push_back(j);
						assignedToLayer[j] = true;
					}
					if(assignedToLayer[i] == false)
					{
						yzFaceLayers.push_back(i);
						assignedToLayer[i] = true;
					}
				} else
				{
					std::cout << "not sure how, but faces weren't assigned to layers!\n";
					std::cout << "j = " << j << ", i = " << i << "\n";
				}
			}
		}
	}
	
	std::cout << "xzFaceLayers.size() = " << xzFaceLayers.size() << "\n";
	std::cout << "yzFaceLayers.size() = " << yzFaceLayers.size() << "\n";
	
	// now go through and separate out the zlayers with a similar process
	
	std::vector<bool> assignedToLayer_xz(xzFaceLayers.size(),false);
	
	for(double j = 0; j < xzFaceLayers.size()-1; j++)	// all but the last one
	{
		for(double i = j+1; i < xzFaceLayers.size(); i++)	// all but the current one and those before
		{
			if(assignedToLayer_xz[j] != true && assignedToLayer_xz[i] != true)
			{
				if((theFaces[xzFaceLayers[j]][2] == theFaces[xzFaceLayers[i]][1]
					&& theFaces[xzFaceLayers[j]][3] == theFaces[xzFaceLayers[i]][0])
					|| (theFaces[xzFaceLayers[j]][1] == theFaces[xzFaceLayers[i]][2]
					&& theFaces[xzFaceLayers[j]][0] == theFaces[xzFaceLayers[i]][3]))
				{
					if(assignedToLayer_xz[j] == false)
					{
						xFaceLayers_unsorted.push_back(xzFaceLayers[j]);
						assignedToLayer_xz[j] = true;
					}
					if(assignedToLayer_xz[i] == false)
					{
						xFaceLayers_unsorted.push_back(xzFaceLayers[i]);
						assignedToLayer_xz[i] = true;
					}
				} else 
				{
					if(assignedToLayer_xz[j] == false)
					{
						zFaceLayers_unsorted.push_back(xzFaceLayers[j]);
						assignedToLayer_xz[j] = true;
					}
					if(assignedToLayer_xz[i] == false)
					{
						zFaceLayers_unsorted.push_back(xzFaceLayers[i]);
						assignedToLayer_xz[i] = true;
					}
				}
			}
		}
	}
	
	std::cout << "xFaceLayers_unsorted.size() = " << xFaceLayers_unsorted.size() << "\n";
	std::cout << "zFaceLayers_unsorted.size() = " << zFaceLayers_unsorted.size() << "\n";
	
	// now go through and separate out the zlayers with a similar process
	
	std::vector<bool> assignedToLayer_yz(yzFaceLayers.size(),false);
	
	for(double j = 0; j < yzFaceLayers.size()-1; j++)	// all but the last one
	{
		for(double i = j+1; i < yzFaceLayers.size(); i++)	// all but the current one and those before
		{
			if(assignedToLayer_yz[j] != true && assignedToLayer_yz[i] != true)
			{
				if((theFaces[yzFaceLayers[j]][2] == theFaces[yzFaceLayers[i]][1]
					&& theFaces[yzFaceLayers[j]][3] == theFaces[yzFaceLayers[i]][0])
					|| (theFaces[yzFaceLayers[j]][1] == theFaces[yzFaceLayers[i]][2]
					&& theFaces[yzFaceLayers[j]][0] == theFaces[yzFaceLayers[i]][3]))
				{
					if(assignedToLayer_yz[j] == false)
					{
						yFaceLayers_unsorted.push_back(yzFaceLayers[j]);
						assignedToLayer_yz[j] = true;
					}
					if(assignedToLayer_yz[i] == false)
					{
						yFaceLayers_unsorted.push_back(yzFaceLayers[i]);
						assignedToLayer_yz[i] = true;
					}
				} else 
				{
					if(assignedToLayer_yz[j] == false)
					{
						zFaceLayers_unsorted.push_back(yzFaceLayers[j]);
						assignedToLayer_yz[j] = true;
					}
					if(assignedToLayer_yz[i] == false)
					{
						zFaceLayers_unsorted.push_back(yzFaceLayers[i]);
						assignedToLayer_yz[i] = true;
					}
				}
			}
		}
	}
	
	std::cout << "yFaceLayers_unsorted.size() = " << yFaceLayers_unsorted.size() << "\n";
	std::cout << "zFaceLayers_unsorted.size() = " << zFaceLayers_unsorted.size() << "\n";
	
}
