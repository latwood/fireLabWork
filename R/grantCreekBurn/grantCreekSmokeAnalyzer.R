#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\GrantCreekBurn\\grantCreekSmokeAnalyzer.R')

rm(list=ls())

#IMPORTANT!!!! always call 'rm(list = ls())' before running on any files with different iteration counts and stuff. 
#All the old information is kept, so matrices still have the old values. This can throw off the plot dimensions
#Might need to delete some files in the directories before running this program!


#record start time
programstarttime <- proc.time()[3]

#import libraries
library(sp)
library(raster)
library(ggplot2)
library(scales)


#variables you can change
mainFileLocation <- "C:\\Users\\latwood\\Documents\\MyDocuments\\GrantCreekBurn"
graphWidth=2000
graphHeight=2000

esamplerFilenames <- "160411esampler1ButlerCreek.TXT"
esamplerFilenames[2] <- "160411esampler2GrantCreek.TXT"
esamplerBreaks <- "2 hours"
esamplerBreaks[2] <- "90 min"
esamplerLocation <- "G1: 46.96686, -114.04710"
esamplerLocation[2] <- "G5: 46.94225, -114.01180"
esamplerTimeZone <- "UTC"  #this is R's equivalent of "GMT"

dataramFilenames <- "160411grantcreekdataram1.TXT"
dataramFilenames[2] <- "160411grantcreekdataram2.TXT"
dataramFilenames[3] <- "160411grantcreekdataram3.TXT"
dataramBreaks <- "2 hours"
dataramBreaks[2] <- "75 min"
dataramBreaks[3] <- "30 min"
dataramLocation <- "G3: 46.98038, -114.02124"
dataramLocation[2] <- "G2: 46.99996, -114.01668"
dataramLocation[3] <- "G4: 46.98055, -114.01986"
dataramTimeZone <- Sys.timezone()

#maybe rename all these to be dataram/anemoeter,esampler, then have each column be a separate variable name that is needed

anemometerFileLocation <- "datafilesfromthegrantcreekanemometers"
extendedFilePath <- paste(mainFileLocation,"\\",anemometerFileLocation,sep="")

anemometerFilenames <- "Grant_Creek-67.csv"
anemometerFilenames[2] <- "Grant_Creek-69.csv"
anemometerFilenames[3] <- "Grant_Creek-72.csv"
anemometerFilenames[4] <- "Grant_Creek-94.csv"
anemometerFilenames[5] <- "Grant_Creek-100.csv"
anemometerBreaks <- "2 hours"
anemometerBreaks[2] <- "30 min"
anemometerBreaks[3] <- "90 min"
anemometerBreaks[4] <- "2 hours"
anemometerBreaks[5] <- "2 hours"
anemometerLocation <- "G2: 46.99996, -114.01668"
anemometerLocation[2] <- "G3: 46.98038, -114.02124"
anemometerLocation[3] <- "G5: 46.94225, -114.01180"
anemometerLocation[4] <- "G1: 46.96686, -114.04710"
anemometerLocation[5] <- "G4: 46.98055, -114.01986"
anemometerTimeZone <- "UTC"  #this is R's equivalent of "GMT"

cutoffConc <- 20		#micrograms/cubic meter
desiredTimeZone <- "America/Denver"
desiredOutputFormat <- "%a %H:%M:%S %Z"
burntime <- strptime("2016-04-11 12:30:00",format="%Y-%m-%d %H:%M:%S",tz=desiredTimeZone)

#Define Functions

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
	pushViewport(viewport(layout = grid.layout(nrow(layout)+1, ncol(layout),heights=unit(c(0.5*space,rep(5,nrow(layout))), "null"))))

	#add main title
	grid.text(title, vp = viewport(layout.pos.row = 1, layout.pos.col = 1:2),gp=gpar(fontsize=80,fontface="bold",lines=1))

    # Make each plot, in the correct location
    for (i in 1:numPlots) {
      # Get the i,j matrix positions of the regions that contain this subplot
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))

      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row+1,
                                      layout.pos.col = matchidx$col))
    }
  }
}

