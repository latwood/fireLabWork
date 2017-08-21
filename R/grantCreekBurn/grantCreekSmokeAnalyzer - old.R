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
esamplerHourBreaks <- "2 hours"
esamplerHourBreaks[2] <- "2 hours"
esamplerLocation <- "G1: 46.96686, -114.04710"
esamplerLocation[2] <- "G5: 46.94225, -114.01180"
esamplerTimeZone <- Sys.timezone()

dataramFilenames <- "160411grantcreekdataram1.TXT"
dataramFilenames[2] <- "160411grantcreekdataram2.TXT"
dataramFilenames[3] <- "160411grantcreekdataram3.TXT"
dataramHourBreaks <- "2 hours"
dataramHourBreaks[2] <- "1 hours"
dataramHourBreaks[3] <- "1 hours"
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
anemometerHourBreaks <- "2 hours"
anemometerHourBreaks[2] <- "1 hours"
anemometerHourBreaks[3] <- "2 hours"
anemometerHourBreaks[4] <- "2 hours"
anemometerHourBreaks[5] <- "2 hours"
anemometerLocation <- "G2"
anemometerLocation[2] <- "G3"
anemometerLocation[3] <- "G5"
anemometerLocation[4] <- "G1"
anemometerLocation[5] <- "G4"
anemometerTimeZone <- "UTC"  #this is R's equivalent of "GMT"


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

datebreakfinder <- function(times,breakcount){
	#define variables that can be changed
	locationcheckrange <- 20
	allowedDifference <- 1

	datebreaks <- seq(times[1],times[length(times)],length.out=breakcount)
	relativeLocations <- seq(1:length(times),length.out=breakcount)
	relativeLocations[2:breakcount] <- relativeLocations[2:breakcount]-locationcheckrange
	for (j in 1:breakcount)
	{
		for (i in 1:locationcheckrange)
		{
		}
	}
			
}

