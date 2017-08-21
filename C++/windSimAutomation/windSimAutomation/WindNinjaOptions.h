#ifndef WINDNINJAOPTIONS_H
#define WINDNINJAOPTIONS_H

class WindNinjaOptions
{
public:

    WindNinjaOptions();

private:

    //gall, if I'm still planning on writing a config file, probably should have a space insert variable that looks for the largest string size of all the options, and adds like 5 to that size, to use as a setw on the string output before the = and the variable values.

    std::string num_threads;                        // number of threads to use during simulation (arg=1)
    std::string elevation_file;                     // input elevation path/filename (*.asc,*.lcp, *.tif, *.img)

    std::string fetch_elevation;                    // download an elevation file from an internet server and save to path/filename
    std::string north;                              // north extent of elevation file bounding box to download
    std::string east;                               // east extent of elevation file bounding box to download
    std::string south;                              // south extent of elevation file bounding box to download
    std::string west;                               // west extent of elevation file bounding box to download
    std::string x_center;                           // x coordinate of center of elevation domain to download
    std::string y_center;                           // y coordinate of center of elevation domain to download
    std::string x_buffer;                           // x buffer of elevation domain to download (distance in east-west direction from center to edge of domain)
    std::string y_buffer;                           // y buffer of elevation domain to download (distance in north-south direction from center to edge of domain)
    std::string buffer_units;                       // units for x_buffer and y_buffer of elevation file to download (kilometers, miles)
    std::string elevation_source;                   // source for downloading elevation data (us_srtm, world_srtm, gmted)

    std::string initialization_method;              // initialization method (domainAverageInitialization, pointInitialization, wxModelInitialization)
    std::string time_zone;                          // time zone (common choices are: America/New_York, America/Chicago, America/Denver, America/Phoenix, America/Los_Angeles, America/Anchorage; use 'auto-detect' to try and find the time zone for the dem.  All choices are listed in date_time_zonespec.csv)

    std::string wx_model_type;                      // type of wx model to download (UCAR-NAM-CONUS-12-KM, UCAR-NAM-ALASKA-11-KM, UCAR-NDFD-CONUS-2.5-KM, UCAR-RAP-CONUS-13-KM, UCAR-GFS-GLOBAL-0.5-DEG, NOMADS-GFS-GLOBAL-0.25-DEG, NOMADS-HIRES-ARW-ALASKA-5-KM, NOMADS-HIRES-NMM-ALASKA-5-KM, NOMADS-HIRES-ARW-CONUS-5-KM, NOMADS-HIRES-NMM-CONUS-5-KM, NOMADS-NAM-ALASKA-11.25-KM, NOMADS-NAM-CONUS-12-KM, NOMADS-NAM-NORTH-AMERICA-32-KM, NOMADS-HRRR-CONUS-3-KM, NOMADS-RAP-CONUS-13-KM, NOMADS-RAP-NORTH-AMERICA-32-KM)
    std::string forecast_duration;                  // forecast duration to download (in hours)
    std::string forecast_filename;                  // path/filename of an already downloaded wx forecast file

    std::string match_points;                       // match simulation to points(true, false)

    std::string input_speed;                        // input wind speed
    std::string input_speed_units;                  // units of input wind speed (mps, mph, kph)
    std::string output_speed_units;                 // units of output wind speed (mps, mph, kph)
    std::string input_direction;                    // input wind direction
    std::string uni_air_temp;                       // surface air temperature
    std::string air_temp_units;                     // surface air temperature units (K, C, R, F)
    std::string uni_cloud_cover;                    // cloud cover
    std::string cloud_cover_units;                  // cloud cover units (fraction, percent, canopy_category)

    std::string wx_station_filename;                // path/filename of input wx station file
    std::string write_wx_station_kml;               // write a Google Earth kml file for the input wx stations (true, false)
    std::string wx_station_kml_filename;            // filename for the Google Earth kml wx station output file

