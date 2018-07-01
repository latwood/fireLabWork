#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project\\IterationAnalyzer4.R')

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
elevationName <- "BigButteSmall"
meshResolution <- "Fine"
convergedName <- "2000"
graphHeight <- 2000
graphWidth <- 2000
angleBreaks <- c(seq(0,360,40))
#angleColors <- circular.colors(12,m=0,M=2*pi)
angleColors <- colorRampPalette(c("red","orange","yellow","forestgreen","green","cyan","lightblue","blue","purple"))(9)
velocityBreaks <- c(0,4,8,12,16,20)
velocityColors <- colorRampPalette(c("blue","green","yellow","orange","red"))(5)
velocityDifferenceColors <- colorRampPalette(c("purple","blue","green","grey","white","brown","yellow","orange","red"))(9)
velocityDifferenceBreaks <- c(-20,-5,-3,-1.5,-0.5,0.5,1.5,3,5,20)
angleDifferenceColors <- colorRampPalette(c("purple","blue","green","grey","white","brown","yellow","orange","red"))(9)
angleDifferenceBreaks <- c(-360,-180,-60,-20,-5,5,20,60,180,360)


#set up the rNames matrix to be used instead of the past method of just grabbing all the files in a directory
rNames <- "10"
rNames[2] <- "50"
rNames[3] <- "100"
rNames[4] <- "200"
rNames[5] <- "300"
rNames[6] <- "400"
rNames[7] <- "500"
rNames[8] <- "600"
rNames[9] <- "800"
rNames[10] <- "1000"
rNames[11] <- "1200"
rNames[12] <- "1600"
rNames[13] <- "1800"
rNames[14] <- "2000"
#rNames[15] <- "1800"
#rNames[16] <- "2000"

rNames <- mixedsort(rNames)  #sort it and replace it
filecount <- length(rNames)

