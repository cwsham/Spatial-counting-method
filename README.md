# Spatial-counting-method
Density Effects on House Prices: A novel spatial counting method

Abstract
The negative effect of housing density is well recognised in urban studies; that is why urban planning puts much emphasis on development intensity control. However, very little is known about the actual perceived impacts of housing density. In planning research, density effects are usually studied qualitatively through surveys or interviews. However, the results are hard to make cross-city comparisons. The few quantitative studies considered housing densities of administrative area units, such as census traits or statistical area units, prone to boundary bias. This paper seeks to empirically identify the general housing density effect and public housing density effect on house prices based on a revealed preference approach and develops a novel spatial counting method of the number of houses at either 0.5 km, 1.0 km or 1.5 km radial circle of each home. Based on 18,000+ housing transactions in 2020 in Auckland, New Zealand, the results confirm the adverse density effects of both ordinary houses and public housing in a neighbourhood, ceteris paribus. In contrast, the density effects estimated using the statistical area units are found to be positive and insignificant, which shows the problem of using administrative area units in estimating neighbourhood effects. The reason for using neighbourhood data at administrative area units is the data availability from the census and the computing resources required to count the number of houses spatially. This study, therefore, develops a novel spatial counting method to sharply reduce the counting time, which can be applied generally in further studies of neighbourhood objects. The results may also have important implications for urban planners and government officials in formulating development intensity controls.

Test.c --- the main program file

residential.csv --- the property file in Auckland.

AirBnB.csv --- the AirBnB file in Auckland

Out_500_2022.txt --- Sample redirected result file.

Sample.xlsx --- Sample excel file imported from the result files

Compile: g++ -o test test.c

Command line: Test <option: 1 for 500m; 2 for 1000m; 3 for 1500m> <Year: 2003-2022> <1: state house; 2: AirBnB>

--- Example: Test 1 2022 1 > result.txt

--- Count the number of state houses at 0.5 km radial circle of each home that are built in or after 2022. The result.txt will store the outputs.
