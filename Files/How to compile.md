#            How to compile and test the code

- Our code works for both static and dynamic VDI files
- It does spoiler alert for inconsistent superblock copies and
  writes the master superblock copy to the backup superblock 
  and makes the superblock copies consistent after the test is
  run on the VDI file.

#            Compiling and Running the code

1. The **main()** function is inside "ext2_integrity_checker.cpp" file
2. Compiling the **integrity_checker.cpp** file
     >	~$ g++ ext2_integrity_checker.cpp
3. Keep the VDI test files inside the **test** folder (create one if not created)

4. Testing the file
     >	~$ ./a.out test/<filename>  

**where "test" is the folder that has VDI test files***

