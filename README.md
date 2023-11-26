# RWXFinder
Find Windows RWX Memory Regions depending on the memory space needed 

The function receives two parameters, the process handler and the space needed. 

With the VirtualQueryEx function we check all the memory spaces of the process and finally if the funcion found a Read Execute Write Memory Region, return the memory address (LPVOID)

