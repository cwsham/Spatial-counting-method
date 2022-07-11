# Spatial-counting-method
Density Effects on House Prices: A novel spatial counting method

Test.c --- the main program file
residential.zip --- the data file in New Zealand. It should be upzipped to be used.

Compile: g++ -o test test.c

Command line: Test <option: 1 for 500m; 2 for 1000m; 3 for 1500m> <Year: 2003-2022> 
--- Example: Test 1 2022 > result.txt
--- Count the number of houses at 0.5 km radial circle of each home that are built in or after 2022. The result.txt will store the outputs.
