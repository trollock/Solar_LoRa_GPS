library(httr)
library(RJSONIO)
library(ggplot2)

options(digits=8)

#blackbirds
apiKey <- 'NNSXS.LWN6CXIYJAWTFDE6N33R6PUOBDTPL5PHG6KALXA.Z5SCZYW2SWVGMSURGLSXJBQVG55VF3KEPBPU2FGAODMBSYD6H2TA'
theApplication = "amsel-tags-2021"
setwd("//Users/ryanshipley/Documents/Github/Solar_LoRa_GPS/API_Integration/Stored_Data") ##Laptop

#cockatoos
#apiKey <- 'NNSXS.74EQKFHALY6FTUFEKYUE55BGVXMVW5U75E7ECRQ.JN5G2JCFMDTG2UF6DH4FYIFLFTSL4U77SRFFIE52MGHH2OCJKIRQ'
#theApplication = "aplin-cockatoo-2021"

#berchtesgaden
#apiKey <- 'NNSXS.HQFNBW6LOIGIC4U3OO7MRGP6JD4KBKDTSQNGWLQ.MD7OGGWO6GJE4IN7PZVMFM3N6NVGLAZH5J23ANN4DPVNUOV2B5QA'
#theApplication = "berchtesgaden-2022-choughs-test"
#setwd("//Users/ryanshipley/Documents/Github/Solar_LoRa_GPS/API_Integration/Test_Data") ##Laptop

#test_tags
#apiKey <- 'NNSXS.76D5CPNMD6VAUZKXYVXFLREQDLT5ML5BNH5A4TI.HAKCARLNAIQ3Y6BLMOVOU7MDKBGJ57J5DZXTAKH6EDBEATY5HVIQ'
#theApplication = "tag-prototypes-2022"
#setwd("//Users/ryanshipley/Documents/Github/Solar_LoRa_GPS/API_Integration/Test_Tag_Data") ##Laptop

theFields = "up.uplink_message.decoded_payload"

theNumberOfRecords = 3600

theURL = paste("https://eu1.cloud.thethings.network/api/v3/as/applications/", theApplication, "/packages/storage/uplink_message?order=-received_at&limit=",theNumberOfRecords,"&field_mask=",theFields, sep="")

#theURL = paste("https://au1.cloud.thethings.network/api/v3/as/applications/", theApplication, "/packages/storage/uplink_message?order=-received_at&last=186400s", "&field_mask=", theFields, sep="")

result <- httr::GET(theURL, add_headers(Accept = 'text/event-stream', Authorization = paste('Bearer', apiKey)))

result <- httr::content(result, as='text', encoding="UTF-8")

result <- paste("{\"data\": [", gsub("\n\n", ",", result), "]}", sep="")

result <- RJSONIO::fromJSON(result, asText = TRUE )

#result <- rjson::fromJSON(result)

#result <- result %>% 
#  spread_all

#result <- result[[1]][lengths(result[[1]]) != 0]

result <- as.data.frame(t(matrix(unlist(result), nrow=length(unlist(result$data[1])))))

result <- result[,-10]

colnames(result) <- c("id", "message_time", "volts", "d/m/y", "hdop", "latitude", "longitude", "siv", "h/m/s/cs")

result$latitude <- as.numeric(result$latitude)
result$longitude <- as.numeric(result$longitude)

curr_date <- Sys.Date()

file_name <- paste0(curr_date, "-GPS-records.csv")

write.csv(result, file_name)

result$volts <- as.numeric(result$volts)
result$volts <- as.numeric(result$volts)

result$time_stamp <- lubridate::ymd_hms(result$message_time)

result$id_6 <- stringr::str_sub(result$id, -6, -1)

#result <- result[which(result$id_6 == "04476c" | result$id_6 == "04476d" | result$id_6 == "04476e" |
#                        result$id_6 == "04476f" | result$id_6 == "044770" | result$id_6 == "044a2b" |
#                         result$id_6 == "044a2c" | result$id_6 == "044a2d" | result$id_6 == "044a2f"),]

ggplot2::ggplot(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts))+
                annotate(geom="rect", ymin=3.95, ymax=4.3, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#CAFFBF", alpha=0.8)+
                annotate(geom="rect", ymin=3.68, ymax=3.95, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FDFFB6", alpha=0.8)+
                annotate(geom="rect", ymin=3.58, ymax=3.68, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FFD6A5", alpha=0.8)+
                annotate(geom="rect", ymin=-Inf, ymax=3.58, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FFADAD", alpha=0.8)+
                geom_point(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts, color=id))+
                geom_line(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts, color=id))+
                cowplot::theme_cowplot()+
                scale_y_continuous(breaks=c(3.2, 3.4, 3.6, 3.8, 4.0, 4.2), limits=c(3.2,4.4))
                #geom_smooth(se=FALSE, method="lm",data=result[ which(result$volts > 3.10),], aes(time_stamp, volts, color=id))+

ggplot2::ggplot(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts))+
  annotate(geom="rect", ymin=3.95, ymax=4.3, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#CAFFBF", alpha=0.8)+
  annotate(geom="rect", ymin=3.68, ymax=3.95, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FDFFB6", alpha=0.8)+
  annotate(geom="rect", ymin=3.58, ymax=3.68, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FFD6A5", alpha=0.8)+
  annotate(geom="rect", ymin=-Inf, ymax=3.58, xmin = min(result$time_stamp), xmax = max(result$time_stamp), fill="#FFADAD", alpha=0.8)+
  geom_point(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts))+
  geom_line(data=result[ which(result$volts > 3.10),], aes(time_stamp, volts))+
  cowplot::theme_cowplot()+
  scale_y_continuous(breaks=c(3.2, 3.4, 3.6, 3.8, 4.0, 4.2), limits=c(3.2,4.4)) +
  facet_wrap(.~id_6, ncol=10)



