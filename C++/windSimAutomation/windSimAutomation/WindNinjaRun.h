#ifndef WINDNINJARUN_H
#define WINDNINJARUN_H

#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

class WindNinjaRun
{
public:

    WindNinjaRun();
    void runWindNinja();
    void runWindNinja(std::string ninjaTailName);
    void writeConfigFile();
    void copyCase();
    void cleanCase();

private:

    std::string num_threads;
    std::string elevation_file;
    std::string initialization_method;
    std::string time_zone;
    std::string input_speed;
    std::string input_speed_units;
    std::string output_speed_units;
    std::string input_direction;
    std::string input_wind_height;
    std::string units_input_wind_height;
    std::string output_wind_height;
    std::string units_output_wind_height;
    std::string vegetation;
    std::string mesh_choice;
    std::string write_goog_output;
    std::string goog_out_resolution;
    std::string units_goog_out_resolution;
    std::string write_vtk_output;
    std::string output_path;
    std::string momentum_flag;
    std::string non_equilibrium_boundary_conditions;

    std::string windninjaPath;
    std::string case_path;
    std::string elevation_file_name;
    std::string configName;
    std::string ninjaTailName;
    std::string copy_path;
    std::string new_case_name;

    void message(std::string theMessage);

};

#endif // WINDNINJARUN_H
