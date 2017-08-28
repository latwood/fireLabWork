#command to run this program (includes the current file location)
#source('/home/latwood/src/fireLabWork/R/smokeTransportVisualization/editPictures.R')

###import libraries
library(png)
library(jpeg)
library(gtools)

###variables you can change


mainOriginalPictureDirectory <- "/home/latwood/Documents/smokeTransportVisualization/newPics"
mainNewPictureDirectory <- "/home/latwood/Documents/smokeTransportVisualization/picturesForVideos"

pictureDirectoryName <- "/SmokeTracer/vshapedValley_tilt_flatbottom39.71mRes/NativeSolver_DomainAvg/diurnal/6-15-2016_21hr00min_72F/rightValley/southView"

OriginalPictureDirectory <- paste(mainOriginalPictureDirectory,pictureDirectoryName,sep="")
NewPictureDirectory <- paste(mainNewPictureDirectory,pictureDirectoryName,sep="")

# numberOfPictures <- 277
# ignoreFirstPics <- 0
# ignoreLastPics <- 0
# timesForPics <- matrix(nrow=numberOfPictures)
# timesForPics[1] <- "04/11/16 at 12:30 hrs"    # 0
# timesForPics[2] <- "04/11/16 at 12:35 hrs"    # 351
# timesForPics[3] <- "04/11/16 at 12:40 hrs"    # 651
# timesForPics[4] <- "04/11/16 at 12:45 hrs"    # 951
# timesForPics[5] <- "04/11/16 at 12:50 hrs"    # 1251
# timesForPics[6] <- "04/11/16 at 12:55 hrs"    # 1551
# timesForPics[7] <- "04/11/16 at 13:00 hrs"    # 1851
# timesForPics[8] <- "04/11/16 at 13:05 hrs"    # 2151
# timesForPics[9] <- "04/11/16 at 13:10 hrs"    # 2451
# timesForPics[10] <- "04/11/16 at 13:15 hrs"    # 2751
# timesForPics[11] <- "04/11/16 at 13:20 hrs"    # 3051
# timesForPics[12] <- "04/11/16 at 13:25 hrs"    # 3351
# timesForPics[13] <- "04/11/16 at 13:30 hrs"    # 3651
# timesForPics[14] <- "04/11/16 at 13:35 hrs"    # 3951
# timesForPics[15] <- "04/11/16 at 13:40 hrs"    # 4251
# timesForPics[16] <- "04/11/16 at 13:45 hrs"    # 4551
# timesForPics[17] <- "04/11/16 at 13:50 hrs"    # 4851
# timesForPics[18] <- "04/11/16 at 13:55 hrs"    # 5151
# timesForPics[19] <- "04/11/16 at 14:00 hrs"    # 5451
# timesForPics[20] <- "04/11/16 at 14:05 hrs"    # 5751
# timesForPics[21] <- "04/11/16 at 14:10 hrs"    # 6051
# timesForPics[22] <- "04/11/16 at 14:15 hrs"    # 6351
# timesForPics[23] <- "04/11/16 at 14:20 hrs"    # 6651
# timesForPics[24] <- "04/11/16 at 14:25 hrs"    # 6951
# timesForPics[25] <- "04/11/16 at 14:30 hrs"    # 7251
# timesForPics[26] <- "04/11/16 at 14:35 hrs"    # 7551
# timesForPics[27] <- "04/11/16 at 14:40 hrs"    # 7851
# timesForPics[28] <- "04/11/16 at 14:45 hrs"    # 8151
# timesForPics[29] <- "04/11/16 at 14:50 hrs"    # 8451
# timesForPics[30] <- "04/11/16 at 14:55 hrs"    # 8751
# timesForPics[31] <- "04/11/16 at 15:00 hrs"    # 9051
# timesForPics[32] <- "04/11/16 at 15:05 hrs"    # 9351
# timesForPics[33] <- "04/11/16 at 15:10 hrs"    # 9651
# timesForPics[34] <- "04/11/16 at 15:15 hrs"    # 9951
# timesForPics[35] <- "04/11/16 at 15:20 hrs"    # 10251
# timesForPics[36] <- "04/11/16 at 15:25 hrs"    # 10551
# timesForPics[37] <- "04/11/16 at 15:30 hrs"    # 10851
# timesForPics[38] <- "04/11/16 at 15:35 hrs"    # 11151
# timesForPics[39] <- "04/11/16 at 15:40 hrs"    # 11451
# timesForPics[40] <- "04/11/16 at 15:45 hrs"    # 11751
# timesForPics[41] <- "04/11/16 at 15:50 hrs"    # 12051
# timesForPics[42] <- "04/11/16 at 15:55 hrs"    # 12351
# timesForPics[43] <- "04/11/16 at 16:00 hrs"    # 12651
# timesForPics[44] <- "04/11/16 at 16:05 hrs"    # 12951
# timesForPics[45] <- "04/11/16 at 16:10 hrs"    # 13251
# timesForPics[46] <- "04/11/16 at 16:15 hrs"    # 13551
# timesForPics[47] <- "04/11/16 at 16:20 hrs"    # 13851
# timesForPics[48] <- "04/11/16 at 16:25 hrs"    # 14151
# timesForPics[49] <- "04/11/16 at 16:30 hrs"    # 14451
# timesForPics[50] <- "04/11/16 at 16:35 hrs"    # 14751
# timesForPics[51] <- "04/11/16 at 16:40 hrs"    # 15051
# timesForPics[52] <- "04/11/16 at 16:45 hrs"    # 15351
# timesForPics[53] <- "04/11/16 at 16:50 hrs"    # 15651
# timesForPics[54] <- "04/11/16 at 16:55 hrs"    # 15951
# timesForPics[55] <- "04/11/16 at 17:00 hrs"    # 16251
# timesForPics[56] <- "04/11/16 at 17:05 hrs"    # 16551
# timesForPics[57] <- "04/11/16 at 17:10 hrs"    # 16851
# timesForPics[58] <- "04/11/16 at 17:15 hrs"    # 17151
# timesForPics[59] <- "04/11/16 at 17:20 hrs"    # 17451
# timesForPics[60] <- "04/11/16 at 17:25 hrs"    # 17751
# timesForPics[61] <- "04/11/16 at 17:30 hrs"    # 18051
# timesForPics[62] <- "04/11/16 at 17:35 hrs"    # 18351
# timesForPics[63] <- "04/11/16 at 17:40 hrs"    # 18651
# timesForPics[64] <- "04/11/16 at 17:45 hrs"    # 18951
# timesForPics[65] <- "04/11/16 at 17:50 hrs"    # 19251
# timesForPics[66] <- "04/11/16 at 17:55 hrs"    # 19551
# timesForPics[67] <- "04/11/16 at 18:00 hrs"    # 19851
# timesForPics[68] <- "04/11/16 at 18:05 hrs"    # 20151
# timesForPics[69] <- "04/11/16 at 18:10 hrs"    # 20451
# timesForPics[70] <- "04/11/16 at 18:15 hrs"    # 20751
# timesForPics[71] <- "04/11/16 at 18:20 hrs"    # 21051
# timesForPics[72] <- "04/11/16 at 18:25 hrs"    # 21351
# timesForPics[73] <- "04/11/16 at 18:30 hrs"    # 21651
# timesForPics[74] <- "04/11/16 at 18:35 hrs"    # 21951
# timesForPics[75] <- "04/11/16 at 18:40 hrs"    # 22251
# timesForPics[76] <- "04/11/16 at 18:45 hrs"    # 22551
# timesForPics[77] <- "04/11/16 at 18:50 hrs"    # 22851
# timesForPics[78] <- "04/11/16 at 18:55 hrs"    # 23151
# timesForPics[79] <- "04/11/16 at 19:00 hrs"    # 23451
# timesForPics[80] <- "04/11/16 at 19:05 hrs"    # 23751
# timesForPics[81] <- "04/11/16 at 19:10 hrs"    # 24051
# timesForPics[82] <- "04/11/16 at 19:15 hrs"    # 24351
# timesForPics[83] <- "04/11/16 at 19:20 hrs"    # 24651
# timesForPics[84] <- "04/11/16 at 19:25 hrs"    # 24951
# timesForPics[85] <- "04/11/16 at 19:30 hrs"    # 25251
# timesForPics[86] <- "04/11/16 at 19:35 hrs"    # 25551
# timesForPics[87] <- "04/11/16 at 19:40 hrs"    # 25851
# timesForPics[88] <- "04/11/16 at 19:45 hrs"    # 26151
# timesForPics[89] <- "04/11/16 at 19:50 hrs"    # 26451
# timesForPics[90] <- "04/11/16 at 19:55 hrs"    # 26751
# timesForPics[91] <- "04/11/16 at 20:00 hrs"    # 27051
# timesForPics[92] <- "04/11/16 at 20:05 hrs"    # 27351
# timesForPics[93] <- "04/11/16 at 20:10 hrs"    # 27651
# timesForPics[94] <- "04/11/16 at 20:15 hrs"    # 27951
# timesForPics[95] <- "04/11/16 at 20:20 hrs"    # 28251
# timesForPics[96] <- "04/11/16 at 20:25 hrs"    # 28551
# timesForPics[97] <- "04/11/16 at 20:30 hrs"    # 28851
# timesForPics[98] <- "04/11/16 at 20:35 hrs"    # 29151
# timesForPics[99] <- "04/11/16 at 20:40 hrs"    # 29451
# timesForPics[100] <- "04/11/16 at 20:45 hrs"    # 29751
# timesForPics[101] <- "04/11/16 at 20:50 hrs"    # 30051
# timesForPics[102] <- "04/11/16 at 20:55 hrs"    # 30351
# timesForPics[103] <- "04/11/16 at 21:00 hrs"    # 30651
# timesForPics[104] <- "04/11/16 at 21:05 hrs"    # 30951
# timesForPics[105] <- "04/11/16 at 21:10 hrs"    # 31251
# timesForPics[106] <- "04/11/16 at 21:15 hrs"    # 31551
# timesForPics[107] <- "04/11/16 at 21:20 hrs"    # 31851
# timesForPics[108] <- "04/11/16 at 21:25 hrs"    # 32151
# timesForPics[109] <- "04/11/16 at 21:30 hrs"    # 32451
# timesForPics[110] <- "04/11/16 at 21:35 hrs"    # 32751
# timesForPics[111] <- "04/11/16 at 21:40 hrs"    # 33051
# timesForPics[112] <- "04/11/16 at 21:45 hrs"    # 33351
# timesForPics[113] <- "04/11/16 at 21:50 hrs"    # 33651
# timesForPics[114] <- "04/11/16 at 21:55 hrs"    # 33951
# timesForPics[115] <- "04/11/16 at 22:00 hrs"    # 34251
# timesForPics[116] <- "04/11/16 at 22:05 hrs"    # 34551
# timesForPics[117] <- "04/11/16 at 22:10 hrs"    # 34851
# timesForPics[118] <- "04/11/16 at 22:15 hrs"    # 35151
# timesForPics[119] <- "04/11/16 at 22:20 hrs"    # 35451
# timesForPics[120] <- "04/11/16 at 22:25 hrs"    # 35751
# timesForPics[121] <- "04/11/16 at 22:30 hrs"    # 36051
# timesForPics[122] <- "04/11/16 at 22:35 hrs"    # 36351
# timesForPics[123] <- "04/11/16 at 22:40 hrs"    # 36651
# timesForPics[124] <- "04/11/16 at 22:45 hrs"    # 36951
# timesForPics[125] <- "04/11/16 at 22:50 hrs"    # 37251
# timesForPics[126] <- "04/11/16 at 22:55 hrs"    # 37551
# timesForPics[127] <- "04/11/16 at 23:00 hrs"    # 37851
# timesForPics[128] <- "04/11/16 at 23:05 hrs"    # 38151
# timesForPics[129] <- "04/11/16 at 23:10 hrs"    # 38451
# timesForPics[130] <- "04/11/16 at 23:15 hrs"    # 38751
# timesForPics[131] <- "04/11/16 at 23:20 hrs"    # 39051
# timesForPics[132] <- "04/11/16 at 23:25 hrs"    # 39351
# timesForPics[133] <- "04/11/16 at 23:30 hrs"    # 39651
# timesForPics[134] <- "04/11/16 at 23:35 hrs"    # 39951
# timesForPics[135] <- "04/11/16 at 23:40 hrs"    # 40251
# timesForPics[136] <- "04/11/16 at 23:45 hrs"    # 40551
# timesForPics[137] <- "04/11/16 at 23:50 hrs"    # 40851
# timesForPics[138] <- "04/11/16 at 23:55 hrs"    # 41151
# timesForPics[139] <- "04/12/16 at 00:00 hrs"    # 41451
# timesForPics[140] <- "04/12/16 at 00:05 hrs"    # 41751
# timesForPics[141] <- "04/12/16 at 00:10 hrs"    # 42051
# timesForPics[142] <- "04/12/16 at 00:15 hrs"    # 42351
# timesForPics[143] <- "04/12/16 at 00:20 hrs"    # 42651
# timesForPics[144] <- "04/12/16 at 00:25 hrs"    # 42951
# timesForPics[145] <- "04/12/16 at 00:30 hrs"    # 43251
# timesForPics[146] <- "04/12/16 at 00:35 hrs"    # 43551
# timesForPics[147] <- "04/12/16 at 00:40 hrs"    # 43851
# timesForPics[148] <- "04/12/16 at 00:45 hrs"    # 44151
# timesForPics[149] <- "04/12/16 at 00:50 hrs"    # 44451
# timesForPics[150] <- "04/12/16 at 00:55 hrs"    # 44751
# timesForPics[151] <- "04/12/16 at 01:00 hrs"    # 45051
# timesForPics[152] <- "04/12/16 at 01:05 hrs"    # 45351
# timesForPics[153] <- "04/12/16 at 01:10 hrs"    # 45651
# timesForPics[154] <- "04/12/16 at 01:15 hrs"    # 45951
# timesForPics[155] <- "04/12/16 at 01:20 hrs"    # 46251
# timesForPics[156] <- "04/12/16 at 01:25 hrs"    # 46551
# timesForPics[157] <- "04/12/16 at 01:30 hrs"    # 46851
# timesForPics[158] <- "04/12/16 at 01:35 hrs"    # 47151
# timesForPics[159] <- "04/12/16 at 01:40 hrs"    # 47451
# timesForPics[160] <- "04/12/16 at 01:45 hrs"    # 47751
# timesForPics[161] <- "04/12/16 at 01:50 hrs"    # 48051
# timesForPics[162] <- "04/12/16 at 01:55 hrs"    # 48351
# timesForPics[163] <- "04/12/16 at 02:00 hrs"    # 48651
# timesForPics[164] <- "04/12/16 at 02:05 hrs"    # 48951
# timesForPics[165] <- "04/12/16 at 02:10 hrs"    # 49251
# timesForPics[166] <- "04/12/16 at 02:15 hrs"    # 49551
# timesForPics[167] <- "04/12/16 at 02:20 hrs"    # 49851
# timesForPics[168] <- "04/12/16 at 02:25 hrs"    # 50151
# timesForPics[169] <- "04/12/16 at 02:30 hrs"    # 50451
# timesForPics[170] <- "04/12/16 at 02:35 hrs"    # 50751
# timesForPics[171] <- "04/12/16 at 02:40 hrs"    # 51051
# timesForPics[172] <- "04/12/16 at 02:45 hrs"    # 51351
# timesForPics[173] <- "04/12/16 at 02:50 hrs"    # 51651
# timesForPics[174] <- "04/12/16 at 02:55 hrs"    # 51951
# timesForPics[175] <- "04/12/16 at 03:00 hrs"    # 52251
# timesForPics[176] <- "04/12/16 at 03:05 hrs"    # 52551
# timesForPics[177] <- "04/12/16 at 03:10 hrs"    # 52851
# timesForPics[178] <- "04/12/16 at 03:15 hrs"    # 53151
# timesForPics[179] <- "04/12/16 at 03:20 hrs"    # 53451
# timesForPics[180] <- "04/12/16 at 03:25 hrs"    # 53751
# timesForPics[181] <- "04/12/16 at 03:30 hrs"    # 54051
# timesForPics[182] <- "04/12/16 at 03:35 hrs"    # 54351
# timesForPics[183] <- "04/12/16 at 03:40 hrs"    # 54651
# timesForPics[184] <- "04/12/16 at 03:45 hrs"    # 54951
# timesForPics[185] <- "04/12/16 at 03:50 hrs"    # 55251
# timesForPics[186] <- "04/12/16 at 03:55 hrs"    # 55551
# timesForPics[187] <- "04/12/16 at 04:00 hrs"    # 55851
# timesForPics[188] <- "04/12/16 at 04:05 hrs"    # 56151
# timesForPics[189] <- "04/12/16 at 04:10 hrs"    # 56451
# timesForPics[190] <- "04/12/16 at 04:15 hrs"    # 56751
# timesForPics[191] <- "04/12/16 at 04:20 hrs"    # 57051
# timesForPics[192] <- "04/12/16 at 04:25 hrs"    # 57351
# timesForPics[193] <- "04/12/16 at 04:30 hrs"    # 57651
# timesForPics[194] <- "04/12/16 at 04:35 hrs"    # 57951
# timesForPics[195] <- "04/12/16 at 04:40 hrs"    # 58251
# timesForPics[196] <- "04/12/16 at 04:45 hrs"    # 58551
# timesForPics[197] <- "04/12/16 at 04:50 hrs"    # 58851
# timesForPics[198] <- "04/12/16 at 04:55 hrs"    # 59151
# timesForPics[199] <- "04/12/16 at 05:00 hrs"    # 59451
# timesForPics[200] <- "04/12/16 at 05:05 hrs"    # 59751
# timesForPics[201] <- "04/12/16 at 05:10 hrs"    # 60051
# timesForPics[202] <- "04/12/16 at 05:15 hrs"    # 60351
# timesForPics[203] <- "04/12/16 at 05:20 hrs"    # 60651
# timesForPics[204] <- "04/12/16 at 05:25 hrs"    # 60951
# timesForPics[205] <- "04/12/16 at 05:30 hrs"    # 61251
# timesForPics[206] <- "04/12/16 at 05:35 hrs"    # 61551
# timesForPics[207] <- "04/12/16 at 05:40 hrs"    # 61851
# timesForPics[208] <- "04/12/16 at 05:45 hrs"    # 62151
# timesForPics[209] <- "04/12/16 at 05:50 hrs"    # 62451
# timesForPics[210] <- "04/12/16 at 05:55 hrs"    # 62751
# timesForPics[211] <- "04/12/16 at 06:00 hrs"    # 63051
# timesForPics[212] <- "04/12/16 at 06:05 hrs"    # 63351
# timesForPics[213] <- "04/12/16 at 06:10 hrs"    # 63651
# timesForPics[214] <- "04/12/16 at 06:15 hrs"    # 63951
# timesForPics[215] <- "04/12/16 at 06:20 hrs"    # 64251
# timesForPics[216] <- "04/12/16 at 06:25 hrs"    # 64551
# timesForPics[217] <- "04/12/16 at 06:30 hrs"    # 64851
# timesForPics[218] <- "04/12/16 at 06:35 hrs"    # 65151
# timesForPics[219] <- "04/12/16 at 06:40 hrs"    # 65451
# timesForPics[220] <- "04/12/16 at 06:45 hrs"    # 65751
# timesForPics[221] <- "04/12/16 at 06:50 hrs"    # 66051
# timesForPics[222] <- "04/12/16 at 06:55 hrs"    # 66351
# timesForPics[223] <- "04/12/16 at 07:00 hrs"    # 66651
# timesForPics[224] <- "04/12/16 at 07:05 hrs"    # 66951
# timesForPics[225] <- "04/12/16 at 07:10 hrs"    # 67251
# timesForPics[226] <- "04/12/16 at 07:15 hrs"    # 67551
# timesForPics[227] <- "04/12/16 at 07:20 hrs"    # 67851
# timesForPics[228] <- "04/12/16 at 07:25 hrs"    # 68151
# timesForPics[229] <- "04/12/16 at 07:30 hrs"    # 68451
# timesForPics[230] <- "04/12/16 at 07:35 hrs"    # 68751
# timesForPics[231] <- "04/12/16 at 07:40 hrs"    # 69051
# timesForPics[232] <- "04/12/16 at 07:45 hrs"    # 69351
# timesForPics[233] <- "04/12/16 at 07:50 hrs"    # 69651
# timesForPics[234] <- "04/12/16 at 07:55 hrs"    # 69951
# timesForPics[235] <- "04/12/16 at 08:00 hrs"    # 70251
# timesForPics[236] <- "04/12/16 at 08:05 hrs"    # 70551
# timesForPics[237] <- "04/12/16 at 08:10 hrs"    # 70851
# timesForPics[238] <- "04/12/16 at 08:15 hrs"    # 71151
# timesForPics[239] <- "04/12/16 at 08:20 hrs"    # 71451
# timesForPics[240] <- "04/12/16 at 08:25 hrs"    # 71751
# timesForPics[241] <- "04/12/16 at 08:30 hrs"    # 72051
# timesForPics[242] <- "04/12/16 at 08:35 hrs"    # 72351
# timesForPics[243] <- "04/12/16 at 08:40 hrs"    # 72651
# timesForPics[244] <- "04/12/16 at 08:45 hrs"    # 72951
# timesForPics[245] <- "04/12/16 at 08:50 hrs"    # 73251
# timesForPics[246] <- "04/12/16 at 08:55 hrs"    # 73551
# timesForPics[247] <- "04/12/16 at 09:00 hrs"    # 73851
# timesForPics[248] <- "04/12/16 at 09:05 hrs"    # 74151
# timesForPics[249] <- "04/12/16 at 09:10 hrs"    # 74451
# timesForPics[250] <- "04/12/16 at 09:15 hrs"    # 74751
# timesForPics[251] <- "04/12/16 at 09:20 hrs"    # 75051
# timesForPics[252] <- "04/12/16 at 09:25 hrs"    # 75351
# timesForPics[253] <- "04/12/16 at 09:30 hrs"    # 75651
# timesForPics[254] <- "04/12/16 at 09:35 hrs"    # 75951
# timesForPics[255] <- "04/12/16 at 09:40 hrs"    # 76251
# timesForPics[256] <- "04/12/16 at 09:45 hrs"    # 76551
# timesForPics[257] <- "04/12/16 at 09:50 hrs"    # 76851
# timesForPics[258] <- "04/12/16 at 09:55 hrs"    # 77151
# timesForPics[259] <- "04/12/16 at 10:00 hrs"    # 77451
# timesForPics[260] <- "04/12/16 at 10:05 hrs"    # 77751
# timesForPics[261] <- "04/12/16 at 10:10 hrs"    # 78051
# timesForPics[262] <- "04/12/16 at 10:15 hrs"    # 78351
# timesForPics[263] <- "04/12/16 at 10:20 hrs"    # 78651
# timesForPics[264] <- "04/12/16 at 10:25 hrs"    # 78951
# timesForPics[265] <- "04/12/16 at 10:30 hrs"    # 79251
# timesForPics[266] <- "04/12/16 at 10:35 hrs"    # 79551
# timesForPics[267] <- "04/12/16 at 10:40 hrs"    # 79851
# timesForPics[268] <- "04/12/16 at 10:45 hrs"    # 80151
# timesForPics[269] <- "04/12/16 at 10:50 hrs"    # 80451
# timesForPics[270] <- "04/12/16 at 10:55 hrs"    # 80751
# timesForPics[271] <- "04/12/16 at 11:00 hrs"    # 81051
# timesForPics[272] <- "04/12/16 at 11:05 hrs"    # 81351
# timesForPics[273] <- "04/12/16 at 11:10 hrs"    # 81651
# timesForPics[274] <- "04/12/16 at 11:15 hrs"    # 81951
# timesForPics[275] <- "04/12/16 at 11:20 hrs"    # 82251
# timesForPics[276] <- "04/12/16 at 11:25 hrs"    # 82551
# timesForPics[277] <- "04/12/16 at 11:30 hrs"    # 82851

