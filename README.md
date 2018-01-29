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

# How to
1. Get the test VDI files? [VDI Test Files](https://drive.google.com/open?id=0BwRCEG_n3G_jc1I0NkJOV3BCWHc)
2. Compile and run the file? see **How to compile.md** inside **File** Folder

# Organization of the project
There are a total of five files, four **header** files and one **.cpp**, file for this project inside **File** folder:
- **ext2_integrity_checker.cpp**
- **datatypes.h**
- **ext2.h**
- **mbr.h**
- **vdiHeader.h**

The **main()** function is in the **ext2_integrity_checker.cpp** file. The **main()** function does the following:
> 1. Reads the **VDI** file in an object of **VDIFile** class
> 2. Displays the **VDI Header Information**
> 3. Reads the **Master Boot Record (MBR)**'s content grabs the **Magic Number** and displays it.
> 4. Reads the **Superblock**'s **MAGIC** number and conforms to be equal to **0xef53**.
> 5. Finally, reads the **ext2** filesystem and displays information such as **Total filesystem size**, **Number of files**,
> **Number of Directories**, etc.

# Future Enhancements
This project was accomplished for the partial fulfilment of **Operating Systems** course. So, there are several issues with
the codes in the project such as:
> 1. It does not implement counting the **inodes** and **directories** in the filesystem.
> 2. The code is not very clean.

Therefore, this project can be extended to overcome the above mentioned issues.


# Acknowledgement
I would like to thank **Dr. Kramer** (Youngstown State University) for giving us the opportunity to do this project.

# References
- [The Second Extended File System (a.k.a. ext2 file system)](http://www.nongnu.org/ext2-doc/ext2.html)
- [GNU low-level I/O](https://www.gnu.org/software/libc/manual/html_node/Opening-and-Closing-Files.html#Opening-and-Closing-Files)
- [Virtualbox Forum](https://forums.virtualbox.org/viewtopic.php?t=8046)
- [Virtualbox manual](https://www.virtualbox.org/manual/ch05.html)
- [operating systems (3rd edition) by Deitel](https://www.dropbox.com/s/2uoj0ojxox6uvze/Operating%20systems%20%28Deitel%29%20%283rd%20edition%29%281%29.pdf?dl=0)
