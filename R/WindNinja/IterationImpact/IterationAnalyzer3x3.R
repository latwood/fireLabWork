#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project\\IterationAnalyzer3x3.R')

#the 'rm(list = ls())' command deletes everything in the workplace
#plot(convergedAngle,col=rainbow(6),breaks=c(0,60,120,180,240,300,360)) interesting color schemes

rm(list=ls())

#IMPORTANT!!!! always call 'rm(list = ls())' before running on any files with different iteration counts and stuff. 
#All the old information is kept, so matrices still have the old values. This can throw off the plot dimensions
#Might need to delete some files in the directories before running this program!

#record start time
programstarttime <- proc.time()[3]

#import libraries
library(sp)
library(raster)
library(gtools)
library(rasterVis)
library(grid)
library(circular, quietly=TRUE,verbose=FALSE)


#variables you can change
mainFileLocation <- "C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project"
elevationName <- "SalmonRiver"
meshResolution <- "Fine"
angleAscFileName <- "salmonriver_220_10_31m_ang.asc"
velocityAscFileName <- "salmonriver_220_10_31m_vel.asc"
convergedName <- "5000"
graphHeight <- 2000
graphWidth <- 2000
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

#set up the rNames matrix to be used instead of the past method of just grabbing all the files in a directory
rNames <- "10"
rNames[2] <- "50"
rNames[3] <- "100"
rNames[4] <- "200"
rNames[5] <- "300"
rNames[6] <- "400"
rNames[7] <- "500"
rNames[8] <- "600"
rNames[9] <- "3000"
rNames[10] <- "5000"
#rNames[11] <- "1000"
#rNames[12] <- "1200"
#rNames[13] <- "1400"
#rNames[14] <- "1600"
#rNames[15] <- "1800"
#rNames[16] <- "5000"

rNames <- mixedsort(rNames)  #sort it and replace it
filecount <- length(rNames)

#variables might need to change, but for now should stay the same
extendedPath <- paste(mainFileLocation,"\\", elevationName,"\\",meshResolution,"\\10mph220deg",sep="")
convergedVelocityName <- paste(extendedPath,"\\",convergedName," iterations\\",velocityAscFileName, sep="")
convergedAngleName <- paste(extendedPath,"\\",convergedName," iterations\\",angleAscFileName, sep="")



#include functions in this section