#this is for doing a lot of the individual plots, hopefully with less commands
singlePlotter <- function(theDataFrame,xValues,yValues,singleTitle,xTitle,yTitle,datebreaks,timezone=desiredTimeZone){
	thePlot <- ggplot(theDataFrame,aes(x=xValues,y=yValues)) + 
geom_point(size=3.5) + 
labs(title = singleTitle, x = xTitle, y = yTitle) + 
scale_x_datetime(date_breaks=datebreaks,labels=date_format("%a %H:%M",tz=timezone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),
axis.ticks.length=unit(12,"points"),axis.title=element_text(size=56),
axis.text.x = element_text(angle=90,vjust=0.5))
	return(thePlot)
}

timeZoneConverter <- function(time,format,inputTimeZone,desiredTimeZone){
	time <- as.POSIXct(time, format,tz=inputTimeZone)
	time <- format(time,tz=desiredTimeZone,usetz=TRUE)
	time <- strptime(time,format,tz=desiredTimeZone)
	return(time)
}

esamplerData <- list()
for (i in 1:length(esamplerFilenames))
{
	fun <- read.csv(file=paste(mainFileLocation,"\\",esamplerFilenames[i],sep=""),skip=6)  #the skip 6 lines is to skip the log report info at the beginning
	fun$Time <- strptime(fun$Time, "%d-%b-%Y %H:%M:%S", tz=esamplerTimeZone)
	N <- length(fun$Time)
	if (i == 1)
	{
		plotrange <- 2:(N-6)
		#plotrange <- 2:(N)
	} else
	{
		plotrange <- 1:(N-8)
		#plotrange <- 1:(N)
	}
	fun <- fun[plotrange,]

	#convert concentration to microgram units, convert windspeed to mph, convert pressure to atm
	fun$Conc.MG.M3. <- fun$Conc.MG.M3.*1000	#mg to ug
	fun$WS.M.S. <- fun$WS.M.S.*2.23694  #m/s to mph
	fun$BP.PA. <- fun$BP.PA.*9.86923e-6  #Pa to atm
	fun$Time <- timeZoneConverter(fun$Time,"%Y-%m-%d %H:%M:%S",esamplerTimeZone,desiredTimeZone)
	
	#could set up a separate color map scale so that all the concentration plots show
	#specific colors for specific ranges of pm 2.5 concentration
	theTitle <- paste(esamplerFilenames[i]," \nLocation",esamplerLocation[i])

	filename <- substr(esamplerFilenames[i],1,nchar(esamplerFilenames[i])-4)
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(fun,fun$Time,fun$Conc.MG.M3.,"pm 2.5 Concentration","Time",expression(bold(ug/m^3)),esamplerBreaks[i])
	p2 <- singlePlotter(fun,fun$Time,fun$WD.Deg.,"Wind Direction","Time","Degrees",esamplerBreaks[i])
	p3 <- singlePlotter(fun,fun$Time,fun$AT.C.,"Temperature","Time","°C",esamplerBreaks[i])
	p4 <- singlePlotter(fun,fun$Time,fun$WS.M.S.,"Wind Speed","Time","mph",esamplerBreaks[i])
	multiplot(p1, p2, p3, p4,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=3)
	dev.off()
	esamplerData[[i]] <- fun
	
}

#may want to split it up into different time ranges. The reason for the plot range not being the entire time is that
#there is a big gap and the next day has a few times where it does reading.

