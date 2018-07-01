###command to run this program (includes the current file location)
#source('/home/latwood/src/fireLabWork/R/farsite/RawsToWindAndWeather.R')

### Notes
## I tried to keep this simple, but also error check for a bunch of stuff. Wish I had an exit function, that would be much
## cleaner than what I currently have here and would make the code much, much more readable. Also some of the logic is a little
## annoying, and I felt that especially with the weather file stuff, there should be a way to combine some of the multiple steps
## if I were to somehow improve the logic.
## That being said, it appears that it currently outputs the right information, though the format is nasty for reading it,
## as there aren't equal spaces between the data. I think some kind of findNeededWhiteSpace function could be very helpful,
## where it looks through the data of a given column for the biggest one up to that point and adds the needed whitespace to make
## the difference. Would also be nice to make the precipitations print out with two decimals, should be easy but I got lazy
## cause I just wanted to get this function done.


###import libraries


###variables you can change
mainFileLocation <- "/home/latwood/Documents/windninjaPanther/test1177973/currentInput/"
rawsDataFile <- "test1177973.input"
windDataFile <- "RAWStoWind.wnd"
weatherDataFile <- "RAWStoWeather.wtr"
rawsUnitsLine <- 25
rawsElevationLine <- 24
rawsDataCountLine <- 27


###include functions in this section
findUniqueDates <- function(years,months,days)
{
  if(length(years) != length(months) && length(months) != length(days))
  {
    cat("!!!Error in findUniqueDates! number of years, months, days, and hours doesn't match somehow! Script continues simply cause not sure how to end it!\n")
    return(list("error"))
  } else
  {
    numDates <- length(years)
    uniqueYears <- years[1]
    uniqueMonths <- months[1]
    uniqueDays <- days[1]
    uniqueDatesCount <- 2
    for(datesCount in 2:numDates)
    {
      if(years[datesCount] != years[datesCount-1] || months[datesCount] != months[datesCount-1] || days[datesCount] != days[datesCount-1])
      {
        uniqueYears[uniqueDatesCount] <- years[datesCount]
        uniqueMonths[uniqueDatesCount] <- months[datesCount]
        uniqueDays[uniqueDatesCount] <- days[datesCount]
        uniqueDatesCount <- uniqueDatesCount + 1
      }
    }
    
    # prepare output
    if(length(uniqueYears) != length(uniqueMonths) && length(uniqueMonths) != length(uniqueDays) 
       && length(uniqueDays) != length(uniqueHours))
    {
      cat("!!!Error in findUniqueDates! number of uniqueYears, uniqueMonths, and uniqueDays doesn't match somehow! Script continues simply cause not sure how to end it!\n")
      return(list("error"))
    }
    uniqueDates <- list(uniqueYears,uniqueMonths,uniqueDays)
    return(uniqueDates)
  }
}

###now run the program, yay!

## first process input data
rawsInputFile <- paste(mainFileLocation,rawsDataFile, sep="")
inputRawsData <- readLines(rawsInputFile)
inputRawsDataLength <- length(inputRawsData)