# numberOfPictures <- 82
# ignoreFirstPics <- 3
# ignoreLastPics <- 0
# timesForPics <- matrix(nrow=numberOfPictures)
# timesForPics[1] <- "04/11/16 at 12:30 hrs"    # 0
# timesForPics[2] <- "04/11/16 at 12:30 hrs"    # 50
# timesForPics[3] <- "04/11/16 at 12:30 hrs"    # 51
# timesForPics[4] <- "04/11/16 at 12:30 hrs"    # 351
# timesForPics[5] <- "04/11/16 at 12:35 hrs"    # 651
# timesForPics[6] <- "04/11/16 at 12:40 hrs"    # 951
# timesForPics[7] <- "04/11/16 at 12:45 hrs"    # 1251
# timesForPics[8] <- "04/11/16 at 12:50 hrs"    # 1551
# timesForPics[9] <- "04/11/16 at 12:55 hrs"    # 1851
# timesForPics[10] <- "04/11/16 at 13:00 hrs"    # 2151
# timesForPics[11] <- "04/11/16 at 13:05 hrs"    # 2451
# timesForPics[12] <- "04/11/16 at 13:10 hrs"    # 2751
# timesForPics[13] <- "04/11/16 at 13:15 hrs"    # 3051
# timesForPics[14] <- "04/11/16 at 13:20 hrs"    # 3351
# timesForPics[15] <- "04/11/16 at 13:25 hrs"    # 3651
# timesForPics[16] <- "04/11/16 at 13:30 hrs"    # 3951
# timesForPics[17] <- "04/11/16 at 13:45 hrs"    # 4851
# timesForPics[18] <- "04/11/16 at 14:00 hrs"    # 5751
# timesForPics[19] <- "04/11/16 at 14:15 hrs"    # 6651
# timesForPics[20] <- "04/11/16 at 14:30 hrs"    # 7551
# timesForPics[21] <- "04/11/16 at 14:45 hrs"    # 8451
# timesForPics[22] <- "04/11/16 at 15:00 hrs"    # 9351
# timesForPics[23] <- "04/11/16 at 15:15 hrs"    # 10251
# timesForPics[24] <- "04/11/16 at 15:30 hrs"    # 11151
# timesForPics[25] <- "04/11/16 at 15:45 hrs"    # 12051
# timesForPics[26] <- "04/11/16 at 16:00 hrs"    # 12951
# timesForPics[27] <- "04/11/16 at 16:15 hrs"    # 13851
# timesForPics[28] <- "04/11/16 at 16:30 hrs"    # 14751
# timesForPics[29] <- "04/11/16 at 16:45 hrs"    # 15651
# timesForPics[30] <- "04/11/16 at 17:00 hrs"    # 16551
# timesForPics[31] <- "04/11/16 at 17:15 hrs"    # 17451
# timesForPics[32] <- "04/11/16 at 17:30 hrs"    # 18351
# timesForPics[33] <- "04/11/16 at 17:45 hrs"    # 19251
# timesForPics[34] <- "04/11/16 at 18:00 hrs"    # 20151
# timesForPics[35] <- "04/11/16 at 18:15 hrs"    # 21051
# timesForPics[36] <- "04/11/16 at 18:30 hrs"    # 21951
# timesForPics[37] <- "04/11/16 at 18:45 hrs"    # 22851
# timesForPics[38] <- "04/11/16 at 19:00 hrs"    # 23751
# timesForPics[39] <- "04/11/16 at 19:15 hrs"    # 24651
# timesForPics[40] <- "04/11/16 at 19:30 hrs"    # 25551
# timesForPics[41] <- "04/11/16 at 19:45 hrs"    # 26451
# timesForPics[42] <- "04/11/16 at 20:00 hrs"    # 27351
# timesForPics[43] <- "04/11/16 at 20:15 hrs"    # 28251
# timesForPics[44] <- "04/11/16 at 20:30 hrs"    # 29151
# timesForPics[45] <- "04/11/16 at 20:45 hrs"    # 30051
# timesForPics[46] <- "04/11/16 at 21:00 hrs"    # 30951
# timesForPics[47] <- "04/11/16 at 21:05 hrs"    # 31251
# timesForPics[48] <- "04/11/16 at 21:10 hrs"    # 31551
# timesForPics[49] <- "04/11/16 at 21:15 hrs"    # 31851
# timesForPics[50] <- "04/11/16 at 21:20 hrs"    # 32151
# timesForPics[51] <- "04/11/16 at 21:25 hrs"    # 32451
# timesForPics[52] <- "04/11/16 at 21:30 hrs"    # 32751
# timesForPics[53] <- "04/11/16 at 21:35 hrs"    # 33051
# timesForPics[54] <- "04/11/16 at 21:40 hrs"    # 33351
# timesForPics[55] <- "04/11/16 at 21:45 hrs"    # 33651
# timesForPics[56] <- "04/11/16 at 21:50 hrs"    # 33951
# timesForPics[57] <- "04/11/16 at 21:55 hrs"    # 34251
# timesForPics[58] <- "04/11/16 at 22:00 hrs"    # 34551
# timesForPics[59] <- "04/11/16 at 22:15 hrs"    # 35451
# timesForPics[60] <- "04/11/16 at 22:30 hrs"    # 36351
# timesForPics[61] <- "04/11/16 at 22:45 hrs"    # 37251
# timesForPics[62] <- "04/11/16 at 23:00 hrs"    # 38151
# timesForPics[63] <- "04/11/16 at 23:15 hrs"    # 39051
# timesForPics[64] <- "04/11/16 at 23:30 hrs"    # 39951
# timesForPics[65] <- "04/11/16 at 23:45 hrs"    # 40851
# timesForPics[66] <- "04/11/16 at 24:00 hrs"    # 41751
# timesForPics[67] <- "04/12/16 at 00:15 hrs"    # 42651
# timesForPics[68] <- "04/12/16 at 00:30 hrs"    # 43551
# timesForPics[69] <- "04/12/16 at 00:45 hrs"    # 44451
# timesForPics[70] <- "04/12/16 at 01:00 hrs"    # 45351
# timesForPics[71] <- "04/12/16 at 01:15 hrs"    # 46251
# timesForPics[72] <- "04/12/16 at 01:30 hrs"    # 47151
# timesForPics[73] <- "04/12/16 at 01:45 hrs"    # 48051
# timesForPics[74] <- "04/12/16 at 02:00 hrs"    # 48951
# timesForPics[75] <- "04/12/16 at 02:15 hrs"    # 49851
# timesForPics[76] <- "04/12/16 at 02:30 hrs"    # 50751
# timesForPics[77] <- "04/12/16 at 02:45 hrs"    # 51651
# timesForPics[78] <- "04/12/16 at 03:00 hrs"    # 52551
# timesForPics[79] <- "04/12/16 at 03:15 hrs"    # 53451
# timesForPics[80] <- "04/12/16 at 03:30 hrs"    # 54351
# timesForPics[81] <- "04/12/16 at 03:45 hrs"    # 55251
# timesForPics[82] <- "04/12/16 at 04:00 hrs"    # 56151

