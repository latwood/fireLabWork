#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project\\IterationAnalyzer2.R')

#the 'rm(list = ls())' command deletes everything in the workplace
#plot(convergedAngle,col=rainbow(6),breaks=c(0,60,120,180,240,300,360)) interesting color schemes

#record start time
programstarttime <- proc.time()[3]

#import libraries
library(sp)
library(raster)
library(gtools)
#library(RColorBrewer)
library(circular, quietly=TRUE,verbose=FALSE)

#variables you can change
mainFileLocation <- "C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project"
elevationName <- "BigButteSmall"
meshResolution <- "Coarse Laptop"
convergedName <- "2000"
graphHeight <- 2000
graphWidth <- 2000
angleBreaks <- c(seq(0,360,40))
angleColors <- circular.colors(12,m=0,M=2*pi)
velocityColors <- colorRampPalette(c("blue","green","yellow","orange","red"))(5)

#variables might need to change, but for now should stay the same
extendedPath <- paste(mainFileLocation,"\\", elevationName,"\\",meshResolution,"\\10mph220deg",sep="")
convergedVelocityName <- paste(extendedPath,"\\",convergedName," iterations\\big_butte_small_220_10_31m_vel.asc", sep="")
convergedAngleName <- paste(extendedPath,"\\",convergedName," iterations\\big_butte_small_220_10_31m_ang.asc", sep="")



#include functions in this section

