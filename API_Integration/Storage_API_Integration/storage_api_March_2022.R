library(httr)
library(RJSONIO)
library(ggplot2)

#insanely long API key to provide to the TTN server
apiKey <- 'NNSXS.LWN6CXIYJAWTFDE6N33R6PUOBDTPL5PHG6KALXA.Z5SCZYW2SWVGMSURGLSXJBQVG55VF3KEPBPU2FGAODMBSYD6H2TA'

#name of the application in TTN
theApplication = "amsel-tags-2021"

#the fields that you want to download
theFields = "up.uplink_message.decoded_payload"

  #self explanatory
  theNumberOfRecords = 1800
  #build the URL to provide to CURL via httr
  theURL = paste("https://eu1.cloud.thethings.network/api/v3/as/applications/", theApplication, "/packages/storage/uplink_message?order=-received_at&limit=",theNumberOfRecords,"&field_mask=",theFields, sep="")

  
result <- httr::GET(theURL, add_headers(Accept = 'text/event-stream', Authorization = paste('Bearer', apiKey)))

result <- httr::content(result, as='text', encoding="UTF-8")

result <- paste("{\"data\": [", gsub("\n\n", ",", result), "]}", sep="")

result <- RJSONIO::fromJSON(result)

result <- as.data.frame(t(matrix(unlist(result), nrow=length(unlist(result$data[1])))))

result <- result[,-10]

colnames(result) <- c("id", "message_time", "volts", "d/m/y", "hdop", "latitude", "longitude", "siv", "h/m/s/cs")

result$latitude <- as.numeric(result$latitude)
result$longitude <- as.numeric(result$longitude)

setwd("//Users/ryanshipley/Documents/Github/Solar_LoRa_GPS/API_Integration/Stored_Data") ##Laptop

curr_date <- Sys.Date()

file_name <- paste0(curr_date, "-GPS-records.csv")

write.csv(result, file_name)


