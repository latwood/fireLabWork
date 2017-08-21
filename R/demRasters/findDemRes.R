#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\FSwork\\smokeTransportVisualization\\googleEarth\\dwnloadLatLong\\findDemRes.R')
#source('/home/latwood/Documents/smokeTracer/findDemRes.R')

###import libraries
library(raster)

###variables you can change
mainPath <- "C:\\Users\\latwood\\FSwork\\smokeTransportVisualization\\googleEarth\\dwnloadLatLong\\"
#demPath <- "/home/latwood/Downloads/case/vshapedValley_notilt.tif"

numDems <- 3
demNames <- matrix(nrow=numDems)
demNames[1] <- paste(mainPath,"grantCreek1st.tif",sep="")
demNames[2] <- paste(mainPath,"windows3-1-4_town.tif",sep="")
demNames[3] <- paste(mainPath,"windows3-1-4_mountains.tif",sep="")


demRaster1 <- raster(demNames[1])
demRaster2 <- raster(demNames[2])
demRaster3 <- raster(demNames[3])	#note this is somehow zone 12 now
