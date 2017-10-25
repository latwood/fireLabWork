#include "meshCell.H"

meshCell::meshCell(double indexValue)
{
	index = indexValue;
	xlayer = 0;
	ylayer = 0;
	zlayer = 0;
}

void meshCell::set_minZfaceID(double faceID)
{
	minZfaceID = faceID;
}
void meshCell::set_maxZfaceID(double faceID)
{
	maxZfaceID = faceID;
}
void meshCell::set_westFaceID(double faceID)
{
	westFaceID = faceID;
}
void meshCell::set_eastFaceID(double faceID)
{
	eastFaceID = faceID;
}
void meshCell::set_southFaceID(double faceID)
{
	southFaceID = faceID;
}
void meshCell::set_northFaceID(double faceID)
{
	northFaceID = faceID;
}

	
void meshCell::set_xlayer(double xlayerValue)
{
	xlayer = xlayerValue;
}
void meshCell::set_ylayer(double ylayerValue)
{
	ylayer = ylayerValue;
}
void meshCell::set_zlayer(double zlayerValue)
{
	zlayer = zlayerValue;
}


double meshCell::minZface()
{
	return minZfaceID;
}
double meshCell::maxZface()
{
	return maxZfaceID;
}
double meshCell::westFace()
{
	return westFaceID;
}
double meshCell::eastFace()
{
	return eastFaceID;
}
double meshCell::southFace()
{
	return southFaceID;
}
double meshCell::northFace()
{
	return northFaceID;
}


double meshCell::xID()
{
	return xlayer;
}
double meshCell::yID()
{
	return ylayer;
}
double meshCell::zID()
{
	return zlayer;
}