# numberOfPictures <- 75
# ignoreFirstPics <- 0
# ignoreLastPics <- 0
# timesForPics <- matrix(nrow=numberOfPictures)
# timesForPics[1] <- "00:00:00 hrs:min:sec"    # 0
# timesForPics[2] <- "00:01:00 hrs:min:sec"    # 60
# timesForPics[3] <- "00:02:00 hrs:min:sec"    # 120
# timesForPics[4] <- "00:03:00 hrs:min:sec"    # 180
# timesForPics[5] <- "00:04:00 hrs:min:sec"    # 240
# timesForPics[6] <- "00:05:00 hrs:min:sec"    # 300
# timesForPics[7] <- "00:06:00 hrs:min:sec"    # 360
# timesForPics[8] <- "00:07:00 hrs:min:sec"    # 420
# timesForPics[9] <- "00:08:00 hrs:min:sec"    # 480
# timesForPics[10] <- "00:09:00 hrs:min:sec"    # 540
# timesForPics[11] <- "00:10:00 hrs:min:sec"    # 600
# timesForPics[12] <- "00:11:00 hrs:min:sec"    # 660
# timesForPics[13] <- "00:12:00 hrs:min:sec"    # 720
# timesForPics[14] <- "00:13:00 hrs:min:sec"    # 780
# timesForPics[15] <- "00:14:00 hrs:min:sec"    # 840
# timesForPics[16] <- "00:15:00 hrs:min:sec"    # 900
# timesForPics[17] <- "00:16:00 hrs:min:sec"    # 960
# timesForPics[18] <- "00:17:00 hrs:min:sec"    # 1020
# timesForPics[19] <- "00:18:00 hrs:min:sec"    # 1080
# timesForPics[20] <- "00:19:00 hrs:min:sec"    # 1140
# timesForPics[21] <- "00:20:00 hrs:min:sec"    # 1200
# timesForPics[22] <- "00:21:00 hrs:min:sec"    # 1260
# timesForPics[23] <- "00:22:00 hrs:min:sec"    # 1320
# timesForPics[24] <- "00:23:00 hrs:min:sec"    # 1380
# timesForPics[25] <- "00:24:00 hrs:min:sec"    # 1440
# timesForPics[26] <- "00:25:00 hrs:min:sec"    # 1500
# timesForPics[27] <- "00:26:00 hrs:min:sec"    # 1560
# timesForPics[28] <- "00:27:00 hrs:min:sec"    # 1620
# timesForPics[29] <- "00:28:00 hrs:min:sec"    # 1680
# timesForPics[30] <- "00:29:00 hrs:min:sec"    # 1740
# timesForPics[31] <- "00:30:00 hrs:min:sec"    # 1800
# timesForPics[32] <- "00:31:00 hrs:min:sec"    # 1860
# timesForPics[33] <- "00:32:00 hrs:min:sec"    # 1920
# timesForPics[34] <- "00:33:00 hrs:min:sec"    # 1980
# timesForPics[35] <- "00:34:00 hrs:min:sec"    # 2040
# timesForPics[36] <- "00:35:00 hrs:min:sec"    # 2100
# timesForPics[37] <- "00:36:00 hrs:min:sec"    # 2160
# timesForPics[38] <- "00:37:00 hrs:min:sec"    # 2220
# timesForPics[39] <- "00:38:00 hrs:min:sec"    # 2280
# timesForPics[40] <- "00:39:00 hrs:min:sec"    # 2340
# timesForPics[41] <- "00:40:00 hrs:min:sec"    # 2400
# timesForPics[42] <- "00:41:00 hrs:min:sec"    # 2460
# timesForPics[43] <- "00:42:00 hrs:min:sec"    # 2520
# timesForPics[44] <- "00:43:00 hrs:min:sec"    # 2580
# timesForPics[45] <- "00:44:00 hrs:min:sec"    # 2640
# timesForPics[46] <- "00:45:00 hrs:min:sec"    # 2700
# timesForPics[47] <- "00:46:00 hrs:min:sec"    # 2760
# timesForPics[48] <- "00:47:00 hrs:min:sec"    # 2820
# timesForPics[49] <- "00:48:00 hrs:min:sec"    # 2880
# timesForPics[50] <- "00:49:00 hrs:min:sec"    # 2940
# timesForPics[51] <- "00:50:00 hrs:min:sec"    # 3000
# timesForPics[52] <- "00:51:00 hrs:min:sec"    # 3060
# timesForPics[53] <- "00:52:00 hrs:min:sec"    # 3120
# timesForPics[54] <- "00:53:00 hrs:min:sec"    # 3180
# timesForPics[55] <- "00:54:00 hrs:min:sec"    # 3240
# timesForPics[56] <- "00:55:00 hrs:min:sec"    # 3300
# timesForPics[57] <- "00:56:00 hrs:min:sec"    # 3360
# timesForPics[58] <- "00:57:00 hrs:min:sec"    # 3420
# timesForPics[59] <- "00:58:00 hrs:min:sec"    # 3480
# timesForPics[60] <- "00:59:00 hrs:min:sec"    # 3540
# timesForPics[61] <- "01:00:00 hrs:min:sec"    # 3600
# timesForPics[62] <- "01:15:00 hrs:min:sec"    # 4500
# timesForPics[63] <- "01:30:00 hrs:min:sec"    # 5400
# timesForPics[64] <- "01:45:00 hrs:min:sec"    # 6300
# timesForPics[65] <- "02:00:00 hrs:min:sec"    # 7200
# timesForPics[66] <- "02:15:00 hrs:min:sec"    # 8100
# timesForPics[67] <- "02:30:00 hrs:min:sec"    # 9000
# timesForPics[68] <- "02:45:00 hrs:min:sec"    # 9900
# timesForPics[69] <- "03:00:00 hrs:min:sec"    # 10800
# timesForPics[70] <- "03:15:00 hrs:min:sec"    # 11700
# timesForPics[71] <- "03:30:00 hrs:min:sec"    # 12600
# timesForPics[72] <- "03:45:00 hrs:min:sec"    # 13500
# timesForPics[73] <- "04:00:00 hrs:min:sec"    # 14400
# timesForPics[74] <- "04:15:00 hrs:min:sec"    # 15300
# timesForPics[75] <- "04:30:00 hrs:min:sec"    # 16200