    std::string input_wind_height;                  // height of input wind speed above the vegetation (U.S. standard is 20ft, outside is 10 m)
    std::string units_input_wind_height;            // units of input wind height (ft, m)
    std::string output_wind_height;                 // height of output wind speed above the vegetation (I recommend same as input, hard to keep consistent with GUI otherwise)
    std::string units_output_wind_height;           // units of output wind height (ft, m)
    std::string vegetation;                         // dominant type of vegetation (grass, brush, trees)

    std::string diurnal_winds;                      // include diurnal winds in simulation (true, false)
    std::string year;                               // year of simulation
    std::string month;                              // month of simulation
    std::string day;                                // day of simulation
    std::string hour;                               // hour of simulation
    std::string minute;                             // minute of simulation

    std::string mesh_choice;                        // mesh resolution choice (coarse, medium, fine)
    std::string mesh_resolution;                    // mesh resolution
    std::string units_mesh_resolution;              // mesh resolution units (ft, m)

    std::string output_buffer_clipping;             // percent to clip buffer on output files
    std::string write_wx_model_goog_output;         // write a Google Earth kmz output file for the raw wx model forecast (true, false)
    std::string write_goog_output;                  // write a Google Earth kmz output file (true, false)
    std::string goog_out_resolution;                // resolution of Google Earth output file (-1 to use mesh resolution)
    std::string units_goog_out_resolution;          // units of Google Earth resolution (ft, m)
    std::string write_wx_model_shapefile_output;    // write a shapefile output file for the raw wx model forecast (true, false)
    std::string write_shapefile_output;             // write a shapefile output file (true, false)
    std::string shape_out_resolution;               // resolution of shapefile output file (-1 to use mesh resolution)
    std::string units_shape_out_resolution;         // units of shapefile resolution (ft, m)
    std::string write_wx_model_ascii_output;        // write ascii fire behavior output files for the raw wx model forecast (true, false)
    std::string write_ascii_output;                 // write ascii fire behavior output files (true, false)
    std::string ascii_out_resolution;               // resolution of ascii fire behavior output files (-1 to use mesh resolution)
    std::string units_ascii_out_resolution;         // units of ascii fire behavior output file resolution (ft, m)
    std::string write_vtk_output;                   // write VTK output file (true, false)
    std::string write_farsite_atm;                  // write a FARSITE atm file (true, false)
    std::string write_pdf_output;                   // write PDF output file (true, false)
    std::string pdf_out_resolution;                 // resolution of pdf output file (-1 to use mesh resolution)
    std::string units_pdf_out_resolution;           // units of PDF resolution (ft, m)
    std::string pdf_linewidth;                      // width of PDF vectors (in pixels)
    std::string pdf_basemap;                        // background image of the geospatial pdf, default is topo map
    std::string pdf_height;                         // height of geospatial pdf
    std::string pdf_width;                          // width of geospatial pdf
    std::string pdf_size;                           // pre-defined pdf sizes (letter, legal, tabloid)

    std::string output_path;                        // path to where output files will be written

    std::string non_neutral_stability;              // use non-neutral stability (true, false)
    std::string alpha_stability;                    // alpha value for atmospheric stability

    std::string input_points_file;                  // input file containing lat,long,z for requested output points (z in m above ground)
    std::string output_points_file;                 // file to write containing output for requested points

    std::string existing_case_directory;            // path to an existing OpenFOAM case directory

    std::string momentum_flag;                      // use momentum solver (true, false)

    std::string number_of_iterations;               // number of iterations for momentum solver
    std::string mesh_count;                         // number of cells in the mesh
    std::string non_equilibrium_boundary_conditions;// use non-equilibrium boundary conditions for a momentum solver run (true, false)
    std::string input_speed_grid;                   // path/filename of input raster speed file (*.asc)
    std::string input_dir_grid;                     // path/filename of input raster dir file (*.asc)

};

#endif // WINDNINJAOPTIONS_H
