library(raster)

prj<-"+proj=utm +zone=12 +datum=WGS84 +units=m +no_defs +ellps=WGS84 +towgs84=0,0,0"
mainDir <- "/home/latwood/Downloads/"
tiffFileName <- paste(mainDir,"mesoscaleWeatherPaper2.tif",sep="")
demPlotFileName <- paste(mainDir,"mesoscaleWeatherPaper2-demPlot.png",sep="")
sidePlotFileName <- paste(mainDir,"mesoscaleWeatherPaper2-sideDemPlot.png",sep="")

h_p <- 1500
L_y <- 100000
S_x <- 9000
S_y <- 9000
X_1 <- 500
X_2 <- 9500
X_3 <- 10500
X_4 <- 19500


x0 <- -60000
xf <- 60000
y0 <- -200000
yf <- 100000
demRes <- 1000
initialHeight <- 0

# xzplotDimensions
sidePlot_xMin <- -30000
sidePlot_xMax <- 30000
sidePlot_zMin <- 0
sidePlot_zMax <- 4000
sidePlot_yPosition <- 1000


#====== functions =======================
hx <- function(x) {
  if (abs(x) <= X_1) {
    h_x <- 0
  } else if (X_1 < abs(x) && abs(x) < X_2) {
    h_x <- 0.5-0.5*cos((pi*(abs(x)-X_1))/S_x)
  } else if (X_2 <= abs(x) && abs(x) <= X_3) {
    h_x <- 1
  } else if(X_3 < abs(x) && abs(x) < X_4) {
    h_x <- 0.5+0.5*cos((pi*(abs(x)-X_3))/S_x)
  } else if(abs(x) >= X_4) {
    h_x <- 0
  }
  return (h_x)
}

hy <- function(y) {
  if (y <= -S_y) {
    h_y <- 0
  } else if (-S_y < y && y < 0) {
    h_y <- 0.5+0.5*cos(pi*y/S_y)
  } else if(y >= 0) {
    h_y <- 1
  }
  return (h_y)
}

# apparently raster stores stuff from top left going out, which is reverse of extent?
# so first row is at top, first col is at left. Adjust functions accordingly
addSlope <- function(theRaster) {
  for (r in 1:nrow(theRaster)){
    h_y <- hy(yf-(r-1)*demRes)
    for (c in 1:ncol(theRaster)){
      h_x <- hx(x0+(c-1)*demRes)
      v <- initialHeight + h_p*h_x*h_y
      theRaster[r,c] <- v
    }
  }
  return(theRaster)
}

getSidePlotValues <- function(theRaster, xMin, xMax, yPos) {
  
  startX <- 0
  endX <- 0
  
  for ( c in 1:ncol(theRaster)) {
    if ((x0+(c-1)*demRes) == xMin) {
      startX <- c
      print(paste("startX = ",startX,sep=""))
    } else if ((x0+(c-1)*demRes) == xMax) {
      endX <- c
      print(paste("endX = ",endX,sep=""))
    }
  }
  
  print(paste("endX-startX = ",(endX-startX),sep=""))
  theSidePlotValues <- matrix(nrow=(endX-startX))
  
  for ( r in 1:nrow(theRaster)) {
    if ((yf-(r-1)*demRes) == yPos) {
      for (c in startX:endX) {
        print(paste("c = ",c,", theRaster[r,c] = ",theRaster[r,c],sep=""))
        theSidePlotValues[c-startX] <- theRaster[r,c]
      }
      break
    }
  }
  
  return (theSidePlotValues)
}


m <- raster()
crs(m)<-prj
extent(m)<-extent(x0, xf, y0, yf)
res(m)<-demRes
m[] <- seq(from=10, to=1000, by=10)

s <- addSlope(m)

writeRaster(s, tiffFileName, overwrite=TRUE)
png(demPlotFileName)
plot(s)
dev.off()

zSidePlotValues <- getSidePlotValues(s, sidePlot_xMin, sidePlot_xMax, sidePlot_yPosition)
xSidePlotValues <- seq(from=sidePlot_xMin,to=sidePlot_xMax,length.out=length(zSidePlotValues))
plot(xSidePlotValues,zSidePlotValues,main="xz plot",type="o",pch=16,ylim=c(sidePlot_zMin,sidePlot_zMax))
png(sidePlotFileName)
plot(xSidePlotValues,zSidePlotValues,main="xz plot",type="o",pch=16,ylim=c(sidePlot_zMin,sidePlot_zMax))
dev.off()

