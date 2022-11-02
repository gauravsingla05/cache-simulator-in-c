
******************* STEPS TO RUN THE PROGRAM *****************


1. Unzip the uplaoded file
2. run command "make clean"
3. run command "make all"
4. It should generate "cachesim.exe" file
5. run the following command with 4 arguments in same directory of ".exe  file"

./cachesim.exe <single-level or two-level cache> <direct> <trace file name> 

Single level Example  - ./cachesim.exe single-level direct ./trace_for_students/trace.hpcg

Two level Example -    ./cachesim.exe two-level direct  ./trace_for_students/trace.hpcg

6. Use "single-level" or "two-level" as 2nd argument for changing the memory levels

7. Use "direct" or "fully-associative" or "n-way" as 3rd argument for changing the mapping type

8. Change cache configuration in cachesim.h file



***************** RESULTS  ***************************


Please find "results" folder for all different trace file results.



***************** WORKING OF CODE ***************************


1. cachesim.c file is responsible for all the outputs and functions.
2. traces_for_students contain all the input files
3. main function in cachesim.c calls 3 functions a.) direct_mapped_cache_access() b.) nway_mapped_cache_access() c.) fully_associative_cache_access()
4. User have to provide 4 arguments to run the program
5. the user can change the configuration of cache in cachesim.h file

