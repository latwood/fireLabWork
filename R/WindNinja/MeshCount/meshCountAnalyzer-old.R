#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\meshcount_project\\meshCountAnalyzer-old.R')


#the 'rm(list = ls())' command deletes everything in the workplace

rm(list=ls())

#IMPORTANT!!!! always call 'rm(list = ls())' before running on any files with different iteration counts and stuff. 
#All the old information is kept, so matrices still have the old values. This can throw off the plot dimensions
#Might need to delete some files in the directories before running this program!

#record start time
programstarttime <- proc.time()[3]

#import libraries
library(raster)
library(rasterVis)
library(grid)


#section for main variables that change each time

#this is the path to the main plot outputs. Is the folder tree for the different folder paths holding the files to be processed
mainFileLocation <- "C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\meshcount_project\\BigButteSmall\\"

#these are the individual folders in the folder tree holding each of the files to be processed.  Currently these names are used as plot titles as well
processingFolderNames <- "20,000 cells"
processingFolderNames[2] <- "50,000 cells"
processingFolderNames[3] <- "100,000 cells"
processingFolderNames[4] <- "Coarse"
processingFolderNames[5] <- "150,000 cells"
processingFolderNames[6] <- "200,000 cells"
processingFolderNames[7] <- "300,000 cells"
processingFolderNames[8] <- "400,000 cells"
processingFolderNames[9] <- "500,000 cells"
processingFolderNames[10] <- "Medium"
processingFolderNames[11] <- "600,000 cells"
processingFolderNames[12] <- "700,000 cells"
processingFolderNames[13] <- "800,000 cells"
processingFolderNames[14] <- "900,000 cells"
processingFolderNames[15] <- "1,000,000 cells"
processingFolderNames[16] <- "Fine"

#this sets up a numeric vector for the manipulated variable. Is a numeric representation of the individual plot titles used as the x axis for the residual plots
numericXlabels <- 20000
numericXlabels[2] <- 50000
numericXlabels[3] <- 100000
numericXlabels[4] <- 100000		#coarse
numericXlabels[5] <- 150000
numericXlabels[6] <- 200000
numericXlabels[7] <- 300000
numericXlabels[8] <- 400000
numericXlabels[9] <- 500000
numericXlabels[10] <- 500000		#medium
numericXlabels[11] <- 600000
numericXlabels[12] <- 700000
numericXlabels[13] <- 800000
numericXlabels[14] <- 900000
numericXlabels[15] <- 1000000
numericXlabels[16] <- 1000000	#fine


#these are the specific file names to be processed in each folder. Currently this is set up to be the same filenames across each folder
velocityAscFileName <- "big_butte_small_220_10_31m_vel.asc"
angleAscFileName <- "big_butte_small_220_10_31m_ang.asc"

#This is the folder name with the converged data to use for all comparisons
convergedName <- processingFolderNames[16]

#these are for controlling output plot size for all regular plots
graphHeight <- 2000
graphWidth <- 2500

#these are for controlling output plot size for all raster plots. Depending on the length and width of the elevation file of interest, this greatly affects general spacing for these plots
#I made the height smaller than the width for the salmon river. Everything else has so far been the same height and width
rasterGraphHeight <- 2000
rasterGraphWidth <- 2500


#section for main variables that don't change each time but could