dataramData <- list()
for (i in 1:length(dataramFilenames))
{
	#there are two separate reads, one for the header and one for the rest of the stuff.
	#Need the header to get the time scales and such
	fun <- readLines(paste(mainFileLocation,"\\",dataramFilenames[i],sep=""))
	
	minimum <- 0
	maximum <- 0
	average <- 0

	Time <- 0
	starttime <- paste(substr(fun[5],41,48),substr(fun[5],nchar(fun[5])-9,nchar(fun[5])-2))
	starttime <- strptime(starttime, "%H:%M:%S %d/%m/%y", tz=dataramTimeZone)
	timeinterval <- as.numeric(substr(fun[11],nchar(fun[11])-3,nchar(fun[11])-2))
	
	Time <- starttime
	for (k in 29:length(fun))
	{
		startspot <- 0
		counter <- 0
		currentstring <- fun[k]
		if (currentstring == "")
		{
			next
		} else
		{
			for (j in 8:nchar(currentstring))
			{
				currentstringpart <- substr(currentstring,j,j)
				if (startspot == 0 && currentstringpart != " ")
				{
					startspot <- j
				}
				if (startspot != 0 && currentstringpart == "," && counter == 0)
				{
					if (k == 29)
					{
						minimum <- substr(currentstring,startspot,j-1)
					} else
					{
						minimum <- append(minimum,substr(currentstring,startspot,j-1))
					}
					startspot <- 0
					counter <- counter + 1
				} else if (startspot != 0 && currentstringpart == "," && counter == 1)
				{
					if (k == 29)
					{
						average <- substr(currentstring,startspot,j-1)
					} else
					{
						average <- append(average,substr(currentstring,startspot,j-1))
					}
					startspot <- 0
					counter <- counter + 1
				} else if (j == nchar(currentstring))
				{
					if (k == 29)
					{
						maximum <- substr(currentstring,startspot,j)
					} else
					{
						maximum <- append(maximum,substr(currentstring,startspot,j))
					}
				}
			}
			#this is right after the for loop, so still in the place for the lines where its not blank
			if (k > 29)
			{
				#this little section is the biggest slowdown so far, though it is faster now than it was
				Time[(k-27)/2] <- Time[(k-29)/2]
				Time[(k-27)/2]$sec <- Time[(k-29)/2]$sec+timeinterval
			}
		}
	}

	minimum <- as.numeric(minimum)
	average <- as.numeric(average)
	maximum <- as.numeric(maximum)

	filename <- substr(dataramFilenames[i],1,nchar(dataramFilenames[i])-4)
	theTitle <- paste(dataramFilenames[i]," \nLocation",dataramLocation[i])

	fun <- data.frame(Time,minimum,average,maximum)
	fun$Time <- timeZoneConverter(fun$Time,"%Y-%m-%d %H:%M:%S",dataramTimeZone,desiredTimeZone)
	
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(fun,fun$Time,fun$minimum,"Minimum ppm Conc.","Time",expression(bold(ug/m^3)),dataramBreaks[i])
	p2 <- singlePlotter(fun,fun$Time,fun$maximum,"Maximum ppm Conc.","Time",expression(bold(ug/m^3)),dataramBreaks[i])
	p3 <- singlePlotter(fun,fun$Time,fun$average,"Average ppm Conc.","Time",expression(bold(ug/m^3)),dataramBreaks[i])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=3)
	dev.off()
	dataramData[[i]] <- fun
}

