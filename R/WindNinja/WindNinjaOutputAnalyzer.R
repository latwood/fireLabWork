#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\WindNinjaOutputAnalyzer.R')

#the 'rm(list = ls())' command deletes everything in the workplace

rm(list=ls())

#IMPORTANT!!!! always call 'rm(list = ls())' before running on any files with different iteration counts and stuff. 
#All the old information is kept, so matrices still have the old values. This can throw off the plot dimensions
#Might need to delete some files in the directories before running this program!

#record start time
programstarttime <- proc.time()[3]

#import libraries   may not need all of these
library(raster)
library(rasterVis)
library(grid)

#section for main variables that change each time

#this is the path to the main plot outputs. Is the folder tree for the different folder paths holding the files to be processed
mainFileLocation <- "C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project\\Askervein\\Coarse\\10mph220deg\\"

#these are the individual folders in the folder tree holding each of the files to be processed.  Currently these names are used as plot titles as well
processingFolderNames <- "10 iterations"
processingFolderNames[2] <- "50 iterations"
processingFolderNames[3] <- "100 iterations"
processingFolderNames[4] <- "150 iterations"
processingFolderNames[5] <- "200 iterations"
processingFolderNames[6] <- "250 iterations"
processingFolderNames[7] <- "300 iterations"
processingFolderNames[8] <- "400 iterations"
processingFolderNames[9] <- "500 iterations"
processingFolderNames[10] <- "600 iterations"
processingFolderNames[11] <- "800 iterations"
processingFolderNames[12] <- "1000 iterations"
processingFolderNames[13] <- "1200 iterations"
processingFolderNames[14] <- "1400 iterations"
processingFolderNames[15] <- "1600 iterations"
processingFolderNames[16] <- "3000 iterations"

#this sets up a numeric vector for the manipulated variable. Is a numeric representation of the individual plot titles used as the x axis for the residual plots
numericXlabels <- 10
numericXlabels[2] <- 50
numericXlabels[3] <- 100
numericXlabels[4] <- 150
numericXlabels[5] <- 200
numericXlabels[6] <- 250
numericXlabels[7] <- 300
numericXlabels[8] <- 400
numericXlabels[9] <- 500
numericXlabels[10] <- 600
numericXlabels[11] <- 800
numericXlabels[12] <- 1000
numericXlabels[13] <- 1200
numericXlabels[14] <- 1400
numericXlabels[15] <- 1600
numericXlabels[16] <- 3000

#these are the specific file names to be processed in each folder. Currently this is set up to be the same filenames across each folder
velocityAscFileName <- "askervein_220_10_23m_vel.asc"
angleAscFileName <- "askervein_220_10_23m_ang.asc"

#This is the folder name with the converged data to use for all comparisons
convergedName <- processingFolderNames[16]

#these are for controlling output plot size for all regular plots
graphHeight <- 2000
graphWidth <- 2000

#these are for controlling output plot size for all raster plots. Depending on the length and width of the elevation file of interest, this greatly affects general spacing for these plots
#I made the height smaller than the width for the salmon river. Everything else has so far been the same height and width
rasterGraphHeight <- 2000
rasterGraphWidth <- 2000


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