for (i in 1:length(esamplerFilenames))
{
	fun <- read.csv(file=paste(mainFileLocation,"\\",esamplerFilenames[i],sep=""),skip=6)  #the skip 6 lines is to skip the log report info at the beginning
	fun$Time <- strptime(fun$Time, "%d-%b-%Y %H:%M:%S")
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

	#could set up a separate color map scale so that all the concentration plots show
	#specific colors for specific ranges of pm 2.5 concentration
	theTitle <- paste(esamplerFilenames[i]," \nFrom",format(fun$Time[1],"%H:%M %m/%d/%y"),"to",format(fun$Time[length(fun$Time)],"%H:%M %m/%d/%y")," \nLocation",esamplerLocation[i])
	fun$Time$zone <- "UTC"

	filename <- substr(esamplerFilenames[i],1,nchar(esamplerFilenames[i])-4)
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- ggplot(fun,aes(x=Time,y=Conc.MG.M3.))+geom_point(size=3.5) + 
labs(title = "pm 2.5 Concentration", x = "Time", y = expression(bold(mg/m^3))) + 
scale_x_datetime(date_breaks=esamplerHourBreaks[i],labels=date_format("%H:%M",tz=esamplerTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	p2 <- ggplot(fun,aes(x=Time,y=WD.Deg.))+geom_point(size=3.5) + 
labs(title = "Wind Direction", x = "Time", y = "Degrees") + 
scale_x_datetime(date_breaks=esamplerHourBreaks[i],labels=date_format("%H:%M",tz=esamplerTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	p3 <- ggplot(fun,aes(x=Time,y=AT.C.))+geom_point(size=3.5) + 
labs(title = "Temperature", x = "Time", y = "°C") + 
scale_x_datetime(date_breaks=esamplerHourBreaks[i],labels=date_format("%H:%M",tz=esamplerTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	p4 <- ggplot(fun,aes(x=Time,y=WS.M.S.))+geom_point(size=3.5) + 
labs(title = "Wind Speed", x = "Time", y = "m/s") + 
scale_x_datetime(date_breaks=esamplerHourBreaks[i],labels=date_format("%H:%M",tz=esamplerTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	multiplot(p1, p2, p3, p4,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=5)
	dev.off()
}

#may want to split it up into different time ranges. The reason for the plot range not being the entire time is that
#there is a big gap and the next day has a few times where it does reading.

for (i in 1:length(dataramFilenames))
{
	#there are two separate reads, one for the header and one for the rest of the stuff.
	#Need the header to get the time scales and such
	fun <- readLines(paste(mainFileLocation,"\\",dataramFilenames[i],sep=""))
	
	minimum <- 0
	maximum <- 0
	average <- 0

	time <- 0
	starttime <- paste(substr(fun[5],41,48),substr(fun[5],nchar(fun[5])-9,nchar(fun[5])-2))
	starttime <- strptime(starttime, "%H:%M:%S %d/%m/%y")
	timeinterval <- as.numeric(substr(fun[11],nchar(fun[11])-3,nchar(fun[11])-2))
	
	time <- starttime
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
				time[(k-27)/2] <- time[(k-29)/2]
				time[(k-27)/2]$sec <- time[(k-29)/2]$sec+timeinterval
			}
		}
	}

	N <- length(time)
	if (i == 1)
	{
		plotrange <- 1:N
	} else
	{
		plotrange <- 1:N
	}

	minimum <- as.numeric(minimum)
	average <- as.numeric(average)
	maximum <- as.numeric(maximum)

	filename <- substr(dataramFilenames[i],1,nchar(dataramFilenames[i])-4)
	theTitle <- paste(dataramFilenames[i]," \nFrom",format(time[1],"%H:%M %m/%d/%y"),"to",format(time[length(time)],"%H:%M %m/%d/%y")," \nLocation",dataramLocation[i])

#changed oma and mar # 2 spot from 6 to 7
	dataram <- data.frame(time,minimum,average,maximum)
	
	plotOutput <- paste(mainFileLocation,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- ggplot(dataram,aes(x=time,y=minimum))+geom_point(size=3.5) + 
labs(title = "Minimum Concentration", x = "Time", y = expression(bold(ug/m^3))) + 
scale_x_datetime(date_breaks=dataramHourBreaks[i],labels=date_format("%H:%M",tz=dataramTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	p2 <- ggplot(dataram,aes(x=time,y=maximum))+geom_point(size=3.5) + 
labs(title = "Maximum Concentration", x = "Time", y = expression(bold(ug/m^3))) + 
scale_x_datetime(date_breaks=dataramHourBreaks[i],labels=date_format("%H:%M",tz=dataramTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	p3 <- ggplot(dataram,aes(x=time,y=average))+geom_point(size=3.5) + 
labs(title = "Average Concentration", x = "Time", y = expression(bold(ug/m^3))) + 
scale_x_datetime(date_breaks=dataramHourBreaks[i],labels=date_format("%H:%M",tz=dataramTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=5)
	dev.off()
}


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
	
	filename <- substr(anemometerFilenames[i],1,nchar(anemometerFilenames[i])-4)
	theTitle <- paste(anemometerFilenames[i]," \nFrom",format(fun$Date.Time..GMT.00.00[1],"%H:%M %m/%d/%y",tz=anemometerTimeZone),"to",format(fun$Date.Time..GMT.00.00[length(fun$Date.Time..GMT.00.00)],"%H:%M %m/%d/%y",tz=anemometerTimeZone)," \nLocation",anemometerLocation[i])

	plotOutput <- paste(extendedFilePath,"\\",filename,".png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	p1 <- ggplot(fun,aes(x=Date.Time..GMT.00.00,y=Wind.Speed..mph))+geom_point(size=3.5) + 
labs(title = "Wind Speed", x = "Time", y = "mph") + 
scale_x_datetime(date_breaks=anemometerHourBreaks[i],labels=date_format("%H:%M",tz=anemometerTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	p2 <- ggplot(fun,aes(x=Date.Time..GMT.00.00,y=Wind.Direction..ø))+geom_point(size=3.5) + 
labs(title = "Wind Direction", x = "Time", y = "Degrees") + 
scale_x_datetime(date_breaks=anemometerHourBreaks[i],labels=date_format("%H:%M",tz=anemometerTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	p3 <- ggplot(fun,aes(x=Date.Time..GMT.00.00,y=Gust.Speed..mph))+geom_point(size=3.5) + 
labs(title = "Gust Speed", x = "Time", y = "mph") + 
scale_x_datetime(date_breaks=anemometerHourBreaks[i],labels=date_format("%H:%M",tz=anemometerTimeZone)) +
theme(axis.text=element_text(size = 40),title=element_text(face="bold",size=64),axis.ticks.length=unit(12,"points"),
axis.title=element_text(size=56),axis.text.x = element_text(angle=90,vjust=0.5))
	multiplot(p1, p2, p3,gHeight=graphHeight,gWidth=graphWidth, cols=2,title=theTitle,space=5)
	dev.off()
}



#the getmap function works by specifying a location in latitude and longitude that is the center of where the map is drawn
#the location needs accompanied by a zoom argument, an integer of 3 to 20, giving the spatial extent of the map
#can do a bounding box in terms of left/bottom/right/top instead of the above two parts
#okay you specify the source, then look up the maptype as given by that source
#SO getmap obtains the map, use ggmap to actually plot the map
#see this website to be able to read and use all the functionality better: https://journal.r-project.org/archive/2013-1/kahle-wickham.pdf