anemometerData <- list()
for (i in 1:length(anemometerFilenames))
{
	fun <- read.csv(file=paste(extendedFilePath,"\\",anemometerFilenames[i],sep=""),skip=1)  #the skip 6 lines is to skip the log report info at the beginning
	for (q in 1:length(fun))
	{
		if (names(fun)[q] == "Wind.Direction..Ã.")
		{
			names(fun)[q] = "Wind.Direction..ø"
		}
	}
	if (i == 1)
	{
		fun$Date.Time..GMT.00.00 <- strptime(fun$Date.Time..GMT.00.00, "%m/%d/%Y %H:%M",tz=anemometerTimeZone)
	} else
	{
		fun$Date.Time..GMT.00.00 <- strptime(fun$Date.Time..GMT.00.00, "%m/%d/%y %I:%M:%S %p",tz=anemometerTimeZone)
	}
	fun$Date.Time..GMT.00.00 <- timeZoneConverter(fun$Date.Time..GMT.00.00,"%Y-%m-%d %H:%M:%S",anemometerTimeZone,desiredTimeZone)
	
	filename <- substr(anemometerFilenames[i],1,nchar(anemometerFilenames[i])-4)
	theTitle <- paste(anemometerFilenames[i]," \nLocation",anemometerLocation[i])

	plotOutput <- paste(extendedFilePath,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(fun,fun$Date.Time..GMT.00.00,fun$Wind.Speed..mph,"Wind Speed","Time","mph",anemometerBreaks[i])
	p2 <- singlePlotter(fun,fun$Date.Time..GMT.00.00,fun$Wind.Direction..ø,"Wind Direction","Time","Degrees",anemometerBreaks[i])
	p3 <- singlePlotter(fun,fun$Date.Time..GMT.00.00,fun$Gust.Speed..mph,"Gust Speed","Time","mph",anemometerBreaks[i])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=3)
	dev.off()
	anemometerData[[i]] <- fun
}


#now that I have a list of all the information, it is time to combine plots

#first plot temperature, pressure, and humidity. I think it would be best to assume that they all have the same condition and
#use G1 data for this
	filename <- "Atmospheric Conditions"
	theTitle <- "Atmospheric Conditions"

	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(esamplerData[[1]],esamplerData[[1]]$Time,esamplerData[[1]]$BP.PA.,"Pressure","Time","atm",esamplerBreaks[1])
	p2 <- singlePlotter(esamplerData[[1]],esamplerData[[1]]$Time,esamplerData[[1]]$RHi...,"Relative Humidity","Time","Percent",esamplerBreaks[1])
	p3 <- singlePlotter(esamplerData[[1]],esamplerData[[1]]$Time,esamplerData[[1]]$AT.C.,"Temperature","Time","°C",esamplerBreaks[1])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=2)
	dev.off()