#these are the raster plot set up values to give a uniform output look
#breaks determine where labels are placed on the legend
#labels are the specific labels for the legend
#colors are the specific colors for each break
#might need to change the units and values for velocity depending on what was analyzed but otherwise this seems to work really well.
angleBreaks <- c(0,40,80,120,160,200,240,280,320,360)
angleLabels <- c("0°","40°","80°","120°","160°","200°","240°","280°","320°","360°")
angleColors <- colorRampPalette(c("red","orange","yellow","forestgreen","green","cyan","lightblue","blue","purple"))(9)
velocityBreaks <- c(0,4,8,12,16,20)
velocityLabels <- c("0 mph","4 mph","8 mph","12 mph","16 mph","20 mph")
velocityColors <- colorRampPalette(c("blue","green","yellow","orange","red"))(5)
velocityDifferenceColors <- colorRampPalette(c("purple","blue","green","grey","white","brown","yellow","orange","red"))(9)
velocityDifferenceBreaks <- c(-20,-5,-3,-1.5,-0.5,0.5,1.5,3,5,20)
velocityDifferenceLabelBreaks <- c(seq(-20,20,length.out=10))
velocityDifferenceLabels <- c("-20 mph","-5 mph","-3 mph","-1.5 mph","-0.5 mph","0.5 mph","1.5 mph","3 mph","5 mph","20 mph")
angleDifferenceColors <- colorRampPalette(c("white","grey","brown","yellow","blue","green","orange","red"))(8)
angleDifferenceBreaks <- c(0,5,15,30,60,90,120,150,180)
angleDifferenceLabelBreaks <- c(seq(0,180,length.out=9))
angleDifferenceLabels <- c("0°","5°","15°","30°","60°","90°","120°","150°","180°")

#this is the number of files to be processed and controls a for loop during the program. It is important that numericXlabels and processingFolderNames are of the same length
filecount <- length(processingFolderNames)


#include functions in this section


#this is a function used by the arrow plotting stuff adapted from Natalie's windtools package
speed2u<-function(s,d){
    u <- -s * sin(d * pi/180)
    return (u) 
}

#this is a function used by the arrow plotting stuff adapted from Natalie's windtools package
speed2v<-function(s,d){
    v <- -s * cos(d * pi/180)
    return (v)
}

#this is a function used by the arrow plotting stuff adapted from Natalie's windtools package
uv2speed<-function(u,v){
    spd <- ((u*u+v*v)**0.5)
    return (spd)
}

#this is a function used by the arrow plotting stuff adapted from Natalie's windtools package
binSpeeds <- function(speedVector){
    b <- speedVector
    range <- 20

    b1 <- round((0.25 * range), digits = 1)
    b2 <- round((0.5 * range), digits = 1)
    b3 <- round((0.75 * range), digits = 1)
    min <- 0
    max <- 20
    for (i in 1:length(speedVector)){
        if (speedVector[i] < b1){
           b[i] <- paste(min, "-", b1)
        }
        else if(speedVector[i] < b2){
            b[i] <- paste0(b1, "-", b2)
        }
        else if(speedVector[i] < b3){
            b[i] <- paste0(b2, "-", b3)
        }
        else{ 
            (b[i] <- paste(b3, "-", max))
        }
    }
    b<-as.factor(b)
    order<-c(paste(b3, "-", max), paste0(b2, "-", b3), paste0(b1, "-", b2) ,paste(min, "-", b1))
    b <- factor(b, levels=order)
    return(b) 
}

mapPlot <- function(rVelocity,rAngle,theTitle){
	r.brick <- brick(rVelocity,rAngle)
	#coarsen if necessary for plotting
	r.brick <- aggregate(r.brick, fact=8, fun=mean)
	sp <- rasterToPoints(r.brick,spatial=TRUE)
	colnames(sp@data) <- c("spd","dir")
	crs <- CRS("+proj=longlat +datum=WGS84")
	sp <- spTransform(sp,crs)
	df <- sp@data
	df <- cbind(sp@coords,df)
	colnames(df) <- c("lon","lat","spd","dir")
	lat = (min(df$lat)+max(df$lat))/2
	lon = (min(df$lon)+max(df$lon))/2
	zoom = 13
	maptype = "terrain"
	p <- rasterToVectorMap(df,lat,lon,zoom,maptype,theTitle,axis_labels=FALSE)
	return(p)
}