numberOfPictures <- 55
ignoreFirstPics <- 0
ignoreLastPics <- 0
timesForPics <- matrix(nrow=numberOfPictures)
timesForPics[1] <- "00:00:00 hrs:min:sec"    # 0
timesForPics[2] <- "00:05:00 hrs:min:sec"    # 300
timesForPics[3] <- "00:10:00 hrs:min:sec"    # 600
timesForPics[4] <- "00:15:00 hrs:min:sec"    # 900
timesForPics[5] <- "00:20:00 hrs:min:sec"    # 1200
timesForPics[6] <- "00:25:00 hrs:min:sec"    # 1500
timesForPics[7] <- "00:30:00 hrs:min:sec"    # 1800
timesForPics[8] <- "00:35:00 hrs:min:sec"    # 2100
timesForPics[9] <- "00:40:00 hrs:min:sec"    # 2400
timesForPics[10] <- "00:45:00 hrs:min:sec"    # 2700
timesForPics[11] <- "00:50:00 hrs:min:sec"    # 3000
timesForPics[12] <- "00:55:00 hrs:min:sec"    # 3300
timesForPics[13] <- "01:00:00 hrs:min:sec"    # 3600
timesForPics[14] <- "01:05:00 hrs:min:sec"    # 3900
timesForPics[15] <- "01:10:00 hrs:min:sec"    # 4200
timesForPics[16] <- "01:15:00 hrs:min:sec"    # 4500
timesForPics[17] <- "01:20:00 hrs:min:sec"    # 4800
timesForPics[18] <- "01:25:00 hrs:min:sec"    # 5100
timesForPics[19] <- "01:30:00 hrs:min:sec"    # 5400
timesForPics[20] <- "01:35:00 hrs:min:sec"    # 5700
timesForPics[21] <- "01:40:00 hrs:min:sec"    # 6000
timesForPics[22] <- "01:45:00 hrs:min:sec"    # 6300
timesForPics[23] <- "01:50:00 hrs:min:sec"    # 6600
timesForPics[24] <- "01:55:00 hrs:min:sec"    # 6900
timesForPics[25] <- "02:00:00 hrs:min:sec"    # 7200
timesForPics[26] <- "02:05:00 hrs:min:sec"    # 7500
timesForPics[27] <- "02:10:00 hrs:min:sec"    # 7800
timesForPics[28] <- "02:15:00 hrs:min:sec"    # 8100
timesForPics[29] <- "02:20:00 hrs:min:sec"    # 8400
timesForPics[30] <- "02:25:00 hrs:min:sec"    # 8700
timesForPics[31] <- "02:30:00 hrs:min:sec"    # 9000
timesForPics[32] <- "02:35:00 hrs:min:sec"    # 9300
timesForPics[33] <- "02:40:00 hrs:min:sec"    # 9600
timesForPics[34] <- "02:45:00 hrs:min:sec"    # 9900
timesForPics[35] <- "02:50:00 hrs:min:sec"    # 10200
timesForPics[36] <- "02:55:00 hrs:min:sec"    # 10500
timesForPics[37] <- "03:00:00 hrs:min:sec"    # 10800
timesForPics[38] <- "03:05:00 hrs:min:sec"    # 11100
timesForPics[39] <- "03:10:00 hrs:min:sec"    # 11400
timesForPics[40] <- "03:15:00 hrs:min:sec"    # 11700
timesForPics[41] <- "03:20:00 hrs:min:sec"    # 12000
timesForPics[42] <- "03:25:00 hrs:min:sec"    # 12300
timesForPics[43] <- "03:30:00 hrs:min:sec"    # 12600
timesForPics[44] <- "03:35:00 hrs:min:sec"    # 12900
timesForPics[45] <- "03:40:00 hrs:min:sec"    # 13200
timesForPics[46] <- "03:45:00 hrs:min:sec"    # 13500
timesForPics[47] <- "03:50:00 hrs:min:sec"    # 13800
timesForPics[48] <- "03:55:00 hrs:min:sec"    # 14100
timesForPics[49] <- "04:00:00 hrs:min:sec"    # 14400
timesForPics[50] <- "04:05:00 hrs:min:sec"    # 14700
timesForPics[51] <- "04:10:00 hrs:min:sec"    # 15000
timesForPics[52] <- "04:15:00 hrs:min:sec"    # 15300
timesForPics[53] <- "04:20:00 hrs:min:sec"    # 15600
timesForPics[54] <- "04:25:00 hrs:min:sec"    # 15900
timesForPics[55] <- "04:30:00 hrs:min:sec"    # 16200