#now plot G1 through G5 by combining the concentration data from the esamplers and datarams with the wind speed and wind 
#direction from the anemometers. I don't fully trust the wind speed and wind direction from the esamplers
	filename <- "G1"
	theTitle <- paste("Location",esamplerLocation[1])
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(anemometerData[[4]],anemometerData[[4]]$Date.Time..GMT.00.00,anemometerData[[4]]$Wind.Speed..mph,"Wind Speed","Time","mph",anemometerBreaks[4])
	p2 <- singlePlotter(esamplerData[[1]],esamplerData[[1]]$Time,esamplerData[[1]]$Conc.MG.M3.,"PM2.5 Concentration","Time",expression(bold(ug/m^3)),esamplerBreaks[1])
	p3 <- singlePlotter(anemometerData[[4]],anemometerData[[4]]$Date.Time..GMT.00.00,anemometerData[[4]]$Wind.Direction..ø,"Wind Direction","Time","Degrees",anemometerBreaks[4])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=2)
	dev.off()

	filename <- "G2"
	theTitle <- paste("Location",dataramLocation[2])
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(anemometerData[[1]],anemometerData[[1]]$Date.Time..GMT.00.00,anemometerData[[1]]$Wind.Speed..mph,"Wind Speed","Time","mph",anemometerBreaks[1])
	p2 <- singlePlotter(dataramData[[2]],dataramData[[2]]$Time,dataramData[[2]]$average,"PM2.5 Concentration","Time",expression(bold(ug/m^3)),dataramBreaks[2])
	p3 <- singlePlotter(anemometerData[[1]],anemometerData[[1]]$Date.Time..GMT.00.00,anemometerData[[1]]$Wind.Direction..ø,"Wind Direction","Time","Degrees",anemometerBreaks[1])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=2)
	dev.off()

	filename <- "G3"
	theTitle <- paste("Location",dataramLocation[1])
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(anemometerData[[2]],anemometerData[[2]]$Date.Time..GMT.00.00,anemometerData[[2]]$Wind.Speed..mph,"Wind Speed","Time","mph",anemometerBreaks[2])
	p2 <- singlePlotter(dataramData[[1]],dataramData[[1]]$Time,dataramData[[1]]$average,"PM2.5 Concentration","Time",expression(bold(ug/m^3)),dataramBreaks[1])
	p3 <- singlePlotter(anemometerData[[2]],anemometerData[[2]]$Date.Time..GMT.00.00,anemometerData[[2]]$Wind.Direction..ø,"Wind Direction","Time","Degrees",anemometerBreaks[2])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=2)
	dev.off()

	filename <- "G4"
	theTitle <- paste("Location",dataramLocation[3])
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(anemometerData[[5]],anemometerData[[5]]$Date.Time..GMT.00.00,anemometerData[[5]]$Wind.Speed..mph,"Wind Speed","Time","mph",anemometerBreaks[5])
	p2 <- singlePlotter(dataramData[[3]],dataramData[[3]]$Time,dataramData[[3]]$average,"PM2.5 Concentration","Time",expression(bold(ug/m^3)),dataramBreaks[3])
	p3 <- singlePlotter(anemometerData[[5]],anemometerData[[5]]$Date.Time..GMT.00.00,anemometerData[[5]]$Wind.Direction..ø,"Wind Direction","Time","Degrees",anemometerBreaks[5])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=2)
	dev.off()

	filename <- "G5"
	theTitle <- paste("Location",esamplerLocation[2])
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- singlePlotter(anemometerData[[3]],anemometerData[[3]]$Date.Time..GMT.00.00,anemometerData[[3]]$Wind.Speed..mph,"Wind Speed","Time","mph",anemometerBreaks[3])
	p2 <- singlePlotter(esamplerData[[2]],esamplerData[[2]]$Time,esamplerData[[2]]$Conc.MG.M3.,"PM2.5 Concentration","Time",expression(bold(ug/m^3)),esamplerBreaks[2])
	p3 <- singlePlotter(anemometerData[[3]],anemometerData[[3]]$Date.Time..GMT.00.00,anemometerData[[3]]$Wind.Direction..ø,"Wind Direction","Time","Degrees",anemometerBreaks[3])
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=2)
	dev.off()

#prepare a file to hold summary information, deleting the old file
plotOutput <- paste(mainFileLocation,"\\summaryInfo.txt", sep="")
sink(plotOutput)
cat(paste("Location","arrivalTime","departureTime","maxTime","maxConc(ug/m^3)","\n",sep=";"))
sink()

#now calculate the maximas for the esamplers
for (i in 1:length(esamplerData))
{
	maxValue <- max(esamplerData[[i]]$Conc.MG.M3.)
	maxTime <- 0
	arrivalTime <- 0
	departureTime <- 0
	for (j in 1:length(esamplerData[[i]]$Time))
	{
		if (maxTime == 0)
		{
			if (esamplerData[[i]]$Conc.MG.M3[j] == maxValue)
			{
				maxTime <- esamplerData[[i]]$Time[j]
			}
		}
		if (esamplerData[[i]]$Time[j] > burntime)  #this is because there is a fluke high concentration at the beginning before the burn
		{
			if (arrivalTime == 0 && j < (length(esamplerData[[i]]$Time)-3))
			{
				counter <- 0
				for (k in j:(j+4))
				{
					if (esamplerData[[i]]$Conc.MG.M3[k] >= cutoffConc)
					{
						counter <- counter + 1
					}
				}
				if (counter >= 2)
				{
					arrivalTime <- esamplerData[[i]]$Time[j]
				}
			}
		}
		if (departureTime == 0 && j < (length(esamplerData[[i]]$Time)-3))
		{
			counter <- 0
			for (k in j:(j+4))
			{
				if (esamplerData[[i]]$Conc.MG.M3[length(esamplerData[[i]]$Time)-k+1] >= cutoffConc)
				{
					counter <- counter + 1
				}
			}
			if (counter >= 2)
			{
				departureTime <- esamplerData[[i]]$Time[length(esamplerData[[i]]$Time)-j]
			}
		}
	}
	sink(plotOutput,append=TRUE)
	cat(paste(esamplerLocation[i],arrivalTime,departureTime,maxTime,maxValue,"\n",sep=";"))
	sink()
}