#this is a function adapted from Natalie's wind tools for plotting with arrows
rasterToVectorMap <- function(df, lat, lon, zoom, maptype,theTitle="Main", colorscale='discrete',
                          axis_labels=FALSE){
    stopifnot(require("ggmap"))
    stopifnot(require("grid"))
    myMap<-get_map(location = c(lon=lon, lat=lat), zoom=zoom, maptype=maptype)

    p <- ggmap(myMap)


    if(colorscale=='discrete'){
        #scale u and v so that speed = 1, maintaining u:v ratio
        #this will allow us to plot vectors of equal length, but oriented in the correct direction
        u_scaled<-mapply(speed2u, 2, df$dir)
        v_scaled<-mapply(speed2v, 2, df$dir)
        speed_bracket <- binSpeeds(df$spd)
        df <- cbind(df, u_scaled, v_scaled, speed_bracket)
        p <- p + geom_segment(data=df, aes(x=lon+u_scaled/1500.0, y=lat+v_scaled/1500.0,
            xend = lon-u_scaled/1500.0, yend = lat-v_scaled/1500.0, 
            colour = speed_bracket), arrow = arrow(ends="first", length = unit(0.2, "cm")), size = 0.7) +
	    scale_colour_manual(values = c("red", "darkorange", "darkgreen", "blue"), name="Speed (mph)") +
		labs(title=theTitle)
	
    }
    else{
        p <- p + geom_segment(data=df, aes(x=lon+u/1500.0, y=lat+v/1500.0,
            xend = lon-u/1500.0, yend = lat-v/1500.0, 
            colour = obs_speed), arrow = arrow(ends="first", length = unit(0.2, "cm")), size = 0.7) +
            scale_colour_gradient(limits=c(min(df$spd),max(df$spd)), name="Speed (m/s)", low="blue", high="red")
    }
    p <- p + theme(plot.title=element_text(size=36))
    p <- p + theme(legend.title=element_text(size=28))
    p <- p + theme(legend.text=element_text(size = 24))
    #p <- p + guides(shape=guide_legend(override.aes=list(size=5)))
    p <- p + theme(legend.key.size=unit(2,"cm"))
    p <- p + theme(strip.text.x=element_text(size = 10))
    #p <- p + theme(axis.text.x = element_text(size=36))
    p <- p + theme(strip.text.y=element_text(size = 10))
    #p <- p + theme(axis.text.y = element_text(size=36))
    p <- p + xlab("") + ylab("")

    if(axis_labels == TRUE){
        p <- p + theme(axis.text.x = element_text(size = 16), axis.text.y = element_text(size = 16))
    }
    else{
        p <- p + theme(axis.text.x = element_blank())
        p <- p + theme(axis.ticks.x = element_blank())
        p <- p + theme(axis.text.y = element_blank())
        p <- p + theme(axis.ticks.y = element_blank())
    }

p <- p + theme(plot.margin=unit(c(0,0,0,0),"cm"))

    
    return(p)
}

#I got this function from online to help plot multiple ggplots in one spot!
multiplot <- function(..., plotlist=NULL, file,gHeight,gWidth, cols=1, layout=NULL,title="MAIN TITLE",space=1) {
  library(grid)

  # Make a list from the ... arguments and plotlist
  plots <- c(list(...), plotlist)

  numPlots = length(plots)

  # If layout is NULL, then use 'cols' to determine layout
  if (is.null(layout)) {
    # Make the panel
    # ncol: Number of columns of plots
    # nrow: Number of rows needed, calculated from # of cols
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                    ncol = cols, nrow = ceiling(numPlots/cols))
  }

 if (numPlots==1) {
    print(plots[[1]])

  } else {
    # Set up the page
    grid.newpage()
	#pushViewport(viewport(layout = grid.layout(nrow(layout)+1, ncol(layout),heights=unit(c(0.5*space,rep(5,nrow(layout))), "null"),widths=unit(c(rep(5,ncol(layout))), "null"))))
	pushViewport(viewport(layout = grid.layout(nrow(layout)+1, ncol(layout),heights=unit(c(0.5*space,rep(3,nrow(layout))), "null"))))

	#add main title
	grid.text(title, vp = viewport(layout.pos.row = 1, layout.pos.col = 2:3),gp=gpar(fontsize=80,fontface="bold",lines=1))

    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))

#      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row+1,
#                                      layout.pos.col = matchidx$col))
	print(plots[[i]], vp = viewport(layout.pos.row = matchidx$col+1,
                               layout.pos.col = matchidx$row))

    }
  }
}


