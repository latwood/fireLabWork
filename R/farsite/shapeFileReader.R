#command to run this program (includes the current file location)
#source('/home/latwood/src/fireLabWork/R/farsite/shapeFileReader.R')

###import libraries
library(maptools)
library(RColorBrewer)
library(ggmap)


###variables you can change
mainPath <- "/home/latwood/Documents/ne_10m_parks_and_protected_lands/"
#mainPath <- "/home/latwood/Documents/windninjaPanther/test1177973/currentInput/"
shapeFilePath_shp <- paste(mainPath,"ne_10m_parks_and_protected_lands_area.shp",sep="")
#shapeFilePath_shp <- paste(mainPath,"Documents/windninjaPanther/test1177973/currentInput/test1177973_NTFBIgnition.shp",sep="")
#shapeFilePath_shx <- paste(mainPath,"Documents/windninjaPanther/test1177973/currentInput/test1177973_NTFBIgnition.shx",sep="")
#shapeFilePath_dbf <- paste(mainPath,"Documents/windninjaPanther/test1177973/currentInput/test1177973_NTFBIgnition.dbf",sep="")
#shapeFilePath_prj <- paste(mainPath,"Documents/windninjaPanther/test1177973/currentInput/test1177973_NTFBIgnition.prj",sep="")

colors <- brewer.pal(9, "BuGn")

### functions needed for different parts


### now make things happen

# choose one of these two options. Note this command only looks to work on the .shp file. 
# Not sure if it reads all the other separate files too or now]t
area <- readShapePoly(shapeFilePath_shp)
# area <- readShapePoly(file.choose())

# create map image
mapImage <- get_map(location = c(lon = -118, lat = 37.5),
                    color = "color",
                    source = "osm",
                    # maptype = "terrain",
                    zoom = 6)

# convert this strange shape file format into a dataframe that ggmap likes to work with. fortify is a ggplot2 package function
area.points <- fortify(area)

# now map the shape file
ggmap(mapImage) +
  geom_polygon(aes(x = long,
                   y = lat,
                   group = group),
               data = area.points,
               color = colors[9],
               fill = colors[6],
               alpha = 0.5) +
  labs(x = "Longitude",
       y = "Latitude")

