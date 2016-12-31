#!/usr/bin/env Rscript

library(ggplot2)
library(scales)
library(sitools)
library(RColorBrewer)

p <- ggplot(legend = TRUE) + ggtitle('MCs & PRNGs benchmark') +
  scale_y_continuous(labels = comma, limits = c(0, 1.1 * 50000)) +
  scale_x_continuous(labels = comma) +
  xlab("Received Bytes") + ylab("Lookup Time")

file_list = list.files(pattern = "*.csv")

stl_data <- data.frame(algorithm=character(),
                       bytes=integer(),
                       rate=double(),
                       stringsAsFactors=FALSE)
lib_cm_data <- data.frame(algorithm=character(),
                       bytes=integer(),
                       rate=double(),
                       stringsAsFactors=FALSE)
lib_prng_data <- data.frame(algorithm=character(),
                       bytes=integer(),
                       rate=double(),
                       stringsAsFactors=FALSE)

for (i in 1:length(file_list)) {
  df <- read.csv(file_list[i], head = TRUE, sep = ",")
  print(ggplot(df, aes(x = bytes, y = rate)) + geom_point()) # [DEBUG]
  algorithm = gsub(".csv", "", file_list[i])
  print(algorithm)
  df$algorithm <- rep(algorithm, nrow(df))
  if (startsWith(algorithm, "STL")) {
    stl_data <<- rbind(stl_data, df)
  }
  if (startsWith(algorithm, "CHAOS_MACHINE")) {
    lib_cm_data <<- rbind(lib_cm_data, df)
  }
  if (startsWith(algorithm, "CHAOS_PRNG")) {
    lib_prng_data <<- rbind(lib_prng_data, df)
  }
}

stl_data <- stl_data[with(stl_data, order(-rate)), ]
rownames(stl_data) <- 1:nrow(stl_data)

lib_cm_data <- lib_cm_data[with(lib_cm_data, order(-rate)), ]
rownames(lib_cm_data) <- 1:nrow(lib_cm_data)

lib_prng_data <- lib_prng_data[with(lib_prng_data, order(-rate)), ]
rownames(lib_prng_data) <- 1:nrow(lib_prng_data)

p <- p + geom_smooth(data = stl_data,
                         mapping = aes(x = bytes, y = rate,
                                       colour = algorithm),
                         method = "loess", span = 0.20, se = FALSE)
p <- p + geom_smooth(data = lib_cm_data,
                         mapping = aes(x = bytes, y = rate,
                                       colour = algorithm),
                         linetype = "dashed",
                         method = "loess", span = 0.20, se = FALSE)
p <- p + geom_smooth(data = lib_prng_data,
                         mapping = aes(x = bytes, y = rate,
                                       colour = algorithm),
                         method = "loess", span = 0.20, se = FALSE)

# DEBUG [number of colors]
my_colors = c(brewer.pal(6, "Reds"),
              c("royalblue", "royalblue2", "royalblue4"),
              brewer.pal(5, "Greens"))
my_headers = c(unique(stl_data$algorithm),
               unique(lib_cm_data$algorithm),
               unique(lib_prng_data$algorithm))

p <- p + theme(legend.position = "bottom") +
         theme(legend.title = element_blank()) +
         theme(legend.key = element_rect(size = 5)) +
         theme(legend.key.size = unit(1.5, 'lines'))

p <- p + scale_colour_manual(
          limits = my_headers,
          breaks = my_headers,
          values = my_colors)

ggsave(p, file = "benchmark.svg", width = 12, height = 5)