#this is the function for the data parser. It only has two arguments
data_parser <- function(fileName,extendedPath,overallDataFrameTextOutput,iteration_number) {

	names <- c("k","epsilon","pressure","Uz","Uy","Ux")  #with the current setup, I can't change the order of these. This is because I need to be able to go back a line each time in the data read
	commandTextFileName <- paste(extendedPath,"\\",fileName," iterations\\command_output.txt", sep="")
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
	cat(paste("\n",iteration_number,") This is the iteration data for the ",fileName," file:\n",sep=""))
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

angleFinderReal <- function(x,y)
{
	a <- 360-abs(x - y)
	b <- abs(x-y)
	ind = which(a > b)
	z = a
	z[ind] <- b[ind]
	return(z)
}

#now run the program, yay!

#set up the converged rasters from the files
convergedVelocity <- raster(convergedVelocityName)
names(convergedVelocity) <- paste("converged_",convergedName,"_iterations",sep="")
rVelocityStack <- stack(convergedVelocity)
convergedAngle <- raster(convergedAngleName)
names(convergedAngle) <- paste("converged_",convergedName,"_iterations",sep="")


#set up and clear out the old summary output
overallDataFrameTextOutput <- paste(extendedPath,"\\overall_data_frame.txt", sep="")
sink(overallDataFrameTextOutput)
cat("This file contains a summary of interesting information from the runs\n")
sink()


#set up matrix variables with zero
iteration_names <- 0
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
titles <- ""

#now set up a loop that processes each of the files for each of these folders
for (i in 1:filecount)
{

#some of these things I could probably put into a function since I'm repeating the same thing a lot. Especially the plots
	print(i)
	rVelocityName <- paste(extendedPath,"\\",rNames[i]," iterations\\",velocityAscFileName, sep="")
	rAngleName <- paste(extendedPath,"\\",rNames[i]," iterations\\",angleAscFileName, sep="")
	rVelocity <- raster(rVelocityName)
	names(rVelocity) <- paste(rNames[i],"_iterations",sep="")
	rAngle <- raster(rAngleName)
	names(rAngle) <- paste(rNames[i],"_iterations",sep="")
	titles[i] <- paste(rNames[i]," Iterations",sep="")

	#set up the percent change raster by using the percent change formula
	velocityDifference <- (convergedVelocity-rVelocity)
	names(velocityDifference) <- paste(rNames[i],"_iterations",sep="")
	angleDifference <- rAngle-convergedAngle
	angleDifference <- angleFinder(rAngle,convergedAngle,angleDifference)
	names(angleDifference) <- paste(rNames[i],"_iterations",sep="")

	#now set up the output plot location and plot the velocity
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\velocity.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	print(levelplot(rVelocity,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Velocities"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=titles[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityColors,at=velocityBreaks,
		colorkey=list(at=velocityBreaks,space="right",labels=list(labels=velocityLabels,at=velocityBreaks,cex=4,fontface='bold'),width=4,height=0.95)))
	dev.off()

	#now set up the output plot location and plot the angle
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\angle.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	print(levelplot(rAngle,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Angles"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=titles[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleColors,at=angleBreaks,
		colorkey=list(at=angleBreaks,labels=list(labels=angleLabels,at=angleBreaks,cex=4,fontface='bold'),width=4,height=0.95,space='right')))
	dev.off()

	#now set up the output plot location and plot the velocity change
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\velocityDiff.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	suppressWarnings(print(levelplot(velocityDifference,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Velocity Differences"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=titles[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityDifferenceColors,at=velocityDifferenceBreaks,
		colorkey=list(at=velocityDifferenceLabelBreaks,labels=list(labels=velocityDifferenceLabels,at=velocityDifferenceLabelBreaks,cex=4,fontface='bold'),width=4,height=0.95,space='right'))))
	dev.off()
			
	#now set up the output plot location and plot the angle change
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\angleDiff.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	suppressWarnings(print(levelplot(angleDifference,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Angle Differences"), cex=5.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),names.attr=titles[i],par.strip.text=list(cex=4,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleDifferenceColors,at=angleDifferenceBreaks,
		colorkey=list(at=angleDifferenceLabelBreaks,labels=list(labels=angleDifferenceLabels,at=angleDifferenceLabelBreaks,cex=4,fontface='bold'),width=4,height=0.95,space='right'))))
	dev.off()
			
	#Now output a textfile with all the class information and the summary
	textOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\raster_summary.txt", sep="")
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


	#run the command line text parser
	data_parser_output <- data_parser(rNames[i],extendedPath,overallDataFrameTextOutput,i)
	
	#now put data parser output values into a format to work with
	residuals <- data.frame(data_parser_output[1])
	continuity_errors <- data.frame(data_parser_output[2])
	
	#now place the stuff into the matrices, probably need to initialize these matrices before the loop
	iteration_names[i] <- as.numeric(rNames[i])
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
	

	#the program has completed, let the user know
	cat(paste("The program has finished processing your",rNames[i],"iterations file","\n"))
	getTime(programstarttime)
}


#maybe add an output plot for each section that goes through and gets every single Ux value for every single timestep,
# the same with all others, and plots all those together for a single iteration

cat("Setting up last few rasters\n")

#set up the overal velocity and angle difference stuff for plotting iteration vs error
#make an overall Velocity and Angle Differences stack for the iteration vs error
velocityOverallStack <- rVelocityStack1
angleOverallStack <- rAngleStack1
velocityDifferenceOverallStack <- velocityDifferenceStack1
angleDifferenceOverallStack <- angleDifferenceStack1

#now set up the iteration vs error stuff for plots
velocityStandardDeviation <- cellStats(velocityOverallStack,'sd')
angleStandardDeviation <- cellStats(angleOverallStack,'sd')
velocityDifferenceSum <- cellStats(velocityDifferenceOverallStack,'sum')
velocityDifferenceRMS <- cellStats(velocityDifferenceOverallStack,'rms')
angleDifferenceSum <- cellStats(angleDifferenceOverallStack,'sum')
angleDifferenceRMS <- cellStats(angleDifferenceOverallStack,'rms')

cat("Finished setting up all rasters\n")
getTime(programstarttime)

cat("Plotting the summary information\n")

#plot the summary information
plotOutput <- paste(extendedPath,"\\residual_summary1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(9,9,7,3),oma=c(6,6,6,3),mgp=c(0,3.75,0),cex.axis=5,cex.main=5,lwd=5,tck=-0.02)
plot(iteration_names,Ux_initial_residual,type="o",xlab="",ylab="",main="Ux Initial Residuals",cex=5)
plot(iteration_names,Ux_final_residual,type="o",xlab="",ylab="",main="Ux Final Residuals",cex=5)
plot(iteration_names,Uy_initial_residual,type="o",xlab="",ylab="",main="Uy Initial Residuals",cex=5)
plot(iteration_names,Uy_final_residual,type="o",xlab="",ylab="",main="Uy Final Residuals",cex=5)
mtext("Initial and Final Residual Information", outer = TRUE, cex = 7.5,font=2)
mtext("Residuals",outer=TRUE,cex=4.5,side=2,font=2)
mtext("Iteration Count",outer=TRUE,cex=4.5,side=1,font=2)
dev.off()

plotOutput <- paste(extendedPath,"\\residual_summary2.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(9,9,7,3),oma=c(6,6,6,3),mgp=c(0,3.75,0),cex.axis=5,cex.main=5,lwd=5,tck=-.02)
plot(iteration_names,Uz_initial_residual,type="o",xlab="",ylab="",main="Uz Initial Residuals",cex=5)
plot(iteration_names,Uz_final_residual,type="o",xlab="",ylab="",main="Uz Final Residuals",cex=5)
plot(iteration_names,pressure_initial_residual,type="o",xlab="",ylab="",main="Pressure Initial Residuals",cex=5)
plot(iteration_names,pressure_final_residual,type="o",xlab="",ylab="",main="Pressure Final Residuals",cex=5)
mtext("Initial and Final Residual Information", outer = TRUE, cex = 7.5,font=2)
mtext("Residuals",outer=TRUE,cex=4.5,side=2,font=2)
mtext("Iteration Count",outer=TRUE,cex=4.5,side=1,font=2)
dev.off()

plotOutput <- paste(extendedPath,"\\residual_summary3.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(9,9,7,3),oma=c(6,6,6,3),mgp=c(0,3.75,0),cex.axis=5,cex.main=5,lwd=5,tck=-0.02)
plot(iteration_names,k_initial_residual,type="o",xlab="",ylab="",main="k Initial Residuals",cex=5)
plot(iteration_names,k_final_residual,type="o",xlab="",ylab="",main="k Final Residuals",cex=5)
plot(iteration_names,epsilon_initial_residual,type="o",xlab="",ylab="",main="Epsilon Initial Residuals",cex=5)
plot(iteration_names,epsilon_final_residual,type="o",xlab="",ylab="",main="Epsilon Final Residuals",cex=5)
mtext("Initial and Final Residual Information", outer = TRUE, cex = 7.5,font=2)
mtext("Residuals",outer=TRUE,cex=4.5,side=2,font=2)
mtext("Iteration Count",outer=TRUE,cex=4.5,side=1,font=2)
dev.off()

#convert time to hours for the total simulation time
total_simulation_time <- total_simulation_time/3600

plotOutput <- paste(extendedPath,"\\continuityErrors_timeStep_totalSimulationTime_summaries.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(12,12,6,3),oma=c(3,3,16,3),mgp=c(9,3.75,0),cex.lab=5,cex.axis=5,cex.main=5,font.lab=2,lwd=5,tck=-0.02)
plot(iteration_names,continuity_errors_local_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Local Sum",cex=5)
plot(iteration_names,continuity_errors_global_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Global Sum",cex=5)
plot(iteration_names,continuity_errors_cumulative_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Cumulative Sum",cex=5)
#plot(iteration_names,timeStep,type="o",xlab="number of iterations",ylab="time step count",main="Time Step",cex=5)
plot(iteration_names,total_simulation_time,type="o",xlab="number of iterations",ylab="total simulation time (hrs)",main="Total Simulation Time",cex=5)
mtext("Continuity Errors and\nSimulation Time Summaries", outer = TRUE, cex = 7.5,font=2)
dev.off()


cat("Summary information finished plotting\n")
getTime(programstarttime)

cat("Plotting the iteration vs error\n")

plotOutput <- paste(extendedPath,"\\Iteration_vs_Error.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(2,2),mar=c(12,12,7,3),oma=c(3,3,7,3),mgp=c(9,3.75,0),cex.lab=5,cex.axis=5,cex.main=5,font.lab=2,lwd=5,tck=-0.02)
#plot(iteration_names,velocityStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Velocity Standard Deviation",cex=3.5)
#plot(iteration_names,angleStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Angle Standard Deviation",cex=5)
plot(iteration_names,velocityDifferenceSum,type="o",xlab="number of iterations",ylab="sum of velocity differences",main="Overall Sum of Velocity Differences",cex=5)
plot(iteration_names,angleDifferenceSum,type="o",xlab="number of iterations",ylab="sum of angle differences",main="Overall Sum of Angle Differences",cex=5)
plot(iteration_names,velocityDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of velocity differences",main="Overall RMS of Velocity Differences",cex=5)
plot(iteration_names,angleDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of angle differences",main="Overall RMS of Angle Differences",cex=5)
mtext("Iteration vs Error Plots", outer = TRUE, cex = 7.5,font=2)
dev.off()

cat("The iteration vs error plots have finished\n")
getTime(programstarttime)

cat("Now plotting the groups of velocities and angles\n")

#now set up the output plot location and plot the groups of velocities
plotOutput <- paste(extendedPath,"\\velocities1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(rVelocityStack1,layout=c(4,4),main=list(label="Velocities 1", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=titles,par.strip.text=list(cex=4,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityColors,at=velocityBreaks,
	colorkey=list(at=velocityBreaks,labels=list(labels=velocityLabels,at=velocityBreaks,cex=4,fontface='bold'),width=5,height=0.95)))
dev.off()

#now set up the output plot location and plot the groups of angles
plotOutput <- paste(extendedPath,"\\angles1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(rAngleStack1,layout=c(4,4),main=list(label="Angles 1", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=titles,par.strip.text=list(cex=4,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleColors,at=angleBreaks,
	colorkey=list(at=angleBreaks,labels=list(labels=angleLabels,at=angleBreaks,cex=4,fontface='bold'),width=5,height=0.95)))
dev.off()

cat("groups of velocities and angles finished plotting\n")
getTime(programstarttime)

cat("Now plotting the velocity and angle differences\n")

#now set up the output plot location and plot the groups of velocity differences
plotOutput <- paste(extendedPath,"\\velocityDifferences1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(velocityDifferenceStack1,layout=c(4,4),main=list(label="Velocity Differences 1", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=titles,par.strip.text=list(cex=4,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityDifferenceColors,at=velocityDifferenceBreaks,
	colorkey=list(at=velocityDifferenceLabelBreaks,labels=list(labels=velocityDifferenceLabels,at=velocityDifferenceLabelBreaks,cex=4,fontface='bold'),width=5,height=0.95)))
dev.off()

#now set up the output plot location and plot the groups of angle differences
plotOutput <- paste(extendedPath,"\\angleDifferences1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(angleDifferenceStack1,layout=c(4,4),main=list(label="Angle Differences 1", cex=5.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),names.attr=titles,par.strip.text=list(cex=4,lines=1,fontface='bold'),
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