#delete the new file directory if it exists
if(file.exists(NewPictureDirectory))
{
  ##set up a, do you want to remove the directory? warning with y,n. Probably use stop() to end the program, or just continue on
  #probably just continue on
  #or just accept as is. Will always be checking to look for strange behavior till sure
  unlink(NewPictureDirectory,recursive=TRUE)
}
#make a new file directory for the copies of the pictures
dir.create(NewPictureDirectory,recursive=TRUE)


# ########## for jpg  ##############
# #copy pictures and sort them so they can be opened and changed in the right order
# pics <- list.files(OriginalPictureDirectory,full.names=TRUE,recursive=FALSE,pattern="jpg")
# file.copy(pics,NewPictureDirectory)
# pics <- list.files(NewPictureDirectory,full.names=TRUE,recursive=FALSE,pattern="jpg")
# pics <- mixedsort(pics)
# 
# #open files and remake them, adding in the text that is the time
# for(i in 1:length(pics))
# {
#   #read file
#   img<-readJPEG(pics[i])
#   #get size
#   h<-dim(img)[1]
#   w<-dim(img)[2]
# 
#   #open new file for output
#   jpeg(pics[i], width=w, height=h)
#   par(mar=c(0,0,0,0), xpd=NA, mgp=c(0,0,0), oma=c(0,0,0,0), ann=F)
#   plot.new()
#   plot.window(0:1, 0:1)
#   #fill plot with image
#   usr<-par("usr")
#   rasterImage(img, usr[1], usr[3], usr[2], usr[4])
#   #add text
#   text(.5,.925,timesForPics[i],cex=3,col="white")
#   #close image
#   dev.off()
# }
# 
# #rename files for video stuff
# file.rename(pics,paste(NewPictureDirectory,"/",0:(numberOfPictures-1),".jpg",sep=""))
# pics <- list.files(NewPictureDirectory,full.names=TRUE,recursive=FALSE,pattern="jpg")
# pics <- mixedsort(pics)