#now calculate the maximas for the datarams
for (i in 1:length(dataramData))
{
	maxValue <- max(dataramData[[i]]$average)
	maxTime <- 0
	arrivalTime <- 0
	departureTime <- 0
	for (j in 1:length(dataramData[[i]]$Time))
	{
		if (maxTime == 0)
		{
			if (dataramData[[i]]$average[j] == maxValue)
			{
				maxTime <- dataramData[[i]]$Time[j]
			}
		}
		if (dataramData[[i]]$Time[j] > burntime)
		{
			if (arrivalTime == 0 && j < (length(dataramData[[i]]$Time)-3))
			{
				counter <- 0
				for (k in j:(j+4))
				{
					if (dataramData[[i]]$average[k] >= cutoffConc)
					{
						counter <- counter + 1
					}
				}
				if (counter >= 2)
				{
					arrivalTime <- dataramData[[i]]$Time[j]
				}
			}
		}
		if (departureTime == 0 && j < (length(dataramData[[i]]$Time)-3))
		{
			counter <- 0
			for (k in j:(j+4))
			{
				if (dataramData[[i]]$average[length(dataramData[[i]]$Time)-k+1] >= cutoffConc)
				{
					counter <- counter + 1
				}
			}
			if (counter >= 2)
			{
				departureTime <- dataramData[[i]]$Time[length(dataramData[[i]]$Time)-j]
			}
		}
	}
	sink(plotOutput,append=TRUE)
	cat(paste(dataramLocation[i],arrivalTime,departureTime,maxTime,maxValue,"\n",sep=";"))
	sink()
}


#now add in headers for the anemometers
sink(plotOutput,append=TRUE)
cat(paste("Location","maxTime","maxWindSpeed(mph)","\n",sep=";"))
sink()

#now calculate the maximas for the anemometers
for (i in 1:length(anemometerData))
{
	maxValue <- max(anemometerData[[i]]$Wind.Speed..mph)
	maxTime <- 0
	for (j in 1:length(anemometerData[[i]]$Date.Time..GMT.00.00))
	{
		if (maxTime == 0)
		{
			if (anemometerData[[i]]$Wind.Speed..mph[j] == maxValue)
			{
				maxTime <- anemometerData[[i]]$Date.Time..GMT.00.00[j]
				break
			}
		}
	}
	sink(plotOutput,append=TRUE)
	cat(paste(anemometerLocation[i],maxTime,maxValue,"\n",sep=";"))
	sink()
}


#note: esamplers do not include a time zone, I've been using MDT, or the default for the region
#note: datarams do not include a time zone, I've been using MDT, or the default for the region, though it might be just getting confused and throwing that time zone in. I'm not sure how it is doing it and not messing stuff up for later
#note: anemometers include a time zone, GMT, and the output has been done as UTC which is equivalent. This might mean that while the data is recorded correctly, displaying it as recorded offsets the times.

#command to print a time in the other format, but for some odd reason it won't show the right time zone, grr
t <- as.POSIXct(esamplerData[[1]]$Time[1], "%Y-%m-%d %H:%M:%S",tz=esamplerTimeZone)
format(t,tz=desiredTimeZone,usetz=TRUE)





#the getmap function works by specifying a location in latitude and longitude that is the center of where the map is drawn
#the location needs accompanied by a zoom argument, an integer of 3 to 20, giving the spatial extent of the map
#can do a bounding box in terms of left/bottom/right/top instead of the above two parts
#okay you specify the source, then look up the maptype as given by that source
#SO getmap obtains the map, use ggmap to actually plot the map
#see this website to be able to read and use all the functionality better: https://journal.r-project.org/archive/2013-1/kahle-wickham.pdf