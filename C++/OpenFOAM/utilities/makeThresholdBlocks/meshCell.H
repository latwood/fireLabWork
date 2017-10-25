#ifndef meshCell_H
#define meshCell_H

#include <iostream>
#include <vector>

class meshCell
{
public:

	//constructor
	meshCell(double indexValue);
	
	void set_minZfaceID(double faceID);
	void set_maxZfaceID(double faceID);
	void set_westFaceID(double faceID);
	void set_eastFaceID(double faceID);
	void set_southFaceID(double faceID);
	void set_northFaceID(double faceID);

	void set_xlayer(double xlayerValue);
	void set_ylayer(double ylayerValue);
	void set_zlayer(double zlayerValue);
	
	double minZface();
	double maxZface();
	double westFace();
	double eastFace();
	double southFace();
	double northFace();
	
	double xID();
	double yID();
	double zID();
	
private:
	
	double index;
	
	double minZfaceID;
	double maxZfaceID;
	double westFaceID;
	double eastFaceID;
	double southFaceID;
	double northFaceID;
	
	double xlayer;
	double ylayer;
	double zlayer;

};

#endif // meshCell_H
