# operating-system-project

GOAL: Develop a program that checks the integrity of an ext2 filesystem, 
      reporting any errors found and possibly correcting the errors.

This program examines a VDI (Virtual Disk Image) file containing a virtual disk with a single 
partition formatted as an ext2 filesystem. The filename of the file to be examined is given on 
the command line.
      
This program displays the following general statistics about the filesystem:
• Total filesystem size in bytes
• Size available for files (used and unused)
• Amount of space currently used
• Number of possible files and directories (number of inodes)
• Number of existing files
• Number of existing directories
• Number of block groups, with the information for each block group
• Block size in bytes
• State of the filesystem

This program also check the following items:
• The appropriate magic number of the superblock
• All copies of the superblock must be consistent (spoiler alert)
• All copies of the block group table must be consistent (see ab)

This program works fine for 1KB, 2KB and 4KB block sizes.
