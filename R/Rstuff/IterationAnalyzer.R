#command to run this program (includes the current file location)
#source('C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project\\IterationAnalyzer.R')

#import libraries
library(sp)
library(raster)

#variables you can change
mainFileLocation <- "C:\\Users\\latwood\\Documents\\MyDocuments\\Windninja\\Iteration_Impact_Project"
elevationName <- "\\BigButteSmall"
meshResolution <- "\\Coarse"
convergedName <- "\\1000"

#variables might need to change, but for now should stay the same
extendedPath <- paste(mainFileLocation, elevationName,meshResolution,"\\10mph220deg",sep="")
f <- list.files(extendedPath)
convergedVelocityName <- paste(extendedPath,convergedName," iterations\\big_butte_small_220_10_31m_vel.asc", sep="")
convergedAngleName <- paste(extendedPath,convergedName," iterations\\big_butte_small_220_10_31m_ang.asc", sep="")

#set up the converged rasters from the files
convergedVelocity <- raster(convergedVelocityName)
convergedAngle <- raster(convergedAngleName)

#start the program by asking if they want to run it
cat("This program will compare results between files,
asking you to type the filename you want compared 
and outputting plots and textfiles into a file location 
based off of the name you specify. Inputs are from the 
same location.\n")

#if they don't want to continue, the program ends
#if they want to continue, an if statement makes all the stuff happen
continue <- readline("Do you want to continue? Type 'yes' or 'no' exactly as shown: ")
while(continue != "no")
{
	if(continue != "yes" && continue != "no")
		{
			cat("Please type 'yes' or 'no' exactly as shown! \n")
			continue <- readline("Do you want to continue? Type 'yes' or 'no' exactly as shown: ")
		} else if(continue == "no")
		{
			cat("Thank you for your input, this program is ending without running \n")
		} else if(continue == "yes")
		{
			cat("The program will now run \n")
		}

	if(continue == "yes")
		{
			#get the filename, it will just die if it isn't correct
			rName <- readline("what number of iterations would you like to run?
type only a number (ie 10 or 100) for a file that
exists or this program will just die
iteration number is:")

			#use the filename to set up the rasters for the unconverged file
			rVelocityName <- paste(extendedPath,"\\",rName," iterations\\big_butte_small_220_10_31m_vel.asc", sep="")
			rAngleName <- paste(extendedPath,"\\",rName," iterations\\big_butte_small_220_10_31m_ang.asc", sep="")
			rVelocity <- raster(rVelocityName)
			rAngle <- raster(rAngleName)

			#set up the percent change raster by using the percent change formula
			#percentChangeVelocity <- abs(convergedVelocity-rVelocity)/convergedVelocity*100
			percentChangeVelocity <- (convergedVelocity-rVelocity)
			#percentChangeAngle <- abs(convergedAngle-rAngle)/convergedAngle*100
			percentChangeAngle <- (convergedAngle-rAngle)

			#now set up the output plot location and plot the velocity change
			plotOutput <- paste(extendedPath,"\\",rName," iterations\\velocityDiff.png", sep="")
			png(filename=plotOutput)
			plot(percentChangeVelocity)
			dev.off()
			
			#now set up the output plot location and plot the angle change
			plotOutput <- paste(extendedPath,"\\",rName," iterations\\angleDiff.png", sep="")
			png(filename=plotOutput)
			plot(percentChangeAngle)
			dev.off()
			
			#Now output a textfile with all the class information and the summary
			textOutput <- paste(extendedPath,"\\",rName," iterations\\percentDiff.txt", sep="")
			sink(textOutput)
			cat("This is the Velocity info for the file being compared\n")
			print(rVelocity)
			print(summary(rVelocity))
			cat("\n\nThis is the Velocity info for the converged file\n")
			print(convergedVelocity)
			print(summary(convergedVelocity))
			cat("\n\nThis is the Velocity info for the percent change, the difference\n")
			print(percentChangeVelocity)
			print(summary(percentChangeVelocity))
			cat("\n\nThis is the Angle info for the file being compared\n")
			print(rAngle)
			print(summary(rAngle))
			cat("\n\nThis is the Angle info for the converged file\n")
			print(convergedAngle)
			print(summary(convergedAngle))
			cat("\n\nThis is the Angle info for the percent change, the difference\n")
			print(percentChangeAngle)
			print(summary(percentChangeAngle))
			sink()
			
			#the program has completed, let the user know
			cat("The program has finished processing your file\n")

		continue <- readline("Do you want process more files? Type 'yes' or 'no' exactly as shown: ")

		}
}

cat("The program has finished. Have a nice day!\n")
line <- readline("Press [enter] or [space] to continue: ")
