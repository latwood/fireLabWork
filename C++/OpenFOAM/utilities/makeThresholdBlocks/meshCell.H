#ifndef MESHCELL_H
#define MESHCELL_H

class meshCell
{
public:

    //constructor
	meshCell(double cellDepthValue);

private:

	double cellDepth;	// the x,y,z coordinate of the cell, depending on what layer this cell is for

};

#endif // MESHCELL_H
