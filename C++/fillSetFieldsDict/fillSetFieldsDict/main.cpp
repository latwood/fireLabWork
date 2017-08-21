
#include <iostream>

#include "editSetFieldsDict.h"

using namespace std;

int main()
{
    /*
     * Going to create a function that reads in the setFieldsDict file and replaces the regions section
     * with whatever you want. This will not be a function, since the idea is to be able to easily change
     * what you want in the regions section, using for loops and whatever is needed to get the right format
     * and values.
     *
     * With time, maybe this could include specific always want functions for setting the fields a certain way.
     *
     * To do this the easiest way, probably need to read in the entire file, saving the contents that are desired
     * into two or more chunks. A vector of chunks. Then print out a new file, using the chunks, but adding the
     * desired fill between each of the chunks. Save this new file over the old file.
     *
     * Sounds like a chunk is a class or struct. Or even just a long string. Whatever multiple lines of text is.
     * Or in fact, a vector of chunks is actually a vector of strings, each string being a line. Lines to be added
     * would not be included into this vector, and a separate vector of doubles would keep track of each spot
     * where the following line needs to be filled text. Then there is a vector of strings for each line of text
     * to be filled in.
     *
     * Maybe make this into a main function that can be called from the command line, with options being the
     * name and path of the setFieldsDict file of interest as one variable
     */

    /*
     * Probably need to change this so it is the main function calling a class that holds all the stuff for
     * reading and writing the file, then calls functions to read, then writes the file.
     * The stuff that gets inserted into the file should be a separate class or some kind of separate function,
     * the idea being that it can point to different sets of filler data, depending on the filling situation
     *
     * It would also be nice to add a class that can read in parts of the mesh, finding how the mesh is split up
     * to figure out the important dx, dy, dz, and startFace values. Could even figure out the constants used
     * for filling stuff for the refine mesh cells, which are based in how the cells were split.
     *
     * Could also write a separate class that reads in different mesh stuff, and reorders the mesh.
     * Then other functions could be created that use this reordered mesh, like a function to output
     * a new case with the mesh broken up into extra blocks for wind vector visualization
     */

    /*
     * Define a mesh class with all openfoam characteristics (classes of owners, neighbors and stuff)
     * but each of these mesh characteristics have cell indices; x, y, and z locations; and each of
     * the U, P, and T values. Generate missing values using an interpolate function
     *
     * The idea is that the mesh is read in and all the values are given, then there should be functions
     * that can use the information to figure out what indices are important for manipulating values
     *
     * After all this is defined from the constructor, have functions to do stuff with info like
     * output reordered mesh, set value, output mesh for paraview
     * Use native vs momentum cases to figure out how to reorganize mesh and how to correctly read in the mesh
     * could have a native vs momentum mesh, if it even becomes necessary to distinguish between the two types
     *
     * before this, figure out how to set each patch and internal field manually for each type of mesh,
     * then make the mesh class/classes
     */

    /*
     * Originally, I thought that reading in the setFieldsDict and replacing different sections was a good idea.
     * Now, I'm realizing that there are so few parts that would be left in the replacement process,
     * that it might just be a good idea to write the whole thing from scratch, generating just the key parts
     * that are capable of changing. This came up because my replace function missed that the setFieldsDict
     * still had lines for a source file, when the case didn't have a source file.
     *
     * So maybe always generate generic fields for each of the files where the user will have to add in their
     * own stuff, or this program generates needed parts. So all headers keep, but all sections leave blank, or
     * better yet, each section has a keyword so that the file can easily be read in and changed to fill in the
     * values.
     *
     * In the end, probably this mesh class will simply output all the folders and files that a mesh needs.
     * Should differentiate between the blank forms where the files just have headers and keywords
     * to be replaced, the keywords that for now are pretty much always used that will need to be used in the
     * replacing, and the new stuff that this program will add in as part of the replacing.
     *
     * This means that the whole program will probably want to be renamed at some point and the only needed
     * variable will be the path to the case
     *
     * I'm also realizing that this desire may not come to fruition with this program, but a second program
     * would be written that does the whole generate stuff from the mesh. In that case there would be no need
     * for a replace file information program as each of the files would be generated from the mesh.
     * While the replace function is not as useful as just writing the file, I will probably keep it in this
     * program for a bit longer since it at least works and this program is more for bruteforce
     * manipulating the different values of the setFieldsDict and needs a lot of user interaction anyways
     *
     */

    std::string setFieldsDictPath = "/home/latwood/Documents/energyEqn-old/recreateImprovedForMeeting/fillSetFieldsDict/twiceRefined/system/setFieldsDict";

    editSetFieldsDict fillSetFieldsDict(setFieldsDictPath);


    std::cout << "Finished updating " << setFieldsDictPath << " file" << endl;
    return 0;
}
