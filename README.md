# RWXFinder
Find Windows RWX Memory Regions depending on the memory space needed 

The function receives two parameters, the process handler and the space needed. 

![image](https://github.com/S12cybersecurity/RWXFinder/assets/79543461/0e78669a-0550-4e45-b99c-df3ed380cce0)

With the VirtualQueryEx function we check all the memory spaces of the process and finally if the funcion found a Read Execute Write Memory Region, return the memory address (LPVOID)

