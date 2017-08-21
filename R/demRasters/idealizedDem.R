library(raster)

prj<-"+proj=utm +zone=12 +datum=WGS84 +units=m +no_defs +ellps=WGS84 +towgs84=0,0,0"

#-----------------------------------
# v-shaped valley
#----------------------------------
r1 <- raster()
crs(r1)<-prj
extent(r1)<-extent(0, 3000, 0, 6000)
res(r1)<-30
r1[] <- seq(from=10, to=1000, by=10)

r2<-raster()
crs(r2)<-prj
extent(r2)<-extent(-3000, 0, 0, 6000)
res(r2)<-30
r2[] <- seq(from=1000, to=10, by=-10)

m <- merge(r1, r2)

r3<-raster()
crs(r3)<-prj
extent(r3)<-extent(-3300, -3000, 0, 6000)
res(r3)<-30
r3[] <- 1000

r4<-raster()
crs(r4)<-prj
extent(r4)<-extent(3000, 3300, 0, 6000)
res(r4)<-30
r4[] <- 1000

m <- merge(m, r3, r4)

writeRaster(m, "vshaped_valley_notilt.tif", overwrite=TRUE)

s <- addTilt(m, 10, filename="vshaped_valley.tif")

#-----------------------------------
# v-shaped, flat-bottomed valley
#----------------------------------
r1 <- raster()
crs(r1)<-prj
extent(r1)<-extent(150, 3150, 0, 6000)
res(r1)<-30
r1[] <- seq(from=10, to=1000, by=10)

r2<-raster()
crs(r2)<-prj
extent(r2)<-extent(-3150, -150, 0, 6000)
res(r2)<-30
r2[] <- seq(from=1000, to=10, by=-10)

m <- merge(r1, r2)

r3<-raster()
crs(r3)<-prj
extent(r3)<-extent(-3450, -3150, 0, 6000)
res(r3)<-30
r3[] <- 1000

r4<-raster()
crs(r4)<-prj
extent(r4)<-extent(3150, 3450, 0, 6000)
res(r4)<-30
r4[] <- 1000

m <- merge(m, r3, r4)

r5<-raster()
crs(r5)<-prj
extent(r5)<-extent(-150, 150, 0, 6000)
res(r5)<-30
r5[] <- 10

m<-merge(m, r5)

writeRaster(m, "vshaped_valley_notilt_flatbottom.tif", overwrite=TRUE)

#1.7% slope
s <- addTilt(m, 0.5, filename="vshaped_valley_tilt_flatbottom.tif")

#====== functions =======================
addTilt <- function(x, a, filename) {
    out <- raster(x)
    out <- writeStart(out, filename, overwrite=TRUE)
    for (r in 1:nrow(out)) {
        v <- getValues(x, r)
        v <- v + a*r
        out <- writeValues(out, v, r)
    }
    out<-writeStop(out)
    return(out)
}
