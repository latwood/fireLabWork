#ifndef foamPoint_H
#define foamPoint_H

class foamPoint
{
public:

    //constructor
	foamPoint(double xCoordValue, double yCoordValue, double zCoordValue);
	
	// set value functions
	void setListPositionDifference(double listPositionDifferenceValue);
	
	// get value functions
	double x();
	double y();
	double z();
	double listPositionDiff();
	
private:

	double xCoord;
	double yCoord;
	double zCoord;

	double listPositionDifference;

};

#endif // SORTEDMESH_H
