#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project\\textOutputGetter.R')

#the 'rm(list = ls())' command deletes everything in the workplace

programstarttime <- proc.time()[3]

#import libraries


#variables you can change
mainFileLocation <- "C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project"
elevationName <- "BigButteSmall"
meshResolution <- "Coarse Laptop"
iterationOfInterest <- "10"

names <- c("k","epsilon","pressure","Uz","Uy","Ux")  #with the current setup, I can't change the order of these. This is because I need to be able to go back a line each time in the data read

#variables might need to change, but for now should stay the same
extendedPath <- paste(mainFileLocation,"\\", elevationName,"\\",meshResolution,"\\10mph220deg",sep="")

commandTextFileName <- paste(extendedPath,"\\",iterationOfInterest," iterations\\command_output.txt", sep="")

all_data <- readLines(commandTextFileName)
all_data_length <- length(all_data)

for (i in 1:all_data_length)
{
	spot <- all_data_length-i+1
	current_string <- substr(all_data[spot[1]],0,29)
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
time_step <- substr(all_data[spot[1]-14],8,nchar(all_data[spot[1]-14]))

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
textOutput <- paste(extendedPath,"\\overall_data_frame.txt", sep="")
sink(textOutput)
cat(paste("\nThis is the iteration data for the ",iterationOfInterest," file:\n",sep=""))
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
	cat(paste(total_simulation_time/60,"minutes\n"))
} else
{
	cat(paste(total_simulation_time/3600,"hours\n"))
}
sink()


endtime <- proc.time()[3]-programstarttime
cat(paste("The total program runtime is:",endtime,"seconds\n"))
cat("Program has finished. Have a nice day!\n")