foundBadRaws <- FALSE
if(inputRawsDataLength == 0)
{
  cat("!!!Error! no data in RAWS file!\n")
  foundBadRaws <- TRUE
} else 
{
  if(inputRawsDataLength < rawsUnitsLine)
  {
    cat("!!!Error! Number of lines in RAWS file is less than rawsUnitsLine!\n")
    foundBadRaws <- TRUE
  } else if(inputRawsDataLength < rawsElevationLine)
  {
    cat("!!!Error! Number of lines in RAWS file is less than rawsElevationLine!\n")
    foundBadRaws <- TRUE
  } else if(inputRawsDataLength < rawsDataCountLine)
  {
    cat("!!!Error! Number of lines in RAWS file is less than rawsDataCountLine!\n")
    foundBadRaws <- TRUE
  } else
  {
    rawsUnitsString <- inputRawsData[rawsUnitsLine]
    if(rawsUnitsString != "RAWS_UNITS: English" && rawsUnitsString != "RAWS_UNITS: Metric")
    {
      cat("!!!Error! rawsUnits are neither Metric or English!\n")
      foundBadRaws <- TRUE
    } else
    {
      if(rawsUnitsString == "RAWS_UNITS: Metric")
      {
        rawsUnits <- "Metric"
      } else
      {
        rawsUnits <- "English"
      }
      rawsElevationString <- inputRawsData[rawsElevationLine]
      rawsElevationStringNchar <- nchar(rawsElevationString)
      if(rawsElevationStringNchar <= 15)
      {
        cat("!!!Error! rawsElevationLine is not \"RAWS_ELEVATION: aNumber\"!\n")
        foundBadRaws <- TRUE
      } else
      {
        rawsElevationStringPart <- substr(rawsElevationString,16,rawsElevationStringNchar)
        rawsElevationNumber <- suppressWarnings(as.numeric(rawsElevationStringPart))
        if(is.na(rawsElevationNumber))
        {
          cat("!!!Error! rawsElevationLine \"RAWS_ELEVATION: aNumber\", aNumber is not a number!\n")
          foundBadRaws <- TRUE
        } else
        {
          rawsDataCountString <- inputRawsData[rawsDataCountLine]
          rawsDataCountStringNchar <- nchar(rawsDataCountString)
          if(rawsDataCountStringNchar <= 5)
          {
            cat("!!!Error! rawsDataCountLine is not \"RAWS: aNumber\"!\n")
            foundBadRaws <- TRUE
          } else
          {
            rawsDataCountStringPart <- substr(rawsDataCountString,6,rawsDataCountStringNchar)
            rawsDataCountNumber <- suppressWarnings(as.numeric(rawsDataCountStringPart))
            if(is.na(rawsDataCountNumber))
            {
              cat("!!!Error! rawsDataCountLine \"RAWS: aNumber\", is not a number!\n")
              foundBadRaws <- TRUE
            } else
            {
              if(inputRawsDataLength < (rawsDataCountLine + rawsDataCountNumber))
              {
                cat("!!!Error! not enough lines in the input file for the rawsDataCountNumber!\n")
                foundBadRaws <- TRUE
              } else
              {
                rawsData <- matrix(nrow=rawsDataCountNumber,ncol=10)
                for(rawsCount in 1:rawsDataCountNumber)
                {
                  currentLine <- inputRawsData[rawsCount+rawsDataCountLine]
                  isProcessingData <- FALSE
                  dataCount <- 1
                  for(charCount in 1:nchar(currentLine))
                  {
                    currentChar <- substr(currentLine,charCount,charCount)
                    if(currentChar != " ")
                    {
                      if(isProcessingData == FALSE)
                      {
                        startSpot <- charCount
                        isProcessingData <- TRUE
                      }
                    } else
                    {
                      if(isProcessingData == TRUE)
                      {
                        rawsData[rawsCount,dataCount] <- suppressWarnings(as.numeric(substr(currentLine,startSpot,charCount)))
                        if(is.na(rawsDataCountNumber))
                        {
                          cat(paste("!!!Error! nonnumeric data at rawsCount = ",rawsCount,", dataCount = ",dataCount,"!\n",sep=""))
                          foundBadRaws <- TRUE
                        }
                        isProcessingData <- FALSE
                        dataCount <- dataCount + 1
                      }
                    }
                    if(charCount == nchar(currentLine))
                    {
                      if(isProcessingData == TRUE)
                      {
                        rawsData[rawsCount,dataCount] <- suppressWarnings(as.numeric(substr(currentLine,startSpot,charCount)))
                        if(is.na(rawsDataCountNumber))
                        {
                          cat(paste("!!!Error! nonnumeric data at rawsCount = ",rawsCount,", dataCount = ",dataCount,"!\n",sep=""))
                          foundBadRaws <- TRUE
                        }
                        isProcessingData <- FALSE
                        dataCount <- dataCount + 1
                      }
                      if(dataCount < 9)
                      {
                        cat(paste("!!!Error! not enough columns of data at rawsCount = ",rawsCount,". dataCount = ",dataCount,"!\n",sep=""))
                        foundBadRaws <- TRUE
                      }
                    }
                  } #end charCount
                } #end rawsCount, #end parsing rawsDataInput
              } #"!!!Error! not enough lines in the input file for the rawsDataCountNumber!\n"
            } #"!!!Error! rawsDataCountLine \"RAWS: aNumber\", aNumber is not a number!\n"
          } #"!!!Error! rawsDataCountLine is not \"RAWS: aNumber\"!\n"
        } #"!!!Error! rawsElevationLine \"RAWS_ELEVATION: aNumber\", aNumber is not a number!\n"
      } #"!!!Error! rawsElevationLine is not \"RAWS_ELEVATION: aNumber\"!\n"
    } #"!!!Error! rawsUnits are neither Metric or English!\n"
  } #"!!!Error! Number of lines in RAWS file is less than rawsUnitsLine or rawsDataCountLine!\n"
} #"!!!Error! no data in RAWS file!\n"

