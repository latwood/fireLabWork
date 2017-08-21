#start by removing everything in the workplace after telling the user 
#that this will remove everything

cat("\nThe program starts by removing everything from the workplace by using the 
'rm(list = ls())' command.\n")

continue <- ""
while(continue != "yes" && continue != "no")
{
	continue <- readline("Do you want to continue? Type 'yes' or 'no' exactly as shown: ")

	if(continue != "yes" && continue != "no")
		{
			cat("Please type 'yes' or 'no' exactly as shown. \n")
		} else if(continue == "no")
		{
			cat("Thank you for your input, this function has ended without running. \n")
			line <- readline("Press [enter] or [space] to continue: ")
		} else
		{
			cat("The program will now remove everything from the workplace and proceed. \n")
			line <- readline("Press [enter] or [space] to continue: ")
		}
}

if(continue == "yes")
{
	cat("Removing everything from the workplace \n")
	cat(". . .\n")
	rm(list = setdiff(ls(), "continue"))
	cat(". . .\n")
	cat("done \n")
	cat("Uploading the burning data csv file from the current directory \n")
	cat(". . . \n")
	burn_data <- read.csv(file="C:\\Users\\latwood\\Documents\\MyDocuments\\Rtutorial\\PracticeData.csv",header=TRUE,sep=",")
	cat(". . . \n")
	cat("done \n")
	cat("Showing resulting file: \n")
	cat(". . . \n ")
	line <- readline("Press [enter] or [space] to continue: ")
	cat("\n")
	print(burn_data,digits=5)
	cat("\n")
	line <- readline("Press [enter] or [space] to continue: ")
	cat(". . .\n")
	cat("Plotting concentrations vs equivalence ratio phi \n")
	cat(". . . \n")

	plot(burn_data$phi,burn_data$y.CH4prod,type="b",pch=0,xlab="Equivalence Ratio: phi",ylab="mol fraction",main="
		Products of Complete Combustion (PCC) of Methane",xlim=c(0.1,4))
	points(burn_data$phi,burn_data$y.O2prod,type="b",col=2,pch=1)
	points(burn_data$phi,burn_data$y.N2prod,type="b",col=3,pch=2)
	points(burn_data$phi,burn_data$y.CO2prod,type="b",col=4,pch=3)
	points(burn_data$phi,burn_data$y.H2Oprod,type="b",col=5,pch=4)
	legend("top",ncol=2,c("CH4","O2","N2","CO2","H2O"),col=c(1,2,3,4,5),pch=c(0,1,2,3,4))
	

	cat(". . . \n")
	cat("The program is complete.\n")
	line <- readline("Press [enter] or [space] to continue: ")
}