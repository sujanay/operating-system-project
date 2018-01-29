# operating system project

**GOAL:** Develop a program that checks the integrity of an ext2 filesystem, 
reporting any errors found and possibly correcting the errors.

This program examines a VDI (Virtual Disk Image) file containing a virtual disk with a single 
partition formatted as an ext2 filesystem. The filename of the file to be examined is given on 
the command line.
      
This program displays the following general statistics about the filesystem:
1. Total filesystem size in bytes
2. Size available for files (used and unused)
3. Amount of space currently used
3. Number of possible files and directories (number of inodes)
4. Number of existing files
5. Number of existing directories
6. Number of block groups, with the information for each block group
7. Block size in bytes
8. State of the filesystem

This program also check the following items:
- The appropriate magic number of the superblock
- All copies of the superblock must be consistent (spoiler alert)
- All copies of the block group table must be consistent

This program works fine for 1KB, 2KB and 4KB block sizes.

# Organization of the project

# Introduction






# References
- [Link 1:](https://www.gnu.org/software/libc/manual/html_node/Opening-and-Closing-Files.html#Opening-and-Closing-Files)
- [Virtualbox Forum](https://forums.virtualbox.org/viewtopic.php?t=8046)
- [Virtualbox manual](https://www.virtualbox.org/manual/ch05.html)
- [operating systems (3rd edition) by Deitel](https://www.dropbox.com/s/2uoj0ojxox6uvze/Operating%20systems%20%28Deitel%29%20%283rd%20edition%29%281%29.pdf?dl=0)
