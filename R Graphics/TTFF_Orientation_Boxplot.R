a <- c(57,54,63,59,65,56,66,52,56,58,56,74, 54, 57, 65, 69, 55, 61)
b <- c(7,14,13,9,15,16,16,12,16,8,16,14,14,7,15,9,15,11)
c <- c("Normal", "Normal", "Normal", "Normal", "Normal", "Normal", "Inverted", "Inverted", "Inverted", "Inverted", "Inverted", "Inverted", 
       "Battery", "Battery", "Battery", "Battery", "Battery", "Battery" )

fixes <- as.data.frame(cbind(as.numeric(a),as.numeric(b), c)) 
colnames(fixes) <- c("TTFF", "TTWF", "Orientation")
fixes$TTFF <- as.numeric(as.character(fixes$TTFF))
fixes$TTWF <- as.numeric(as.character(fixes$TTWF))

require(ggplot2)
require(cowplot)

theme = theme_set(theme_minimal())
theme = theme_update(legend.position="top", legend.title=element_blank(), panel.grid.major.x=element_blank())

boxplot1 = ggplot(data=fixes, aes(Orientation, TTFF))

#Stylized Boxplot
boxplot1 = boxplot1 + geom_boxplot(outlier.colour = NULL, aes_string(colour="Orientation", fill="Orientation")) + # geom_boxplot(notch=T) to compare groups
  stat_summary(geom = "crossbar", width=0.65, fatten=0, color="white", fun.data = function(x){ return(c(y=median(x), ymin=median(x), ymax=median(x))) })

#No X Axis
theme = theme_update(axis.ticks.x = element_blank(), axis.line.x = element_blank())
#No Y Axis
theme = theme_update(axis.ticks.y = element_blank(), axis.line.y = element_blank())
#No Y Axis Label + Grey Axis Numbers
theme = theme_update(axis.line.y = element_blank(), axis.text.y = element_text(colour="grey"), axis.ticks.y= element_line(colour="grey"), axis.title.y= element_text(colour="grey"), axis.title.x= element_text(colour="grey"))

boxplot1 = boxplot1 + 
  ylab("TTFF (seconds)") + 
  xlab ("Absolute CAM-M8Q Orientation")+
  geom_jitter(color="grey70", width=0.05)

boxplot1

boxplot2 = ggplot(data=fixes, aes(Orientation, TTWF))

#Stylized Boxplot
boxplot2 = boxplot2 + geom_boxplot(outlier.colour = NULL, aes_string(colour="Orientation", fill="Orientation")) + # geom_boxplot(notch=T) to compare groups
  stat_summary(geom = "crossbar", width=0.65, fatten=0, color="white", fun.data = function(x){ return(c(y=median(x), ymin=median(x), ymax=median(x))) })

#No X Axis
theme = theme_update(axis.ticks.x = element_blank(), axis.line.x = element_blank())
#No Y Axis
theme = theme_update(axis.ticks.y = element_blank(), axis.line.y = element_blank())
#No Y Axis Label + Grey Axis Numbers
theme = theme_update(axis.line.y = element_blank(), axis.text.y = element_text(colour="grey"), axis.ticks.y= element_line(colour="grey"), axis.title.y= element_text(colour="grey"), axis.title.x= element_text(colour="grey"))

boxplot2 = boxplot2 + 
  ylab("TTWF (seconds)") + 
  xlab ("Absolute CAM-M8Q Orientation")+
  geom_jitter(color="grey70", width=0.05)

boxplot2