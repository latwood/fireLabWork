#include "foamPoint.H"

foamPoint::foamPoint(double xCoordValue, double yCoordValue, double zCoordValue)
{
	xCoord = xCoordValue;
	yCoord = yCoordValue;
	zCoord = zCoordValue;
	
	listPositionDifference = 0;
}

void foamPoint::setListPositionDifference(double listPositionDifferenceValue)
{
	listPositionDifference = listPositionDifferenceValue;
}

double foamPoint::x()
{
	return xCoord;
}

double foamPoint::y()
{
	return yCoord;
}

double foamPoint::z()
{
	return zCoord;
}

double foamPoint::listPositionDiff()
{
	return listPositionDifference;
}