########## for png  ##############
#copy pictures and sort them so they can be opened and changed in the right order
pics <- list.files(OriginalPictureDirectory,full.names=TRUE,recursive=FALSE,pattern="png")
pics <- mixedsort(pics)
file.copy(pics[(1+ignoreFirstPics):(numberOfPictures-ignoreLastPics)],NewPictureDirectory)
pics <- list.files(NewPictureDirectory,full.names=TRUE,recursive=FALSE,pattern="png")
pics <- mixedsort(pics)

#open files and remake them, adding in the text that is the time
for(i in 1:length(pics))
{
  title <- "Tilt_FlatBot_RightValley, DT = 1, source = 75"
  centeringValue <- 0.5
  textRelativeHeight <- 0.6
  cropLeftValue <- 100
  cropTopValue <- 525
  cropRightValue <- 100
  cropBotValue <- 425
  
  #read file
  img<-readPNG(pics[i])
  #get size
  h<-dim(img)[1]
  w<-dim(img)[2]

  #open new file for output
  png(pics[i], width=w, height=h)
  par(mar=c(0,0,0,0), xpd=NA, mgp=c(0,0,0), oma=c(0,0,0,0), ann=F)
  plot.new()
  plot.window(0:1, 0:1)
  #fill plot with image
  usr<-par("usr")
  rasterImage(img, usr[1], usr[3], usr[2], usr[4])
  #add text
  text(centeringValue,textRelativeHeight,timesForPics[i+ignoreFirstPics],cex=3,col="white")
  text(centeringValue,textRelativeHeight+0.035,title,cex=4,col="white")
  #close image
  dev.off()
  #crop image if desired
  system(paste("mogrify -chop ",cropLeftValue,"x0+0+0 -gravity West ",pics[i],sep=""))
  system(paste("mogrify -chop 0x",cropTopValue,"+0+0 -gravity North ",pics[i],sep=""))
  system(paste("mogrify -chop ",cropRightValue,"x0+0+0 -gravity East ",pics[i],sep=""))
  system(paste("mogrify -chop 0x",cropBotValue,"+0+0 -gravity South ",pics[i],sep=""))
}