#this is the function for the data parser.
data_parser <- function(fileName,path,overallDataFrameTextOutput,variable_number) {

	names <- c("k","epsilon","pressure","Uz","Uy","Ux")  #with the current setup, I can't change the order of these. This is because I need to be able to go back a line each time in the data read
	
	#also note that while this should work even with diurnal winds included or changing some of the setup,
	#the final solution time will be the wrong time if this is done on a diurnal simulation.
	#probably has got to be a way to simplify this, but I haven't gotten there yet
	#this was written specifically for the momentum solver with no diurnal winds, using CPL_DEBUG=NINJAFOAM and outputting
	#normal stuff with debug information to a command text file when running windninja

	commandTextFileName <- paste(path,"\\",fileName,"\\command_output.txt", sep="")
	all_data <- readLines(commandTextFileName)
	all_data_length <- length(all_data)
	
	goodToGo <- FALSE
	spot <- all_data_length  #I think this is not getting thrown out of the for loop	

	for (i in 1:all_data_length)
	{
		problem <- FALSE

		spot[1] <- all_data_length-i+1
		current_string <- substr(all_data[spot[1]],0,29)
	
		#now start getting data if we have gone back one
		if (goodToGo == TRUE)
		{
			spot[1] <- spot[1]+1
			
			for (j in 2:length(names))
			{
				if (names[j] == "pressure")
				{
					spot[j] <- spot[j-1]-2
				} else
				{
					spot[j] <- spot[j-1]-1
				}
				string_length[j] <- nchar(all_data[spot[j]])
				for (k in 0:string_length[j])
				{
					current_string <- substr(all_data[spot[j]],k,k+15)
					if (k == (string_length[j]))
					{
						problem <- TRUE
					} else if (current_string == "Initial residual")
					{
						startSpot <- k + 4 + 15
						break
					}
				}	
				#just got the start of the string where numbers are actually at
				if (problem == TRUE){ break }
				for (k in 0:(string_length[j]-startSpot))
				{
					current_string <- substr(all_data[spot[j]],startSpot+k,startSpot+k)
					if (k == (string_length[j]-startSpot))
					{
						problem <- TRUE
					} else if (current_string == ",")
					{
						endSpot <- k + startSpot - 1
						break
					}	
				}	
				if (problem == TRUE){ break }
				initial_residual[j] <- substr(all_data[spot[j]],startSpot,endSpot)	
				startSpot <- endSpot + 20    # 3 + 13 + 4 which is to get to the right start spot, skips the names
				for (k in 0:(string_length[j]-startSpot))
				{
					current_string <- substr(all_data[spot[j]],startSpot+k,startSpot+k)
					if (k == (string_length[j]-startSpot))
					{
						problem <- TRUE
					} else if (current_string == ",")
					{
						endSpot <- k + startSpot - 1
						break
					}
				}	
				if (problem == TRUE){ break }
				final_residual[j] <- substr(all_data[spot[j]],startSpot,endSpot)
				startSpot <- endSpot + 17     # 3 + 12 + 2 this gets it past the name to where the text begins
				for (k in 0:(string_length[j]-startSpot))
				{
					current_string <- substr(all_data[spot[j]],startSpot+k,startSpot+k)
					if (k == (string_length[j]-startSpot))
					{
						endSpot <- k + startSpot
						break
					}
				}		
				iteration_count[j] <- substr(all_data[spot[j]],startSpot,endSpot)
			
				k_values <- initial_residual[j]
				k_values[2] <- final_residual[j]
				k_values[3] <- iteration_count[j]
				k_debug_frame <- suppressWarnings(as.numeric(k_values))

				if (k_values[1] == "" || k_values[2] == "" || k_values[3] == "" || is.na(k_debug_frame[1]) || is.na(k_debug_frame[2]) || is.na(k_debug_frame[3]) || is.null(k_debug_frame[1]) || is.null(k_debug_frame[2]) || is.null(k_debug_frame[3]))
				{
					problem <- TRUE
				}
			}

			if (problem == TRUE)
			{ 
				goodToGo <- FALSE
				next 
			}		
			break
		}
		
		#this is to determine where to start, we always want to go back one extra to avoid getting cut off information, unless I can figure out if info has been cut off
		if (current_string == "smoothSolver:  Solving for k,")
		{
			string_length <- nchar(all_data[spot[1]])
			startSpot <- 50     # 31+15+4 which is to get to the right start spot to just feed in the number
			for (k in 0:(string_length[1]-startSpot))
			{
				current_string <- substr(all_data[spot[1]],startSpot+k,startSpot+k)
				if (k == (string_length[1]-startSpot))
				{
					problem <- TRUE
				} else if (current_string == ",")
				{
					endSpot <- k + startSpot - 1
					break
				}
			}
			if (problem == TRUE){ next }
			initial_residual <- substr(all_data[spot[1]],startSpot,endSpot)
			startSpot <- endSpot + 20    # 3 + 13 + 4 which is to get to the right start spot, skips the names
			for (k in 0:(string_length[1]-startSpot))
			{
				current_string <- substr(all_data[spot[1]],startSpot+k,startSpot+k)
				if (k == (string_length[1]-startSpot))
				{
					problem <- TRUE
				} else if (current_string == ",")
				{
					endSpot <- k + startSpot - 1
					break
				}
			}
			if (problem == TRUE){ next }
			final_residual <- substr(all_data[spot[1]],startSpot,endSpot)
			startSpot <- endSpot + 17     # 3 + 12 + 2 this gets it past the name to where the text begins
			for (k in 0:(string_length[1]-startSpot))
			{
				current_string <- substr(all_data[spot[1]],startSpot+k,startSpot+k)
				if (k == (string_length[1]-startSpot))
				{
					endSpot <- k + startSpot
					break
				}
			}
			iteration_count <- substr(all_data[spot[1]],startSpot,endSpot)
			
			k_values <- initial_residual[1]
			k_values[2] <- final_residual[1]
			k_values[3] <- iteration_count[1]
			k_debug_frame <- suppressWarnings(as.numeric(k_values))

			if (k_values[1] == "" || k_values[2] == "" || k_values[3] == "" || is.na(k_debug_frame[1]) || is.na(k_debug_frame[2]) || is.na(k_debug_frame[3]) || is.null(k_debug_frame[1]) || is.null(k_debug_frame[2]) || is.null(k_debug_frame[3]))
			{
				goodToGo <- FALSE
			} else
			{
				goodToGo <- TRUE
			}
		}
	}
	
	#now find the continuity errors, these will be placed in a separate data frame
	current_string_length <- nchar(all_data[spot[1]-2])
	for (k in 0:current_string_length)
	{
		current_string <- substr(all_data[spot[1]-2],k,k+8)
		if (current_string == "sum local")
		{
			startSpot <- k + 4 + 8
			break
		}
	}
	#just got the start of the string where numbers are actually at
	for (k in 0:(current_string_length-startSpot))
	{
		current_string <- substr(all_data[spot[1]-2],startSpot+k,startSpot+k)
		if (current_string == ",")
		{
			endSpot <- k + startSpot - 1
			break
		}
	}
	local_sum <- substr(all_data[spot[1]-2],startSpot,endSpot)	
	startSpot <- endSpot + 12
	for (k in 0:(current_string_length-startSpot))
	{
		current_string <- substr(all_data[spot[1]-2],startSpot+k,startSpot+k)
		if (current_string == ",")
		{
			endSpot <- k + startSpot - 1
			break
		}
	}
	global_sum <- substr(all_data[spot[1]-2],startSpot,endSpot)
	startSpot <- endSpot + 16
	for (k in 0:(current_string_length-startSpot))
	{
		current_string <- substr(all_data[spot[1]-2],startSpot+k,startSpot+k)
		if (k == (current_string_length-startSpot))
		{
			endSpot <- k + startSpot
			break
		}
	}
	cumulative_sum <- substr(all_data[spot[1]-2],startSpot,endSpot)

	#now find the timestep for these values
	timeisfound <- FALSE
	for (i in 1:(all_data_length-spot[1]-5))
	{
		current_string <- all_data[spot[1]-5-i]
		#cat(paste(current_string,"\n"))
		#browser()
		if (current_string != "")
		{
			current_string_length <- nchar(current_string)
			for (j in 1:current_string_length)
			{
				current_string <- substr(all_data[spot[1]-5-i],j,j+3)
				if (current_string == "Time")
				{
					time_step <- substr(all_data[spot[1]-5-i],j+3+3,current_string_length)
					timeisfound <- TRUE
					break
				}
			}
		}
		if (timeisfound == TRUE)
		{
			break
		}
	}
	
	#now find the total simulation time for the run
	current_string_length <- nchar(all_data[all_data_length-1])
	for (i in 0:current_string_length)
	{
		current_string <- substr(all_data[all_data_length-1],i,i+2)
		if (current_string == "was")
		{
			for (k in 0:current_string_length)
			{
				current_string <- substr(all_data[all_data_length-1],k+i+4,k+i+4)
				if (current_string == " ")
				{
					total_simulation_time <- substr(all_data[all_data_length-1],i+4,i+k+3)
					break
				}
			}
			break
		}
	}	

	#now set up the data frames, note that k is already done in the above part since it is used for debugging
	Ux <- initial_residual[6]
	Uy <- initial_residual[5]
	Uz <- initial_residual[4]
	pressure <- initial_residual[3]
	epsilon <- initial_residual[2]
	
	Ux[2] <- final_residual[6]
	Uy[2] <- final_residual[5]
	Uz[2] <- final_residual[4]
	pressure[2] <- final_residual[3]
	epsilon[2] <- final_residual[2]
	
	Ux[3] <- iteration_count[6]
	Uy[3] <- iteration_count[5]
	Uz[3] <- iteration_count[4]
	pressure[3] <- iteration_count[3]
	epsilon[3] <- iteration_count[2]
	
	variable_name <- "initial_residual"
	variable_name[2] <- "final_residual"
	variable_name[3] <- "iteration_count"
	
	iteration_data <- data.frame(variable_name,Ux,Uy,Uz,pressure,k_values,epsilon)

	continuity_errors <- local_sum
	continuity_errors[2] <- global_sum
	continuity_errors[3] <- cumulative_sum
	variable_name <- "local_sum"
	variable_name[2] <- "global_sum"
	variable_name[3] <- "cumulative_sum"

	continuity_error_data <- data.frame(variable_name,continuity_errors)
	
	#now convert time stuff into numerics
	time_step <- as.numeric(time_step)
	total_simulation_time <- as.numeric(total_simulation_time)
	

	#now output the information
	sink(overallDataFrameTextOutput,append=TRUE)
	cat(paste("\n",variable_number,") This is the command line data for the ",fileName," file:\n",sep=""))
	print(iteration_data)
	cat("\nThis is the continuity error data:\n")
	print(continuity_error_data)
	cat("\nThis is the time step for this output:\n")
	cat(paste(time_step,"\n"))
	cat("\nThis is the total simulation time for this run:\n")
	if (total_simulation_time < 60)
	{
		cat(paste(total_simulation_time,"seconds\n"))
	} else if (total_simulation_time < 3600)
	{
		cat(paste(total_simulation_time, "seconds =",total_simulation_time/60,"minutes\n"))
	} else
	{
		cat(paste(total_simulation_time, "seconds =",total_simulation_time/3600,"hours\n"))
	}
	sink()

	data_parser_output <- list(iteration_data,continuity_error_data,time_step,total_simulation_time)
	return(data_parser_output)
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

#set up and clear out the old summary output
overallDataFrameTextOutput <- paste(mainFileLocation,"\\overall_data_frame.txt", sep="")
sink(overallDataFrameTextOutput)
cat("This file contains a summary of interesting information from the runs\n")
sink()

#set up matrix variables with zero (causes trouble trying to fill these values otherwise)
Ux_initial_residual <- 0
Ux_final_residual <- 0
Ux_iteration_count <- 0
Uy_initial_residual <- 0
Uy_final_residual <- 0
Uy_iteration_count <- 0
Uz_initial_residual <- 0
Uz_final_residual <- 0
Uz_iteration_count <- 0
pressure_initial_residual <- 0
pressure_final_residual <- 0
pressure_iteration_count <- 0
k_initial_residual <- 0
k_final_residual <- 0
k_iteration_count <- 0
epsilon_initial_residual <- 0
epsilon_final_residual <- 0
epsilon_iteration_count <- 0
continuity_errors_local_sum <- 0
continuity_errors_global_sum <- 0
continuity_errors_cumulative_sum <- 0
timeStep <- 0
total_simulation_time <- 0

#now set up a loop that processes each of the files for each of these folders
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

	#set up the difference between the current velocity and the converged velocity as well as the difference between the current angle and the final angle
	velocityDifference <- convergedVelocity-rVelocity
	names(velocityDifference) <- processingFolderNames[i]
	angleDifference <- convergedAngle-rAngle	#this gets replaced by the function, so this is just to initialize a value that forces the type to be right for the function
	angleDifference <- angleFinder(rAngle,convergedAngle,angleDifference)
	names(angleDifference) <- processingFolderNames[i]

#these next few plots are not necessary. They are just outputting the individual plots that later are all put together.
	#now set up the output plot location and plot the velocity
	plotOutput <- paste(mainFileLocation,processingFolderNames[i],"\\velocity.png", sep="")
	png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
	print(levelplot(rVelocity,layout=c(1,1),main=list(label=paste(processingFolderNames[i],"Velocities"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=processingFolderNames[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityColors,at=velocityBreaks,
		colorkey=list(at=velocityBreaks,space="right",labels=list(labels=velocityLabels,at=velocityBreaks,cex=4,fontface='bold'),width=4,height=0.95)))
	dev.off()

	#now set up the output plot location and plot the angle
	plotOutput <- paste(mainFileLocation,processingFolderNames[i],"\\angle.png", sep="")
	png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
	print(levelplot(rAngle,layout=c(1,1),main=list(label=paste(processingFolderNames[i],"Angles"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=processingFolderNames[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleColors,at=angleBreaks,
		colorkey=list(at=angleBreaks,labels=list(labels=angleLabels,at=angleBreaks,cex=4,fontface='bold'),width=4,height=0.95,space='right')))
	dev.off()

	#now set up the output plot location and plot the velocity change
	plotOutput <- paste(mainFileLocation,processingFolderNames[i],"\\velocityDiff.png", sep="")
	png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
	suppressWarnings(print(levelplot(velocityDifference,layout=c(1,1),main=list(label=paste(processingFolderNames[i],"Velocity Differences"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=processingFolderNames[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityDifferenceColors,at=velocityDifferenceBreaks,
		colorkey=list(at=velocityDifferenceLabelBreaks,labels=list(labels=velocityDifferenceLabels,at=velocityDifferenceLabelBreaks,cex=4,fontface='bold'),width=4,height=0.95,space='right'))))
	dev.off()
			
	#now set up the output plot location and plot the angle change
	plotOutput <- paste(mainFileLocation,processingFolderNames[i],"\\angleDiff.png", sep="")
	png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
	suppressWarnings(print(levelplot(angleDifference,layout=c(1,1),main=list(label=paste(processingFolderNames[i],"Angle Differences"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=processingFolderNames[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleDifferenceColors,at=angleDifferenceBreaks,
		colorkey=list(at=angleDifferenceLabelBreaks,labels=list(labels=angleDifferenceLabels,at=angleDifferenceLabelBreaks,cex=4,fontface='bold'),width=4,height=0.95,space='right'))))
	dev.off()

#this is just a summary of the velocity, angle, velocity difference, and angle difference rasters. Might cut this out.		
	#Now output a textfile with all the class information and the summary
	textOutput <- paste(mainFileLocation,processingFolderNames[i],"\\raster_summary.txt", sep="")
	sink(textOutput)
	cat("This is the Velocity info for the file being compared\n")
	print(rVelocity)
	print(summary(rVelocity))
	cat("\n\nThis is the Velocity info for the converged file\n")
	print(convergedVelocity)
	print(summary(convergedVelocity))
	cat("\n\nThis is the Velocity info for the velocity difference\n")
	print(velocityDifference)
	print(summary(velocityDifference))
	cat("\n\nThis is the Angle info for the file being compared\n")
	print(rAngle)
	print(summary(rAngle))
	cat("\n\nThis is the Angle info for the converged file\n")
	print(convergedAngle)
	print(summary(convergedAngle))
	cat("\n\nThis is the Angle info for the angle difference\n")
	print(angleDifference)
	print(summary(angleDifference))
	sink()

	#now stack the individual rasters with past rasters	
	if( i == 1 )
	{
		rVelocityStack1 <- stack(rVelocity)
		rAngleStack1 <- stack(rAngle)
		velocityDifferenceStack1 <- stack(velocityDifference)
		angleDifferenceStack1 <- stack(angleDifference)
	} else
	{
		rVelocityStack1 <- stack(rVelocityStack1,rVelocity)
		rAngleStack1 <- stack(rAngleStack1,rAngle)
		velocityDifferenceStack1 <- stack(velocityDifferenceStack1,velocityDifference)
		angleDifferenceStack1 <- stack(angleDifferenceStack1,angleDifference)
	}

	#run the command line text parser to get residual and runtime information
	data_parser_output <- data_parser(processingFolderNames[i],mainFileLocation,overallDataFrameTextOutput,i)
	
	#since the function only outputs one variable, I made it output a list
	#now put data parser output values into a format to work with
	residuals <- data.frame(data_parser_output[1])
	continuity_errors <- data.frame(data_parser_output[2])
	
	#now place the stuff into the matrices, kind of like but not the same as a raster stack
	Ux_initial_residual[i] <- as.numeric(paste(residuals$Ux[1]))
	Ux_final_residual[i] <- as.numeric(paste(residuals$Ux[2]))
	Ux_iteration_count[i] <- as.numeric(paste(residuals$Ux[3]))
	Uy_initial_residual[i] <- as.numeric(paste(residuals$Uy[1]))
	Uy_final_residual[i] <- as.numeric(paste(residuals$Uy[2]))
	Uy_iteration_count[i] <- as.numeric(paste(residuals$Uy[3]))
	Uz_initial_residual[i] <- as.numeric(paste(residuals$Uz[1]))
	Uz_final_residual[i] <- as.numeric(paste(residuals$Uz[2]))
	Uz_iteration_count[i] <- as.numeric(paste(residuals$Uz[3]))
	pressure_initial_residual[i] <- as.numeric(paste(residuals$pressure[1]))
	pressure_final_residual[i] <- as.numeric(paste(residuals$pressure[2]))
	pressure_iteration_count[i] <- as.numeric(paste(residuals$pressure[3]))
	k_initial_residual[i] <- as.numeric(paste(residuals$k_values[1]))
	k_final_residual[i] <- as.numeric(paste(residuals$k_values[2]))
	k_iteration_count[i] <- as.numeric(paste(residuals$k_values[3]))
	epsilon_initial_residual[i] <- as.numeric(paste(residuals$epsilon[1]))
	epsilon_final_residual[i] <- as.numeric(paste(residuals$epsilon[2]))
	epsilon_iteration_count[i] <- as.numeric(paste(residuals$epsilon[3]))
	continuity_errors_local_sum[i] <- as.numeric(paste(continuity_errors$continuity_errors[1]))
	continuity_errors_global_sum[i] <- as.numeric(paste(continuity_errors$continuity_errors[2]))
	continuity_errors_cumulative_sum[i] <- as.numeric(paste(continuity_errors$continuity_errors[3]))
	timeStep[i] <- data_parser_output[[3]]
	total_simulation_time[i] <- data_parser_output[[4]]
	
	#this part of the program has completed, let the user know. Helps for debugging.
	cat(paste("The program has finished processing your",processingFolderNames[i],"file\n"))
	getTime(programstarttime)
}

cat("Setting up last few rasters\n")

#set up the overal velocity and angle difference stuff for plotting the desired variable vs error
#make an overall Velocity and Angle Differences stack for the desired variable vs error
velocityOverallStack <- rVelocityStack1
angleOverallStack <- rAngleStack1
velocityDifferenceOverallStack <- velocityDifferenceStack1
angleDifferenceOverallStack <- angleDifferenceStack1

#now set up the desired variable vs error stuff for plots
#velocityStandardDeviation <- cellStats(velocityOverallStack,'sd')		#the standard deviation is interesting but not useful
#angleStandardDeviation <- cellStats(angleOverallStack,'sd')
velocityDifferenceSum <- cellStats(velocityDifferenceOverallStack,'sum')
velocityDifferenceRMS <- cellStats(velocityDifferenceOverallStack,'rms')
angleDifferenceSum <- cellStats(angleDifferenceOverallStack,'sum')
angleDifferenceRMS <- cellStats(angleDifferenceOverallStack,'rms')

cat("Finished setting up all rasters\n")
getTime(programstarttime)

cat("Plotting the summary information\n")

#plot the summary information, or the information obtained from the command line output. These are regular plots but could be converted to ggplots if necessary
#mfrow controlls the number of plots per output, mar controls the margins, oma controls the white space, mgp controlls axis label spacing, 
#the different cex controls sizes, lwd controls line width, tck controls tick size/location, font controls bold or not, side controls grid text location

plotOutput <- paste(mainFileLocation,"\\residual_summary1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(9,9,7,3),oma=c(6,6,6,3),mgp=c(0,3.75,0),cex.axis=5,cex.main=5,lwd=5,tck=-0.02)
plot(numericXlabels,Ux_initial_residual,type="o",xlab="",ylab="",main="Ux Initial Residuals",cex=5)
plot(numericXlabels,Ux_final_residual,type="o",xlab="",ylab="",main="Ux Final Residuals",cex=5)
plot(numericXlabels,Uy_initial_residual,type="o",xlab="",ylab="",main="Uy Initial Residuals",cex=5)
plot(numericXlabels,Uy_final_residual,type="o",xlab="",ylab="",main="Uy Final Residuals",cex=5)
mtext("Residual Information", outer = TRUE, cex = 7.5,font=2)
mtext("Residuals",outer=TRUE,cex=4.5,side=2,font=2)
mtext("Iteration Count",outer=TRUE,cex=4.5,side=1,font=2)
dev.off()

plotOutput <- paste(mainFileLocation,"\\residual_summary2.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(9,9,7,3),oma=c(6,6,6,3),mgp=c(0,3.75,0),cex.axis=5,cex.main=5,lwd=5,tck=-.02)
plot(numericXlabels,Uz_initial_residual,type="o",xlab="",ylab="",main="Uz Initial Residuals",cex=5)
plot(numericXlabels,Uz_final_residual,type="o",xlab="",ylab="",main="Uz Final Residuals",cex=5)
plot(numericXlabels,pressure_initial_residual,type="o",xlab="",ylab="",main="Pressure Initial Residuals",cex=5)
plot(numericXlabels,pressure_final_residual,type="o",xlab="",ylab="",main="Pressure Final Residuals",cex=5)
mtext("Residual Information", outer = TRUE, cex = 7.5,font=2)
mtext("Residuals",outer=TRUE,cex=4.5,side=2,font=2)
mtext("Iteration Count",outer=TRUE,cex=4.5,side=1,font=2)
dev.off()

plotOutput <- paste(mainFileLocation,"\\residual_summary3.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(9,9,7,3),oma=c(6,6,6,3),mgp=c(0,3.75,0),cex.axis=5,cex.main=5,lwd=5,tck=-0.02)
plot(numericXlabels,k_initial_residual,type="o",xlab="",ylab="",main="k Initial Residuals",cex=5)
plot(numericXlabels,k_final_residual,type="o",xlab="",ylab="",main="k Final Residuals",cex=5)
plot(numericXlabels,epsilon_initial_residual,type="o",xlab="",ylab="",main="Epsilon Initial Residuals",cex=5)
plot(numericXlabels,epsilon_final_residual,type="o",xlab="",ylab="",main="Epsilon Final Residuals",cex=5)
mtext("Residual Information", outer = TRUE, cex = 7.5,font=2)
mtext("Residuals",outer=TRUE,cex=4.5,side=2,font=2)
mtext("Iteration Count",outer=TRUE,cex=4.5,side=1,font=2)
dev.off()

#convert time to hours for the total simulation time
total_simulation_time <- total_simulation_time/3600

plotOutput <- paste(mainFileLocation,"\\continuityErrors_SimulationTime.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(12,12,6,3),oma=c(3,3,16,3),mgp=c(9,3.75,0),cex.lab=5,cex.axis=5,cex.main=5,font.lab=2,lwd=5,tck=-0.02)
plot(numericXlabels,continuity_errors_local_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Local Sum",cex=5)
plot(numericXlabels,continuity_errors_global_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Global Sum",cex=5)
plot(numericXlabels,continuity_errors_cumulative_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Cumulative Sum",cex=5)
#plot(numericXlabels,timeStep,type="o",xlab="number of iterations",ylab="time step count",main="Time Step",cex=5)
plot(numericXlabels,total_simulation_time,type="o",xlab="number of iterations",ylab="total simulation time (hrs)",main="Total Simulation Time",cex=5)
mtext("Continuity Errors and\nSimulation Time Summaries", outer = TRUE, cex = 7.5,font=2)
dev.off()


cat("Summary information finished plotting\n")
getTime(programstarttime)

cat("Plotting the desired variable vs error\n")

#plot the desired variable vs overall error. This helps to see final convergence criteria without all the individual colored plots
plotOutput <- paste(mainFileLocation,"\\Iteration_vs_Error.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(12,12,7,3),oma=c(3,3,7,3),mgp=c(9,3.75,0),cex.lab=5,cex.axis=5,cex.main=5,font.lab=2,lwd=5,tck=-0.02)
#plot(numericXlabels,velocityStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Velocity Standard Deviation",cex=3.5)
#plot(numericXlabels,angleStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Angle Standard Deviation",cex=5)
plot(numericXlabels,velocityDifferenceSum,type="o",xlab="number of iterations",ylab="sum of velocity differences",main="Overall Sum of Velocity Differences",cex=5)
plot(numericXlabels,angleDifferenceSum,type="o",xlab="number of iterations",ylab="sum of angle differences",main="Overall Sum of Angle Differences",cex=5)
plot(numericXlabels,velocityDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of velocity differences",main="Overall RMS of Velocity Differences",cex=5)
plot(numericXlabels,angleDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of angle differences",main="Overall RMS of Angle Differences",cex=5)
mtext("Iteration vs Error Plots", outer = TRUE, cex = 7.5,font=2)
dev.off()

cat("The iteration vs error plots have finished\n")
getTime(programstarttime)

cat("Now plotting the groups of velocities and angles\n")

#now plot all the raster information. layout controls the number of plots per output, cex controls different sizes, lines may control spacing, 
#colorkey width and height controls legend size.
#part of the trouble with this part is commands for manipulating plot picture are ignored when placed in the wrong places

#now set up the output plot location and plot the groups of velocities
plotOutput <- paste(mainFileLocation,"\\velocities.png", sep="")
png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
print(levelplot(rVelocityStack1,layout=c(4,4),main=list(label="Velocities", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=processingFolderNames,par.strip.text=list(cex=4,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityColors,at=velocityBreaks,
	colorkey=list(at=velocityBreaks,labels=list(labels=velocityLabels,at=velocityBreaks,cex=4,fontface='bold'),width=5,height=0.95)))
dev.off()

#now set up the output plot location and plot the groups of angles
plotOutput <- paste(mainFileLocation,"\\angles.png", sep="")
png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
print(levelplot(rAngleStack1,layout=c(4,4),main=list(label="Angles", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=processingFolderNames,par.strip.text=list(cex=4,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleColors,at=angleBreaks,
	colorkey=list(at=angleBreaks,labels=list(labels=angleLabels,at=angleBreaks,cex=4,fontface='bold'),width=5,height=0.95)))
dev.off()

cat("groups of velocities and angles finished plotting\n")
getTime(programstarttime)

cat("Now plotting the velocity and angle differences\n")

#now set up the output plot location and plot the groups of velocity differences
plotOutput <- paste(mainFileLocation,"\\velocityDifferences.png", sep="")
png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
print(levelplot(velocityDifferenceStack1,layout=c(4,4),main=list(label="Velocity Differences", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=processingFolderNames,par.strip.text=list(cex=4,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityDifferenceColors,at=velocityDifferenceBreaks,
	colorkey=list(at=velocityDifferenceLabelBreaks,labels=list(labels=velocityDifferenceLabels,at=velocityDifferenceLabelBreaks,cex=4,fontface='bold'),width=5,height=0.95)))
dev.off()

#now set up the output plot location and plot the groups of angle differences
plotOutput <- paste(mainFileLocation,"\\angleDifferences.png", sep="")
png(filename=plotOutput,width=rasterGraphWidth,height=rasterGraphHeight)
print(levelplot(angleDifferenceStack1,layout=c(4,4),main=list(label="Angle Differences", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=processingFolderNames,par.strip.text=list(cex=4,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleDifferenceColors,at=angleDifferenceBreaks,
	colorkey=list(at=angleDifferenceLabelBreaks,labels=list(labels=angleDifferenceLabels,at=angleDifferenceLabelBreaks,cex=4,fontface='bold'),width=5,height=0.95)))
dev.off()

cat("finished velocity and angle difference plots\n")
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