## now output data

if(foundBadRaws == TRUE)
{
  cat("Found bad raws data, ending script\n")
} else
{
  years <- rawsData[,1]
  months <- rawsData[,2]
  days <- rawsData[,3]
  hours <- rawsData[,4]
  temperatures <- rawsData[,5]
  humidities <- rawsData[,6]
  precipitations <- rawsData[,7]
  windSpeeds <- rawsData[,8]
  windDirections <- rawsData[,9]
  cloudCovers <- rawsData[,10]
  
  # do the wind output data
  windOutputFile <- paste(mainFileLocation,windDataFile, sep="")
  if(file.exists(windOutputFile))
  {
    cat("windOutputFile already exists, continuing script without writing file\n")
  } else
  {
    sink(windOutputFile)
    cat(paste(rawsUnits,"\n",sep=""))
    for(rawsCount in 1:rawsDataCountNumber)
    {
      cat(paste(months[rawsCount],days[rawsCount],hours[rawsCount],
                windSpeeds[rawsCount],windDirections[rawsCount],cloudCovers[rawsCount],sep=" "))
      cat("\n")
    }
    sink()
  } #"windOutputFile already exists, continuing script without writing file\n"
  
  # do the weather output data
  weatherOutputFile <- paste(mainFileLocation,weatherDataFile, sep="")
  if(file.exists(weatherOutputFile))
  {
    cat("weatherOutputFile already exists, continuing script without writing file\n")
  } else
  {
    uniqueDates <- findUniqueDates(years,months,days)
    uniqueYears <- uniqueDates[[1]]
    uniqueMonths <- uniqueDates[[2]]
    uniqueDays <- uniqueDates[[3]]
    finalWtrMonths <- 0
    finalWtrDays <- 0
    finalWtrPrecips <- 0
    finalWtrHoursMin <- 0
    finalWtrHoursMax <- 0
    finalWtrTemperaturesMin <- 0
    finalWtrTemperaturesMax <- 0
    finalWtrHumiditiesMax <- 0
    finalWtrHumiditiesMin <- 0
    finalWtrElevations <- rawsElevationNumber  #note that there is only a single elevation given in the RAWS file
    finalWtrPrecipStartHours <- 0
    finalWtrPrecipEndHours <- 0
    
    hourGreatestMax <- 2399
    hourLeastMin <- 0
    # there might be some issues with the humidity being chosen this way. 
    # Might actually supposed to be the humidity found at the min and max temperatures, so not really a min or max humidity
    humidityGreatestMax <- 100
    humidityLeastMin <- 0
    
    # set starting values as needed
    firstDayHour <- hours[1] # need to set this for the first time, or use an if statement inside the loop. Whether it is a realistic hour is checked in the loop
    hourMin <- hourGreatestMax
    hourMax <- hourLeastMin
    temperatureMin <- 99999
    temperatureMax <- 0
    humidityMin <- humidityGreatestMax
    humidityMax <- humidityLeastMin
    firstPrecipHour <- hourLeastMin
    foundFirstPrecipHour <- FALSE
    lastPrecipHour <- hourGreatestMax
    foundLastPrecipHour <- FALSE
    totalDailyPrecip <- 0
    datesCount <- 1
    
    for(rawsCount in 1:rawsDataCountNumber)
    {
      # check for error
      if(hours[rawsCount] < hourLeastMin || hours[rawsCount] > hourGreatestMax)
      {
        cat(paste("!!!Error! hours for rawsCount = ",rawsCount,", aren't realistic!\n",sep=""))
      } #paste("!!!Error! hours for rawsCount = ",rawsCount,", aren't realistic!\n",sep="")
      if(humidities[rawsCount] < humidityLeastMin || humidities[rawsCount] > humidityGreatestMax)
      {
        cat(paste("!!!Error! humidities for rawsCount = ",rawsCount,", aren't realistic!\n",sep=""))
      } #paste("!!!Error! humidities for rawsCount = ",rawsCount,", aren't realistic!\n",sep="")
      
      # on similar days, process information and store temporary information
      if(days[rawsCount] == uniqueDays[datesCount])
      { 
        # find min and max temperatures
        if(temperatures[rawsCount] < temperatureMin)
        {
          temperatureMin <- temperatures[rawsCount]
          hourMin <- hours[rawsCount]
        }
        if(temperatures[rawsCount] > temperatureMax)
        {
          temperatureMax <- temperatures[rawsCount]
          hourMax <- hours[rawsCount]
        }
        # find min and max humidities. This might actually need to be replaced with humidities found at the min and max temperatures
        if(humidities[rawsCount] < humidityMin)
        {
          humidityMin <- humidities[rawsCount]
        }
        if(humidities[rawsCount] > humidityMax)
        {
          humidityMax <- humidities[rawsCount]
        }
        
        # find precipitation, specifically first and last hour and accumulative precip
        # only first precip hour actually loads in the precipitation to avoid double counting
        if(foundFirstPrecipHour == FALSE)
        {
          if(precipitations[rawsCount] > 0)
          {
            firstPrecipHour <- hours[rawsCount]
            totalDailyPrecip <- precipitations[rawsCount]
            foundFirstPrecipHour <- TRUE
          }
        } else
        {
          if(hours[rawsCount] <= lastPrecipHour)
          {
            totalDailyPrecip <- totalDailyPrecip + precipitations[rawsCount]
          }
        }
        if(foundLastPrecipHour == FALSE)
        {
          if(precipitations[rawsCount] > 0)
          {
            lastPrecipHour <- hours[rawsCount]
            foundLastPrecipHour <- TRUE
          }
        } #end find precip hours
      } else #found a new day, so need to process and store the information found in the previous day, and reset all values
      {
        # store found output values
        finalWtrMonths[datesCount] <- uniqueMonths[datesCount]
        finalWtrDays[datesCount] <- uniqueDays[datesCount]
        finalWtrPrecips[datesCount] <- totalDailyPrecip
        finalWtrHoursMin[datesCount] <- hourMin
        finalWtrHoursMax[datesCount] <- hourMax
        finalWtrTemperaturesMin[datesCount] <- temperatureMin
        finalWtrTemperaturesMax[datesCount] <- temperatureMax
        finalWtrHumiditiesMax[datesCount] <- humidityMax
        finalWtrHumiditiesMin[datesCount] <- humidityMin
        # notice that elevation is already found as it was a single value in the RAWS. Assigned this at init of loop
        finalWtrPrecipStartHours[datesCount] <- firstPrecipHour
        finalWtrPrecipEndHours[datesCount] <- lastPrecipHour
        # precips are a special case, the hours get off without this special exception
        if(foundFirstPrecipHour == FALSE || foundLastPrecipHour == FALSE)
        {
          if(rawsCount == 1)
          {
            cat("!!!Error! Somehow rawsCount is not 2 for first new found day! Can't handle this error, program should die\n")
          }
          finalWtrPrecipStartHours[datesCount] <- firstDayHour # times for precips get messed up without this section
          finalWtrPrecipEndHours[datesCount] <- hours[rawsCount-1]
          finalWtrPrecips[datesCount] <- 0
        } # finished processing precips outputs
        
        # now reset values for the next loop
        firstDayHour <- hours[rawsCount]
        hourMin <- hourGreatestMax
        hourMax <- hourLeastMin
        temperatureMin <- 99999
        temperatureMax <- 0
        humidityMin <- humidityGreatestMax
        humidityMax <- humidityLeastMin
        firstPrecipHour <- hourLeastMin
        foundFirstPrecipHour <- FALSE
        lastPrecipHour <- hourGreatestMax
        foundLastPrecipHour <- FALSE
        totalDailyPrecip <- 0
        
        # now reset the counter for the next unique day
        datesCount <- datesCount + 1
      } # end unique days or not processing
      
      # need to process the final day if it is not a new day
      if(rawsCount == rawsDataCountNumber)
      {
        if(rawsCount == 1)
        {
          # store found output values
          finalWtrMonths[datesCount] <- uniqueMonths[datesCount]
          finalWtrDays[datesCount] <- uniqueDays[datesCount]
          finalWtrPrecips[datesCount] <- totalDailyPrecip
          finalWtrHoursMin[datesCount] <- hourMin
          finalWtrHoursMax[datesCount] <- hourMax
          finalWtrTemperaturesMin[datesCount] <- temperatureMin
          finalWtrTemperaturesMax[datesCount] <- temperatureMax
          finalWtrHumiditiesMax[datesCount] <- humidityMax
          finalWtrHumiditiesMin[datesCount] <- humidityMin
          # notice that elevation is already found as it was a single value in the RAWS. Assigned this at init of loop
          finalWtrPrecipStartHours[datesCount] <- firstPrecipHour
          finalWtrPrecipEndHours[datesCount] <- lastPrecipHour
          # precips are a special case, the hours get off without this special exception
          if(foundFirstPrecipHour == FALSE || foundLastPrecipHour == FALSE)
          {
            finalWtrPrecipStartHours[datesCount] <- firstDayHour # times for precips get messed up without this section
            finalWtrPrecipEndHours[datesCount] <- hours[rawsCount]
            finalWtrPrecips[datesCount] <- 0
          } # finished processing precips outputs
        } else
        {
          # store found output values
          finalWtrMonths[datesCount] <- uniqueMonths[datesCount]
          finalWtrDays[datesCount] <- uniqueDays[datesCount]
          finalWtrPrecips[datesCount] <- totalDailyPrecip
          finalWtrHoursMin[datesCount] <- hourMin
          finalWtrHoursMax[datesCount] <- hourMax
          finalWtrTemperaturesMin[datesCount] <- temperatureMin
          finalWtrTemperaturesMax[datesCount] <- temperatureMax
          finalWtrHumiditiesMax[datesCount] <- humidityMax
          finalWtrHumiditiesMin[datesCount] <- humidityMin
          # notice that elevation is already found as it was a single value in the RAWS. Assigned this at init of loop
          finalWtrPrecipStartHours[datesCount] <- firstPrecipHour
          finalWtrPrecipEndHours[datesCount] <- lastPrecipHour
          # precips are a special case, the hours get off without this special exception
          if(foundFirstPrecipHour == FALSE || foundLastPrecipHour == FALSE)
          {
            finalWtrPrecipStartHours[datesCount] <- firstDayHour # times for precips get messed up without this section
            finalWtrPrecipEndHours[datesCount] <- hours[rawsCount-1]
            finalWtrPrecips[datesCount] <- 0
          } # finished processing precips outputs
        }
      }
      
    } # end rawsCount
    
    # now write the output file
    sink(weatherOutputFile)
    cat(paste(rawsUnits,"\n",sep=""))
    for(datesCount in 1:length(finalWtrMonths))
    {
      cat(paste(finalWtrMonths[datesCount],finalWtrDays[datesCount],finalWtrPrecips[datesCount],finalWtrHoursMin[datesCount],
                finalWtrHoursMax[datesCount],finalWtrTemperaturesMin[datesCount],finalWtrTemperaturesMax[datesCount],
                finalWtrHumiditiesMax[datesCount],finalWtrHumiditiesMin[datesCount],finalWtrElevations,
                finalWtrPrecipStartHours[datesCount],finalWtrPrecipEndHours[datesCount],sep=" "))
      cat("\n")
    }
    sink()
    
  } #"weatherOutputFile already exists, continuing script without writing file\n"
} #"Found bad raws data, ending script\n"