#rename files for video stuff
file.rename(pics,paste(NewPictureDirectory,"/",0:(numberOfPictures-1-ignoreFirstPics-ignoreLastPics),".png",sep=""))
pics <- list.files(NewPictureDirectory,full.names=TRUE,recursive=FALSE,pattern="png")
pics <- mixedsort(pics)

finalPictureArea <- paste(w-cropLeftValue-cropRightValue,"x",h-cropTopValue-cropBotValue,sep="")

sink(paste(OriginalPictureDirectory,"/lastCropInfo.txt",sep=""),append=FALSE)
cat(paste("ignoreFirstPic = \"",ignoreFirstPics,"\"\n",sep=""))
cat(paste("ignoreLastPic = \"",ignoreLastPics,"\"\n",sep=""))
cat(paste("title = \"",title,"\"\n",sep=""))
cat(paste("centeringValue = \"",centeringValue,"\"\n",sep=""))
cat(paste("textRelativeHeight = \"",textRelativeHeight,"\"\n",sep=""))
cat(paste("cropLeftValue = \"",cropLeftValue,"\"\n",sep=""))
cat(paste("cropTopValue = \"",cropTopValue,"\"\n",sep=""))
cat(paste("cropRightValue = \"",cropRightValue,"\"\n",sep=""))
cat(paste("cropBotValue = \"",cropBotValue,"\"\n",sep=""))
cat(paste("final picture area = \"",finalPictureArea,"\"\n",sep=""))
sink()
