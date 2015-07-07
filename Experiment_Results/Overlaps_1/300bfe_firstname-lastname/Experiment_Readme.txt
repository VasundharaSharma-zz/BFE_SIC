
c++ Files needed for execution on SHARCNET
------------------------------------------
bfe.cpp
sha1.cpp
sharcbfe.cpp

bfe.h
sha1.h

makefile (included in the test package)
------------------------------------------


Input Text Files needed in Experiment:
--------------------------------------

bfe_list.txt (contains names which will be converted to bfe list at runtime, this file has passed permitted overlap check ; hence do not alter bfe.h during the course of experiment execution)
name_list.txt (contains all names in the database, considered for this specific experiment)
dice_name.txt (contains plain text names comparison database for dice co-eff matching for each of the names in nam_list.txt)


Execution command
-----------------

./testbfe <i>    

Note: Start i=0
for eg: i = 0-299 ; for this experiment since bfe_list.txt has 300 entries 