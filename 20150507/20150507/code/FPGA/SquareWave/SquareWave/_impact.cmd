setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref ConcurrentMode:FALSE
setPreference -pref UseHighz:FALSE
setPreference -pref UserLevel:NOVICE
setPreference -pref svfUseTime:FALSE
loadProjectFile -file "C:/GP/code/fpga/FPGA/SquareWave/SquareWave/SquareWave.ipf"
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -pff
setMode -pff
setMode -pff
setMode -pff
setCurrentDesign -version 0
setMode -pff
setCurrentDeviceChain -index 0
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
setMode -bs
setMode -bs
setCable -port auto
Identify 
identifyMPM 
assignFile -p 2 -file "C:/GP/code/fpga/FPGA/SquareWave/SquareWave/SquareWave.mcs"
setAttribute -position 2 -attr packageName -value "(null)"
assignFile -p 2 -file "C:/GP/code/fpga/FPGA/SquareWave/SquareWave/SquareWave.mcs"
setAttribute -position 2 -attr packageName -value "(null)"
Program -p 2 -e -v -defaultVersion 0 
saveProjectFile -file "C:/GP/code/fpga/FPGA/SquareWave/SquareWave/SquareWave.ipf"