#variables might need to change, but for now should stay the same
extendedPath <- paste(mainFileLocation,"\\", elevationName,"\\",meshResolution,"\\10mph220deg",sep="")
convergedVelocityName <- paste(extendedPath,"\\",convergedName," iterations\\big_butte_small_220_10_31m_vel.asc", sep="")
convergedAngleName <- paste(extendedPath,"\\",convergedName," iterations\\big_butte_small_220_10_31m_ang.asc", sep="")



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
		
		#this is to determine where to start, we always want to go back one extra to avoid getting cut off information, unless I can figure out if info has been cut off
		if (current_string == "smoothSolver:  Solving for k,")
		{
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


#now set up a loop that processes each of the files for each of these folders
for (i in 1:filecount)
{

#some of these things I could probably put into a function since I'm repeating the same thing a lot. Especially the plots
	print(i)
	rVelocityName <- paste(extendedPath,"\\",rNames[i]," iterations\\big_butte_small_220_10_31m_vel.asc", sep="")
	rAngleName <- paste(extendedPath,"\\",rNames[i]," iterations\\big_butte_small_220_10_31m_ang.asc", sep="")
	rVelocity <- raster(rVelocityName)
	names(rVelocity) <- paste(rNames[i],"_iterations",sep="")
	rAngle <- raster(rAngleName)
	names(rAngle) <- paste(rNames[i],"_iterations",sep="")

	#set up the percent change raster by using the percent change formula
	velocityDifference <- (convergedVelocity-rVelocity)
	names(velocityDifference) <- paste(rNames[i],"_iterations",sep="")
	angleDifference <- (convergedAngle-rAngle)
	names(angleDifference) <- paste(rNames[i],"_iterations",sep="")

	#now set up the output plot location and plot the velocity
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\velocity.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	print(levelplot(rVelocity,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Velocities"), cex=4.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityColors,at=velocityBreaks,
		colorkey=list(at=velocityBreaks,space="right",labels=list(at=velocityBreaks,cex=3),width=4,height=0.95)))
	grid.text('velocity (m/s)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
	dev.off()

	#now set up the output plot location and plot the angle
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\angle.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	print(levelplot(rAngle,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Angles"), cex=4.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleColors,at=angleBreaks,
		colorkey=list(at=angleBreaks,labels=list(at=angleBreaks,cex=3),width=4,height=0.95,space='right')))
	grid.text('angle (degrees)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
	dev.off()

	#now set up the output plot location and plot the velocity change
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\velocityDiff.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	suppressWarnings(print(levelplot(velocityDifference,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Velocity Differences"), cex=4.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityDifferenceColors,at=velocityDifferenceBreaks,
		colorkey=list(at=velocityDifferenceBreaks,labels=list(at=velocityDifferenceBreaks,cex=3),width=4,height=0.95,space='right'))))
	grid.text('difference (m/s)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
	dev.off()
			
	#now set up the output plot location and plot the angle change
	plotOutput <- paste(extendedPath,"\\",rNames[i]," iterations\\angleDiff.png", sep="")
	png(filename=plotOutput,width=graphWidth,height=graphHeight)
	suppressWarnings(print(levelplot(velocityDifference,layout=c(1,1),main=list(label=paste(rNames[i],"iterations Velocity Differences"), cex=4.5,lines=2),
		xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
		fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
		scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityDifferenceColors,at=velocityDifferenceBreaks,
		colorkey=list(at=velocityDifferenceBreaks,labels=list(at=velocityDifferenceBreaks,cex=3),width=4,height=0.95,space='right'))))
	grid.text('difference (m/s)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
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
plotOutput <- paste(extendedPath,"\\residual_summary.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(4,4),mar=c(9,9,9,9),oma=c(0,0,9,0),mgp=c(6,2.5,0),cex.lab=4,cex.axis=4,cex.sub=3.5,cex.main=4,font.lab=2,lwd=3)

plot(iteration_names,Ux_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Ux Initial Residuals",cex=2.5)
plot(iteration_names,Ux_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Ux Final Residuals",cex=2.5)

plot(iteration_names,Uy_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uy Initial Residuals",cex=2.5)
plot(iteration_names,Uy_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uy Final Residuals",cex=2.5)

plot(iteration_names,Uz_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uz Initial Residuals",cex=2.5)
plot(iteration_names,Uz_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Uz Final Residuals",cex=2.5)

plot(iteration_names,pressure_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Pressure Initial Residuals",cex=2.5)
plot(iteration_names,pressure_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Pressure Final Residuals",cex=2.5)

plot(iteration_names,k_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="k Initial Residuals",cex=2.5)
plot(iteration_names,k_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="k Final Residuals",cex=2.5)

plot(iteration_names,epsilon_initial_residual,type="o",xlab="number of iterations",ylab="residuals",main="Epsilon Initial Residuals",cex=2.5)
plot(iteration_names,epsilon_final_residual,type="o",xlab="number of iterations",ylab="residuals",main="Epsilon Final Residuals",cex=2.5)

mtext("Initial and Final Residual Information", outer = TRUE, cex = 4.5,font=2)
dev.off()


plotOutput <- paste(extendedPath,"\\continuityErrors_timeStep_totalSimulationTime_summaries.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(3,2),mar=c(9,9,15,6),oma=c(0,0,15,0),mgp=c(6,2.5,0),cex.lab=4,cex.axis=4,cex.sub=3.5,cex.main=4,font.lab=2,lwd=3)
plot(iteration_names,continuity_errors_local_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Local Sum",cex=2.5)
plot(iteration_names,continuity_errors_global_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Global Sum",cex=2.5)
plot(iteration_names,continuity_errors_cumulative_sum,type="o",xlab="number of iterations",ylab="sum of errors",main="Continuity Errors Cumulative Sum",cex=2.5)

plot(iteration_names,timeStep,type="o",xlab="number of iterations",ylab="time step count",main="Time Step",cex=2.5)
plot(iteration_names,total_simulation_time,type="o",xlab="number of iterations",ylab="total simulation time (seconds)",main="Total Simulation Time vs Number of Iterations",cex=2.5)
mtext("Continuity Errors, Time Step for Final Iteration,\nand Total Simulation Time Summaries", outer = TRUE, cex = 4.5,font=2)
dev.off()


cat("Summary information finished plotting\n")
getTime(programstarttime)

cat("Plotting the iteration vs error\n")

plotOutput <- paste(extendedPath,"\\Iteration_vs_Error.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
par(mfrow=c(3,2),mar=c(9,9,9,9),oma=c(0,0,9,0),mgp=c(6,2.5,0),cex.lab=4,cex.axis=4,cex.sub=3.5,cex.main=4,font.lab=2,lwd=3)
plot(iteration_names,velocityStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Velocity Standard Deviation",cex=2.5)
plot(iteration_names,angleStandardDeviation,type="o",xlab="number of iterations",ylab="standard deviation",main="Overall Angle Standard Deviation",cex=2.5)

plot(iteration_names,velocityDifferenceSum,type="o",xlab="number of iterations",ylab="sum of velocity differences",main="Overall Sum of Velocity Differences",cex=2.5)
plot(iteration_names,angleDifferenceSum,type="o",xlab="number of iterations",ylab="sum of angle differences",main="Overall Sum of Angle Differences",cex=2.5)

plot(iteration_names,velocityDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of velocity differences",main="Overall RMS of Velocity Differences",cex=2.5)
plot(iteration_names,angleDifferenceRMS,type="o",xlab="number of iterations",ylab="rms of angle differences",main="Overall RMS of Angle Differences",cex=2.5)
mtext("Iteration vs Error Plots", outer = TRUE, cex = 4.5,font=2)
dev.off()

cat("The iteration vs error plots have finished\n")
getTime(programstarttime)

cat("Now plotting the groups of velocities and angles\n")

#now set up the output plot location and plot the groups of velocities
plotOutput <- paste(extendedPath,"\\velocities1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(rVelocityStack1,layout=c(4,4),main=list(label="Velocities 1", cex=4.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityColors,at=velocityBreaks,
	colorkey=list(at=velocityBreaks,labels=list(at=velocityBreaks,cex=3),width=4,height=0.95)))
grid.text('velocity (m/s)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
dev.off()

#now set up the output plot location and plot the groups of angles
plotOutput <- paste(extendedPath,"\\angles1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(rAngleStack1,layout=c(4,4),main=list(label="Angles 1", cex=4.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleColors,at=angleBreaks,
	colorkey=list(at=angleBreaks,labels=list(at=angleBreaks,cex=3),width=4,height=0.95)))
grid.text('angle (degrees)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
dev.off()

cat("groups of velocities and angles finished plotting\n")
getTime(programstarttime)

cat("Now plotting the velocity and angle differences\n")

#now set up the output plot location and plot the groups of velocity differences
plotOutput <- paste(extendedPath,"\\velocityDifferences1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(velocityDifferenceStack1,layout=c(4,4),main=list(label="Velocity Differences 1", cex=4.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=velocityDifferenceColors,at=velocityDifferenceBreaks,
	colorkey=list(at=velocityDifferenceBreaks,labels=list(at=velocityDifferenceBreaks,cex=3),width=4,height=0.95)))
grid.text('difference (m/s)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
dev.off()

#now set up the output plot location and plot the groups of angle differences
plotOutput <- paste(extendedPath,"\\angleDifferences1.png", sep="")
png(filename=plotOutput,width=graphWidth,height=graphHeight)
print(levelplot(angleDifferenceStack1,layout=c(4,4),main=list(label="Angle Differences 1", cex=4.5,lines=2),
	xlab=list(label="longitude",cex=4,fontface="bold",lines=2),ylab=list(label="latitude",cex=4,
	fontface="bold",lines=2),par.strip.text=list(cex=2.5,lines=1,fontface='bold'),
	scales=list(x=list(cex=3),y=list(cex=3)),col.regions=angleDifferenceColors,at=angleDifferenceBreaks,
	colorkey=list(at=angleDifferenceBreaks,labels=list(at=angleDifferenceBreaks,cex=3),width=4,height=0.95)))
grid.text('difference (degrees)',rot=90,y=unit(0.5,"npc"),x=unit(0.975,"npc"),gp=gpar(fontsize=14,fontface="bold",cex=3))
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