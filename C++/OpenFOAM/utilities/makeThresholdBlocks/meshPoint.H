#ifndef MESHPOINT_H
#define MESHPOINT_H

class meshPoint
{
public:

    //constructor
	meshPoint(double pointDepthValue);

private:

	double pointDepth;	// the x,y,z coordinate of the point, depending on what layer this point is for

};

#endif // MESHPOINT_H
