library(httr)
library(RJSONIO)
library(ggplot2)

options(digits=8)


#berchtesgaden
apiKey <- 'NNSXS.HQFNBW6LOIGIC4U3OO7MRGP6JD4KBKDTSQNGWLQ.MD7OGGWO6GJE4IN7PZVMFM3N6NVGLAZH5J23ANN4DPVNUOV2B5QA'
theApplication = "berchtesgaden-2022-choughs-test"
setwd("//Users/ryanshipley/Documents/Github/Solar_LoRa_GPS/API_Integration/Test_Data") ##Laptop

theFields = "up.uplink_message.decoded_payload"

theNumberOfRecords = 3600

theURL = paste("https://eu1.cloud.thethings.network/api/v3/as/applications/", theApplication, "/packages/storage/uplink_message?order=-received_at&limit=",theNumberOfRecords,"&field_mask=",theFields, sep="")

#theURL = paste("https://au1.cloud.thethings.network/api/v3/as/applications/", theApplication, "/packages/storage/uplink_message?order=-received_at&last=186400s", "&field_mask=", theFields, sep="")

result <- httr::GET(theURL, add_headers(Accept = 'text/event-stream', Authorization = paste('Bearer', apiKey)))

result <- httr::content(result, as='text', encoding="UTF-8")

result <- paste("{\"data\": [", gsub("\n\n", ",", result), "]}", sep="")

result <- RJSONIO::fromJSON(result, asText = TRUE )

result <- as.data.frame(t(matrix(unlist(result), nrow=length(unlist(result$data[1])))))

result <- result[,-10]

colnames(result) <- c("id", "message_time", "volts", "d/m/y", "hdop", "latitude", "longitude", "siv", "h/m/s/cs")

result$latitude <- as.numeric(result$latitude)
result$longitude <- as.numeric(result$longitude)

result$volts <- as.numeric(result$volts)
result$volts <- as.numeric(result$volts)

result$time_stamp <- lubridate::ymd_hms(result$message_time)

result$id_6 <- stringr::str_sub(result$id, -6, -1)

result_test <- result[, c("id", "message_time", "volts", "latitude", "longitude", "time_stamp")]


apiKey <- 'NNSXS.2DPWFOWGNZPFVRDRJFR2JOKJYGDFZWDLJZUWZGY.UXY264XINHEDNQFYMXNV42JPW7CQKFUV5NHAS7AOAAQF5YBAUUVQ'
theApplication = "amsel-group-7-2022"

setwd("//Users/ryanshipley/Documents/Research/2022_Blackbirds_Release/Data/Group_7") ##Laptop

theURL = paste("https://eu1.cloud.thethings.network/api/v3/as/applications/", theApplication, "/packages/storage/uplink_message?order=-received_at&limit=",theNumberOfRecords,"&field_mask=",theFields, sep="")

result <- httr::GET(theURL, add_headers(Accept = 'text/event-stream', Authorization = paste('Bearer', apiKey)))
result <- httr::content(result, as='text', encoding="UTF-8")
result <- paste("{\"data\": [", gsub("\n\n", ",", result), "]}", sep="")
result <- RJSONIO::fromJSON(result, asText = TRUE )
result <- as.data.frame(t(matrix(unlist(result), nrow=length(unlist(result$data[1])))))
result <- result[,-16]

colnames(result) <- c("id", "message_time", "day", "hdop", "historical", "hour", "latitude", "longitude", "minute", "month", "pressure", "second", "temperature", "volts", "year")

result$latitude <- as.numeric(result$latitude)
result$longitude <- as.numeric(result$longitude)

result$time_stamp <- lubridate::ymd_hms(result$message_time)

result <- result[ which(result$id == "eui-70b3d57ed005497f" | result$id ==  "eui-70b3d57ed0054980" | result$id == "eui-70b3d57ed0054981" |
                         result$id == "eui-70b3d57ed0054982" | result$id == "eui-70b3d57ed0054983" | result$id == "eui-70b3d57ed0054984" ),]

result <- result[, c("id", "message_time", "volts", "latitude", "longitude", "time_stamp")]

result <- rbind.data.frame(result, result_test)

result$volts <- as.numeric(result$volts)

ggplot2::ggplot(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts))+
  annotate(geom="rect", ymin=3.95, ymax=4.3, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#CAFFBF", alpha=0.8)+
  annotate(geom="rect", ymin=3.68, ymax=3.95, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FDFFB6", alpha=0.8)+
  annotate(geom="rect", ymin=3.58, ymax=3.68, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FFD6A5", alpha=0.8)+
  annotate(geom="rect", ymin=-Inf, ymax=3.58, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FFADAD", alpha=0.8)+
  geom_point(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts, color=id))+
  geom_line(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts, color=id))+
  geom_point(data=result[ which(result$volts > 3.10 & result$id == "eui-70b3d57ed0054980" | result$id ==  "eui-70b3d57ed0054982"),], aes(time_stamp, volts, group=id), color="black")+
  geom_line(data=result[ which(result$volts > 3.10 & result$id == "eui-70b3d57ed0054980" | result$id ==  "eui-70b3d57ed0054982"),], aes(time_stamp, volts, group=id), color="black")+
  cowplot::theme_cowplot()+
  scale_y_continuous(breaks=c(3.2, 3.4, 3.6, 3.8, 4.0, 4.2), limits=c(3.2,4.4))
#geom_smooth(se=FALSE, method="lm",data=result[ which(result$volts > 3.10),], aes(time_stamp, volts, color=id))+

setwd("//Users/ryanshipley/Documents/Research/2022_Blackbirds_Release/Performance") ##Laptop

curr_date <- Sys.Date()

file_name <- paste0(curr_date, "-GPS-records.csv")

write.csv(result, file_name)