#this function is for getting the current run time from the start time and the current time
getTime <- function(programstarttime){
	currenttime <- proc.time()[3]-programstarttime
	if (currenttime < 60)
	{
		cat(paste("The current runtime is:",currenttime,"seconds\n"))
	} else if (currenttime < 3600)
	{
		cat(paste("The current runtime is:",currenttime,"seconds =",currenttime/60,"minutes\n"))
	} else
	{
		cat(paste("The current runtime is:",currenttime,"seconds =",currenttime/3600,"hours\n"))
	}
}

#this is the formula for getting the difference between two angles, set up to work quickly for rasters
angleFinderReal <- function(x,y)
{
	a <- 360-abs(x - y)
	b <- abs(x-y)
	ind = which(a > b)
	z = a
	z[ind] <- b[ind]
	return(z)
}

#this gets the angle difference for rasters, looking at all the different values in a way that isn't as slow as other methods
angleFinder <- function(x,y,z) 
{
	bs <- blockSize(x)
	vv <- matrix(ncol=nrow(x), nrow=ncol(x))
	for (i in 1:bs$n)
	{
		v <- getValues(x,row=bs$row[i],nrows=bs$nrows[i])
		w <- getValues(y,row=bs$row[i],nrows=bs$nrows[i])
		difference <- angleFinderReal(v,w)
		cols <- bs$row[i]:(bs$row[i]+bs$nrows[i]-1)
		vv[,cols] <- matrix(difference, nrow=ncol(x))
		z <- setValues(z, as.vector(vv))
	}

	return(z)
}

#now run the program

#just setting up the converged information from input variables
convergedVelocityName <- paste(mainFileLocation,convergedName,"\\",velocityAscFileName, sep="")
convergedAngleName <- paste(mainFileLocation,convergedName,"\\",angleAscFileName, sep="")

#set up the converged rasters from the files
convergedVelocity <- raster(convergedVelocityName)
names(convergedVelocity) <- paste("Converged",convergedName)
rVelocityStack <- stack(convergedVelocity)
convergedAngle <- raster(convergedAngleName)
names(convergedAngle) <- paste("Converged",convergedName)


#now set up a loop that processes each of the files for each of these folders
plotMatrix=list()
maxVelocities <- 0
minVelocities <- 0
meanVelocities <- 0
for (i in 1:filecount)
{
	#see output location for debugging
	print(i)
	
	#set up rasters holding velocity and angle information from the files
	rVelocityName <- paste(mainFileLocation,processingFolderNames[i],"\\",velocityAscFileName, sep="")
	rAngleName <- paste(mainFileLocation,processingFolderNames[i],"\\",angleAscFileName, sep="")
	rVelocity <- raster(rVelocityName)
	names(rVelocity) <- processingFolderNames[i]
	rAngle <- raster(rAngleName)
	names(rAngle) <- processingFolderNames[i]

	maxVelocities[i] <- cellStats(rVelocity,stat="max")
	minVelocities[i] <- cellStats(rVelocity,stat="min")
	meanVelocities[i] <- cellStats(rVelocity,stat="mean")
	plotMatrix[[i]] <- mapPlot(rVelocity,rAngle,processingFolderNames[i])
	
	#this part of the program has completed, let the user know. Helps for debugging.
	cat(paste("The program has finished processing your",processingFolderNames[i],"file\n"))
	getTime(programstarttime)
}


cat("Now plotting windninja outputs\n")

filename <- "MeshCountResults"
theTitle <- "Mesh Count Results"
plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
multiplot(plotlist=plotMatrix,gHeight=graphHeight,gWidth=graphWidth, cols=4,title=theTitle,space=2)
dev.off()



cat("finished windninja plots\n")
getTime(programstarttime)

cat("Finished all plots\n")
cat("\nThe program has finished\n")

endtime <- proc.time()[3]-programstarttime
if (endtime < 60)
{
	cat(paste("The total program runtime is:",endtime,"seconds\n"))
} else if (endtime < 3600)
{
	cat(paste("The total program runtime is:",endtime/60,"minutes\n"))
} else
{
	cat(paste("The total program runtime is:",endtime/3600,"hours\n"))
}
cat("Have a nice day!\n\n")

line <- readline("Press [enter] or [space] to continue: ")