#this is the function for the data parser. It only has two arguments
data_parser <- function(rName,extendedPath,overallDataFrameTextOutput,iteration_number) {

	names <- c("k","epsilon","pressure","Uz","Uy","Ux")  #with the current setup, I can't change the order of these. This is because I need to be able to go back a line each time in the data read
	commandTextFileName <- paste(extendedPath,"\\",rName,"\\command_output.txt", sep="")
	all_data <- readLines(commandTextFileName)
	all_data_length <- length(all_data)
	
	goBackOne <- FALSE
	spot <- all_data_length  #I think this is not getting thrown out of the for loop	

	for (i in 1:all_data_length)
	{
		spot[1] <- all_data_length-i+1
		current_string <- substr(all_data[spot[1]],0,29)
	
		#now start getting data if we have gone back one
		if (goBackOne == TRUE)
		{
			if (current_string == "smoothSolver:  Solving for k,")
			{
				#this is annoying, I cannot affect the loop variable inside if statements inside the loop.
				#I have to basically know when the variable hits a certain point and have if statements, case statements 
				#for when the counter gets to where I want to take out variables.

				string_length <- nchar(all_data[spot[1]])
				startSpot <- 50     # 31+15+4 which is to get to the right start spot to just feed in the number
				for (k in 0:(string_length[1]-startSpot))
				{
					current_string <- substr(all_data[spot[1]],startSpot+k,startSpot+k)
					if (current_string == ",")
					{
						endSpot <- k + startSpot - 1
						break
					}
				}
				initial_residual <- substr(all_data[spot[1]],startSpot,endSpot)
				startSpot <- endSpot + 20    # 3 + 13 + 4 which is to get to the right start spot, skips the names
				for (k in 0:(string_length[1]-startSpot))
				{
					current_string <- substr(all_data[spot[1]],startSpot+k,startSpot+k)
					if (current_string == ",")
					{
						endSpot <- k + startSpot - 1
						break
					}
				}
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
						if (current_string == "Initial residual")
						{
							startSpot <- k + 4 + 15
							break
						}
					}	
					#just got the start of the string where numbers are actually at
					for (k in 0:(string_length[j]-startSpot))
					{
						current_string <- substr(all_data[spot[j]],startSpot+k,startSpot+k)
						if (current_string == ",")
						{
							endSpot <- k + startSpot - 1
							break
						}	
					}	
					initial_residual[j] <- substr(all_data[spot[j]],startSpot,endSpot)	
					startSpot <- endSpot + 20    # 3 + 13 + 4 which is to get to the right start spot, skips the names
					for (k in 0:(string_length[j]-startSpot))
					{
						current_string <- substr(all_data[spot[j]],startSpot+k,startSpot+k)
						if (current_string == ",")
						{
							endSpot <- k + startSpot - 1
							break
						}
					}	
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
				}	
				break
			}
		}
		
		#this is to determine where to start, we always want to go back one extra to avoid getting cut off information, unless I can figure out if info has been cut off
		if (current_string == "smoothSolver:  Solving for k,")
		{
			if (goBackOne == FALSE)
			{
				goBackOne <- TRUE
				next
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

	#now set up the data frames
	Ux <- initial_residual[6]
	Uy <- initial_residual[5]
	Uz <- initial_residual[4]
	pressure <- initial_residual[3]
	k <- initial_residual[1]
	epsilon <- initial_residual[2]
	
	Ux[2] <- final_residual[6]
	Uy[2] <- final_residual[5]
	Uz[2] <- final_residual[4]
	pressure[2] <- final_residual[3]
	k[2] <- final_residual[1]
	epsilon[2] <- final_residual[2]
	
	Ux[3] <- iteration_count[6]
	Uy[3] <- iteration_count[5]
	Uz[3] <- iteration_count[4]
	pressure[3] <- iteration_count[3]
	k[3] <- iteration_count[1]
	epsilon[3] <- iteration_count[2]
	
	variable_name <- "initial_residual"
	variable_name[2] <- "final_residual"
	variable_name[3] <- "iteration_count"
	
	iteration_data <- data.frame(variable_name,Ux,Uy,Uz,pressure,k,epsilon)

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
	cat(paste("\n",iteration_number,") This is the iteration data for the ",rName," file:\n",sep=""))
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



#now run the program, yay!

#set up the converged rasters from the files
convergedVelocity <- raster(convergedVelocityName)
names(convergedVelocity) <- paste("converged",convergedName,"iterations")
rVelocityStack <- stack(convergedVelocity)
convergedAngle <- raster(convergedAngleName)
names(convergedAngle) <- paste("converged",convergedName,"iterations")

#now set up a variable that holds the folder names for all the stuff we want to process
f <- list.dirs(path=extendedPath,full.names=FALSE)
filecount <- length(f)
f <- f[2:filecount]   #have to get rid of the empty value that this thing always generates
filecount <- filecount - 1   #have to adjust filecount since we got rid of the extra empty thing
f <- mixedsort(f)  #sort it and replace it

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


#now set up a loop that processes each of the files for each of these folders
for (i in 1:filecount)
{

#some of these things I could probably put into a function since I'm repeating the same thing a lot. Especially the plots
	rName <- f[i]
	print(i)
	rVelocityName <- paste(extendedPath,"\\",rName,"\\big_butte_small_220_10_31m_vel.asc", sep="")
	rAngleName <- paste(extendedPath,"\\",rName,"\\big_butte_small_220_10_31m_ang.asc", sep="")
	rVelocity <- raster(rVelocityName)
	names(rVelocity) <- rName
	rAngle <- raster(rAngleName)
	names(rAngle) <- rName

	#set up the percent change raster by using the percent change formula
	velocityDifference <- (convergedVelocity-rVelocity)
	names(velocityDifference) <- rName
	angleDifference <- (convergedAngle-rAngle)
	names(angleDifference) <- rName

	#now set up the output plot location and plot the velocity
	plotOutput <- paste(extendedPath,"\\",rName,"\\velocity.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(rVelocity,col=velocityColors)
	dev.off()

	#now set up the output plot location and plot the angle
	plotOutput <- paste(extendedPath,"\\",rName,"\\angle.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(rAngle,col=angleColors,breaks=angleBreaks)
	dev.off()

	#now set up the output plot location and plot the velocity change
	plotOutput <- paste(extendedPath,"\\",rName,"\\velocityDiff.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(velocityDifference,col=velocityColors)
	dev.off()
			
	#now set up the output plot location and plot the angle change
	plotOutput <- paste(extendedPath,"\\",rName,"\\angleDiff.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(angleDifference,col=velocityColors)
	dev.off()
			
	#Now output a textfile with all the class information and the summary
	textOutput <- paste(extendedPath,"\\",rName,"\\raster_summary.txt", sep="")
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
	} else if ( i < 17 )  #there is a maximum of 16 raster layers per group plot, but I think 9 per plot is better
	{
		rVelocityStack1 <- stack(rVelocityStack1,rVelocity)
		rAngleStack1 <- stack(rAngleStack1,rAngle)
		velocityDifferenceStack1 <- stack(velocityDifferenceStack1,velocityDifference)
		angleDifferenceStack1 <- stack(angleDifferenceStack1,angleDifference)
	}

	if( i == 17 )
	{
		rVelocityStack2 <- stack(rVelocity)
		rAngleStack2 <- stack(rAngle)
		velocityDifferenceStack2 <- stack(velocityDifference)
		angleDifferenceStack2 <- stack(angleDifference)
	} else if ( i < 33 && i > 17 )
	{
		rVelocityStack2 <- stack(rVelocityStack2,rVelocity)
		rAngleStack2 <- stack(rAngleStack2,rAngle)
		velocityDifferenceStack2 <- stack(velocityDifferenceStack2,velocityDifference)
		angleDifferenceStack2 <- stack(angleDifferenceStack2,angleDifference)
	}

	if( i == 33 )
	{
		rVelocityStack3 <- stack(rVelocity)
		rAngleStack3 <- stack(rAngle)
		velocityDifferenceStack3 <- stack(velocityDifference)
		angleDifferenceStack3 <- stack(angleDifference)
	} else if ( i < 49 && i > 33 )
	{
		rVelocityStack3 <- stack(rVelocityStack3,rVelocity)
		rAngleStack3 <- stack(rAngleStack3,rAngle)
		velocityDifferenceStack3 <- stack(velocityDifferenceStack3,velocityDifference)
		angleDifferenceStack3 <- stack(angleDifferenceStack3,angleDifference)

	}

	#run the command line text parser
	data_parser_output <- data_parser(rName,extendedPath,overallDataFrameTextOutput,i)
	
	#now put data parser output values into a format to work with
	residuals <- data.frame(data_parser_output[1])
	continuity_errors <- data.frame(data_parser_output[2])
	
	#now place the stuff into the matrices, probably need to initialize these matrices before the loop
	iteration_names[i] <- as.numeric(substr(rName,0,nchar(rName)-11))
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
	k_initial_residual[i] <- as.numeric(paste(residuals$k[1]))
	k_final_residual[i] <- as.numeric(paste(residuals$k[2]))
	k_iteration_count[i] <- as.numeric(paste(residuals$k[3]))
	epsilon_initial_residual[i] <- as.numeric(paste(residuals$epsilon[1]))
	epsilon_final_residual[i] <- as.numeric(paste(residuals$epsilon[2]))
	epsilon_iteration_count[i] <- as.numeric(paste(residuals$epsilon[3]))
	continuity_errors_local_sum[i] <- as.numeric(paste(continuity_errors$continuity_errors[1]))
	continuity_errors_global_sum[i] <- as.numeric(paste(continuity_errors$continuity_errors[2]))
	continuity_errors_cumulative_sum[i] <- as.numeric(paste(continuity_errors$continuity_errors[3]))
	timeStep[i] <- data_parser_output[[3]]
	total_simulation_time[i] <- data_parser_output[[4]]
	

	#the program has completed, let the user know
	cat(paste("The program has finished processing your",rName,"file","\n"))
	getTime(programstarttime)
}


#set up the overal velocity and angle difference stuff for plotting iteration vs error
if (filecount > 16)
{
	#make an overall Velocity and Angle Differences stack for the iteration vs error
	velocityOverallStack <- stack(rVelocityStack1,rVelocityStack2)
	angleOverallStack <- stack(rAngleStack1,rAngleStack2)
	velocityDifferenceOverallStack <- stack(velocityDifferenceStack1,velocityDifferenceStack2)
	angleDifferenceOverallStack <- stack(angleDifferenceStack1,angleDifferenceStack2)
}
if (filecount > 32)
{
	#make an overall Velocity and Angle Differences stack for the iteration vs error
	velocityOverallStack <- stack(velocityOverallStack,rVelocityStack3)
	angleOverallStack <- stack(angleOverallStack,rAngleStack3)
	velocityDifferenceOverallStack <- stack(velocityDifferenceOverallStack,velocityDifferenceStack3)
	angleDifferenceOverallStack <- stack(angleDifferenceOverallStack,angleDifferenceStack3)
}

#now set up the iteration vs error stuff for plots
velocityStandardDeviation <- cellStats(velocityOverallStack,'sd')
angleStandardDeviation <- cellStats(angleOverallStack,'sd')
velocityDifferenceSum <- cellStats(velocityDifferenceOverallStack,'sum')
velocityDifferenceRMS <- cellStats(velocityDifferenceOverallStack,'rms')
angleDifferenceSum <- cellStats(angleDifferenceOverallStack,'sum')
angleDifferenceRMS <- cellStats(angleDifferenceOverallStack,'rms')

#this adds the converged velocity/angle to the end of a lot of the rasters
#this is placed after the rms and deviation stuff since we don't want the converged velocity put in twice
if ( filecount >= 49 )
{
	cat("Not enough stacks set up for the number of files
some of the files were not made into stacks
stacks not created will not be plotted")
} else if (filecount < 49 && i >= 33)
{
	rVelocityStack3 <- stack(rVelocityStack3,convergedVelocity)
	rAngleStack3 <- stack(rAngleStack3,convergedAngle)
	velocityDifferenceStack3 <- stack(velocityDifferenceStack3,convergedVelocity)
	angleDifferenceStack3 <- stack(angleDifferenceStack3,convergedAngle)
} else if (filecount < 33 && filecount >= 17)
{
	rVelocityStack2 <- stack(rVelocityStack2,convergedVelocity)
	rAngleStack2 <- stack(rAngleStack2,convergedAngle)
	velocityDifferenceStack2 <- stack(velocityDifferenceStack2,convergedVelocity)
	angleDifferenceStack2 <- stack(angleDifferenceStack2,convergedAngle)
} else
{
	rVelocityStack1 <- stack(rVelocityStack1,convergedVelocity)
	rAngleStack1 <- stack(rAngleStack1,convergedAngle)
	velocityDifferenceStack1 <- stack(velocityDifferenceStack1,convergedVelocity)
	angleDifferenceStack1 <- stack(angleDifferenceStack1,convergedAngle)
}

cat("Converged velocities and angles added to end of rasters\n")

cat("Plotting the summary information\n")

#plot the summary information
plotOutput <- paste(extendedPath,"\\Ux_Uy_Uz_summaries.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(3,3))
plot(iteration_names,Ux_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Ux Initial Residuals")
plot(iteration_names,Ux_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Ux Final Residuals")
plot(iteration_names,Ux_iteration_count,type="o",xlab="number of iterations",ylab="iteration count",main="Ux Iteration Count")

plot(iteration_names,Uy_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uy Initial Residuals")
plot(iteration_names,Uy_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uy Final Residuals")
plot(iteration_names,Uy_iteration_count,type="o",xlab="number of iterations",ylab="iteration count",main="Uy Iteration Count")

plot(iteration_names,Uz_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uz Initial Residuals")
plot(iteration_names,Uz_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uz Final Residuals")
plot(iteration_names,Uz_iteration_count,type="o",xlab="number of iterations",ylab="iteration count",main="Uz Iteration Count")
dev.off()


plotOutput <- paste(extendedPath,"\\pressure_k_epsilon_summaries.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(3,3))
plot(iteration_names,pressure_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Pressure Initial Residuals")
plot(iteration_names,pressure_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Pressure Final Residuals")
plot(iteration_names,pressure_iteration_count,type="o",xlab="number of iterations",ylab="iteration count",main="Pressure Iteration Count")

plot(iteration_names,k_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="k Initial Residuals")
plot(iteration_names,k_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="k Final Residuals")
plot(iteration_names,k_iteration_count,type="o",xlab="number of iterations",ylab="iteration count",main="k Iteration Count")

plot(iteration_names,epsilon_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Epsilon Initial Residuals")
plot(iteration_names,epsilon_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Epsilon Final Residuals")
plot(iteration_names,epsilon_iteration_count,type="o",xlab="number of iterations",ylab="iteration count",main="Epsilon Iteration Count")
dev.off()


plotOutput <- paste(extendedPath,"\\continuityErrors_timeStep_totalSimulationTime_summaries.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(3,3))
plot(iteration_names,continuity_errors_local_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Local Sum")
plot(iteration_names,continuity_errors_global_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Global Sum")
plot(iteration_names,continuity_errors_cumulative_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Cumulative Sum")

plot(iteration_names,timeStep,type="o",xlab="number of iterations",ylab="time step count",main="Time Step")
plot(iteration_names,total_simulation_time,type="o",xlab="number of iterations",ylab="total simulation time (seconds)",main="Total Simulation Time vs Number of Iterations")
dev.off()


cat("Summary information finished plotting\n")
getTime(programstarttime)

cat("Plotting the iteration vs error\n")

plotOutput <- paste(extendedPath,"\\Iteration_vs_Error.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(3,3))
plot(iteration_names,velocityStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Velocity Standard Deviation vs Iteration Count")
plot(iteration_names,angleStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Angle Standard Deviation vs Iteration Count")
plot(iteration_names,velocityDifferenceSum,type="o",xlab="number of iterations",ylab="sum of velocity differences",main="Overall Sum of Velocity Differences vs Iteration Count")

plot(iteration_names,velocityDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of velocity differences",main="Overall RMS of Velocity Differences vs Iteration Count")
plot(iteration_names,angleDifferenceSum,type="o",xlab="number of iterations",ylab="sum of angle differences",main="Overall Sum of Angle Differences vs Iteration Count")
plot(iteration_names,angleDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of angle differences",main="Overall RMS of Angle Differences vs Iteration Count")
dev.off()

cat("The iteration vs error plots have finished\n")
getTime(programstarttime)

cat("Now plotting the groups of velocities and angles\n")

#now set up the output plot location and plot the groups of velocities
plotOutput <- paste(extendedPath,"\\velocities1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
plot(rVelocityStack1,col=velocityColors)
dev.off()
if (filecount > 16 )
{
	plotOutput <- paste(extendedPath,"\\velocities2.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(rVelocityStack2,col=velocityColors)
	dev.off()
}
if (filecount > 32 )
{
	plotOutput <- paste(extendedPath,"\\velocities3.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(rVelocityStack3,col=velocityColors)
	dev.off()
}

#now set up the output plot location and plot the groups of angles
plotOutput <- paste(extendedPath,"\\angles1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
plot(rAngleStack1,col=angleColors,breaks=angleBreaks)
dev.off()
if (filecount > 16 )
{
	plotOutput <- paste(extendedPath,"\\angles2.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(rAngleStack2,col=angleColors,breaks=angleBreaks)
	dev.off()
}
if (filecount > 32 )
{
	plotOutput <- paste(extendedPath,"\\angles3.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(rAngleStack2,col=angleColors,breaks=angleBreaks)
	dev.off()
}

cat("groups of velocities and angles finished plotting\n")
getTime(programstarttime)

cat("Now plotting the velocity and angle differences\n")

#now set up the output plot location and plot the groups of velocity differences
plotOutput <- paste(extendedPath,"\\velocityDifferences1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
plot(velocityDifferenceStack1,col=velocityColors)
dev.off()
if (filecount > 16)
{
	plotOutput <- paste(extendedPath,"\\velocityDifferences2.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(velocityDifferenceStack2,col=velocityColors)
	dev.off()
}
if (filecount > 32)
{
	plotOutput <- paste(extendedPath,"\\velocityDifferences3.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(velocityDifferenceStack3,col=velocityColors)
	dev.off()
}

#now set up the output plot location and plot the groups of angle differences
plotOutput <- paste(extendedPath,"\\angleDifferences1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
plot(angleDifferenceStack1,col=velocityColors)
dev.off()
if (filecount > 16)
{
	plotOutput <- paste(extendedPath,"\\angleDifferences2.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(angleDifferenceStack2,col=velocityColors)
	dev.off()
}
if (filecount > 32)
{
	plotOutput <- paste(extendedPath,"\\angleDifferences3.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	plot(angleDifferenceStack3,col=velocityColors)
	dev.off()
}

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




#the rest of this is just comments to myself trying to think of ideas of what to do to the data as I read stuff,
#trying to decide if an idea was worth it. I need to go through these and throw some out still.


#the raster tutorial shows putting them as separate plots, but next to each other, if you plot a brick
#this somehow makes all the plots get put into one: plotRGB(b, r=1, g=2, b=3), but it looks pretty funky
#hist, persp, contour, and density seem like good commands for this



#could have statistical functions for each point in the converged file, assuming the converged is
#the actual population or is the actual mean, and every other iteration count is a deviation
#another possibly useful idea would be to calculate whether anything is independent or not (frequency vs probability)
#how do I calculate frequency? Just do a tally for each new value.
#how do I calculate probability? I don't think this is a good idea, or possible to use independence
#but the frequency would give an idea of dependency. Probably use the current divided by the last? what is that?
#maybe do covariance and correlation for comparing angle, topography, and velocity effects
#Linear combinations of the variables seems interesting except how can you add velocity and wind together? What would that even be? Get some kind of special quantity for the spacial location? It would sound good except that the angles can go to technically negative values. I could think of something that gives an overall percent error, not just one for the angle and the velocity each separate.

#Natalie says that we are assuming we don't actually know the real velocity/angle, even with the converged file
#also there will probably be eddies where the values just keep changing each time. The idea is to find when only the eddies keep changing
#how can I make it handle that stuff? Need to see the spacial output.

#maybe do strip charts with stacking and jitter so it is possible to see patterns in the change of the output
#QQ plots let you see if it is not normally distributed (you can see what the normal distribution would look like)
#maybe do a scatter plot with each grid velocities/angles all on the same plot, kind of like the kmz files
#Pairwise relation plots look really handy. 2-d graphs done for each of the pairs or groups of datasets!
#maybe I could even just make one nice picture where it shows all the raster plots all lined up like the pairwise plots, all of them together.
#maybe use na.rm=true to or the same with zeroes to get it to do the calculations ignoring the zeroes
#or b <- a[!is.na(a)] sounds useful, extremely useful!

#make a two way table of occurrences to combine data sets