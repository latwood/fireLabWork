#include "WindNinjaRun.h"


//need to make this so that the input is each type of necessary option, to fill in all necessary variables
//figure out the heirarchy of what needs what to determine what is needed
WindNinjaRun::WindNinjaRun(std::string input_speed_value,std::string input_direction_value)
{
    num_threads = "4";
    initialization_method =	"domainAverageInitialization";
    time_zone = "America/Denver";
    input_speed = input_speed_value;
    input_speed_units = "mph";
    output_speed_units = "mph";
    input_direction = input_direction_value;
    input_wind_height = "10";
    units_input_wind_height = "m";
    output_wind_height = "10";
    units_output_wind_height = "m";
    vegetation = "grass";
    mesh_choice = "coarse";
    write_goog_output = "true";
    goog_out_resolution = "-1";
    units_goog_out_resolution = "m";
    write_vtk_output = "true";
    momentum_flag = "false";
    non_equilibrium_boundary_conditions = "false";

    windninjaPath = "/home/latwood/src/meshConversion/build-windninja-Desktop-Default/src/cli/WindNinja_cli";
    case_path = "/home/latwood/Downloads/case/";
    elevation_file_name = "grantCreek1st.tif";
    configName = "WindNinjaConfig.cfg";
    ninjaTailName = "WindNinjaDebug.txt";
    copy_path = "/home/latwood/Documents/smokeTransport/trialCopy/";

    new_case_name = input_speed + input_speed_units + input_direction + "deg/";

    output_path = case_path;
    elevation_file = case_path + elevation_file_name;

    writeConfigFile();
    runWindNinja();
    copyCase();
    cleanCase();

    message("finished simulation\n\n");
}

void WindNinjaRun::writeConfigFile()
{
    std::string writePath = case_path + configName;
    message("writing config file: " + writePath + ".\n");

    FILE *fzout;
    fzout = fopen(writePath.c_str(), "w");

    fprintf(fzout,"num_threads\t=\t%s\n",num_threads.c_str());
    fprintf(fzout,"elevation_file\t=\t%s\n",elevation_file.c_str());
    fprintf(fzout,"initialization_method\t=\t%s\n",initialization_method.c_str());
    fprintf(fzout,"time_zone\t=\t%s\n",time_zone.c_str());
    fprintf(fzout,"input_speed\t=\t%s\n",input_speed.c_str());
    fprintf(fzout,"input_speed_units\t=\t%s\n",input_speed_units.c_str());
    fprintf(fzout,"output_speed_units\t=\t%s\n",output_speed_units.c_str());
    fprintf(fzout,"input_direction\t=\t%s\n",input_direction.c_str());
    fprintf(fzout,"input_wind_height\t=\t%s\n",input_wind_height.c_str());
    fprintf(fzout,"units_input_wind_height\t=\t%s\n",units_input_wind_height.c_str());
    fprintf(fzout,"output_wind_height\t=\t%s\n",output_wind_height.c_str());
    fprintf(fzout,"units_output_wind_height\t=\t%s\n",units_output_wind_height.c_str());
    fprintf(fzout,"vegetation\t=\t%s\n",vegetation.c_str());
    fprintf(fzout,"mesh_choice\t=\t%s\n",mesh_choice.c_str());
    fprintf(fzout,"write_goog_output\t=\t%s\n",write_goog_output.c_str());
    fprintf(fzout,"goog_out_resolution\t=\t%s\n",goog_out_resolution.c_str());
    fprintf(fzout,"units_goog_out_resolution\t=\t%s\n",units_goog_out_resolution.c_str());
    fprintf(fzout,"write_vtk_output\t=\t%s\n",write_vtk_output.c_str());
    fprintf(fzout,"output_path\t=\t%s\n",output_path.c_str());
    //fprintf(fzout,"momentum_flag\t=\t%s\n",momentum_flag.c_str());
    //fprintf(fzout,"non_equilibrium_boundary_conditions\t=\t%s\n",non_equilibrium_boundary_conditions.c_str());

    fclose(fzout);

}

void WindNinjaRun::runWindNinja()
{
    message("now running windninja simulation\n");
    std::string runWindNinjaCommand = windninjaPath + " " + case_path+configName + " >& "
            + case_path+ninjaTailName;
    system(runWindNinjaCommand.c_str());
}

void WindNinjaRun::copyCase()
{
    message("now copying and renaming case\n");
    std::string copyCommand = "cp -r " + case_path + " " + copy_path + new_case_name;
    system(copyCommand.c_str());
}

void WindNinjaRun::cleanCase()
{
    message("now cleaning case\n");
    std::string cleanCommand = "find " + case_path + " ! -name '" + elevation_file_name + "' -type f -exec rm -f {} +";
    system(cleanCommand.c_str());
}

void WindNinjaRun::message(std::string theMessage)
{
    std::cout << theMessage;
    system("sleep 0.5");
}
