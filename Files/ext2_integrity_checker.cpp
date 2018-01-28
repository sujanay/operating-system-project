#include "mbr.h"
#include "datatypes.h"
#include "vdiHeader.h"
#include "ext2.h"
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <fcntl.h>   
#include <unistd.h>     
#include <sys/types.h>
#define width 30
u32 blockSize, nBlockGroups, nBlocksPerGroup;
int startPartition;

using namespace std;

   //code for comparing superblock
bool compareSuperblock(ext2_super_block* sb1, ext2_super_block* sb2)
{
	return ((sb1->s_inodes_count     ==  sb2->s_inodes_count)&&
		(sb1->s_blocks_count     ==  sb2->s_blocks_count)&&
		(sb1->s_r_blocks_count   ==  sb2->s_r_blocks_count)&&
		(sb1->s_first_data_block ==  sb2->s_first_data_block)&&
		(sb1->s_log_block_size   ==  sb2->s_log_block_size)&&
		(sb1->s_log_frag_size    ==  sb2->s_log_frag_size)&&
		(sb1->s_blocks_per_group ==  sb2->s_blocks_per_group)&&
		(sb1->s_frags_per_group  ==  sb2->s_frags_per_group)&&
		(sb1->s_magic            ==  sb2->s_magic)&&
		(sb1->s_minor_rev_level  ==  sb2->s_minor_rev_level)&&
		(sb1->s_creator_os       ==  sb2->s_creator_os)&&
		(sb1->s_rev_level        ==  sb2->s_rev_level)&&
		(sb1->s_first_ino        ==  sb2->s_first_ino)&&
		(sb1->s_inode_size       ==  sb2->s_inode_size));
};

//---------------- Class for the VDI File-----------------//

class VDIFile
{
	public:
		int fd;							 //file descriptor
		int cursor;	 
		int* map;
		vdiHeader vh;
	//public:
		int open(char* filename)
		{	
			if((fd= ::open(filename, O_RDWR))<0)
			{
		        return 1;
			} 
			cursor=0;
			
			::read(fd, &vh, sizeof(vh));
			int blocks_in_HDD=vh.blocks_in_HDD;
	  		map=new int[blocks_in_HDD];			          //allocate your map
	  		lseek(fd, vh.offset_blocks, SEEK_SET); 			  //this is where the map is 
	  		::read(fd, map, blocks_in_HDD*sizeof(int));

		}

		void vdiRead(void* buf, u32 size)
		{
			::read(fd, buf, size);
		}

		void vdiSeek(u32 offset)
		{
			::lseek(fd, offset, SEEK_SET);
		}

		void displayStatistics()
		{
			cout << setw(width + 20)  << right << vh.text;   cout << endl;
	  		cout << setw(width)  << right << "SIGNATURE: "        << hex << vh.signature << endl;
			cout << setw(width)  << right << "VERSION: "          << hex << vh.version << endl;
			cout << setw(width)  << right << "HEADER SIZE: "      << dec << vh.header_size << endl;
			cout << setw(width)  << right << "IMAGE TYPE: "       << dec << vh.image_type << endl;	
			cout << setw(width)  << right << "IMAGE FLAGS: "      << dec << vh.image_flags << endl;
		//	cout << setw(width)  << right << "DESCRIPTION: "      << vh.description << endl;
			cout << setw(width)  << right << "OFFSET BLOCKS: "    << hex << vh.offset_blocks << endl;
			cout << setw(width)  << right << "OFFSET DATA: "	  << hex << vh.offset_data << endl;
		//	cout << setw(width)  << right << "CYINDERS: "         << dec << vh.cylinders << endl;
		//	cout << setw(width)  << right << "HEADS: "            << dec << vh.heads << endl;
		//	cout << setw(width)  << right << "SECTORS: "          << dec << vh.sectors << endl;
			cout << setw(width)  << right << "SECTOR SIZE: "      << dec << vh.sector_size << endl;
		//	cout << setw(width)  << right << "UNUSED1: "          << dec << vh.unused1 << endl;
			cout << setw(width)  << right << "DISK SIZE: "        << dec << vh.disk_size << endl;
			cout << setw(width)  << right << "BLOCK SIZE: "       << dec << vh.block_size << endl;
		//	cout << setw(width)  << right << "BLOCK EXTRA: "      << dec << vh.block_extra << endl;
			cout << setw(width)  << right << "BLOCKS IN HDD: "    << dec << vh.blocks_in_HDD << endl;
			cout << setw(width)  << right << "BLOCKS ALLOCATED: " << dec << vh.blocks_allocated << endl;
			cout << endl;
		//	cout << "*******************************************************************************"<< endl;

	}


		//--------------------vdi translate function---------------------//

		u32 vdiTranslate(u32 type)
		{
			u32 page;
			u32 offset;
			u32 location;
			u32 frame;

			page=(type/vh.offset_blocks);
			offset=(type%vh.offset_blocks);
			frame=map[page];
			location=vh.offset_data + (frame*vh.offset_blocks) + offset;
			return location;
		}

		void vdiWrite(void* buf, int size)
		{
	 		::write(fd,buf,size);
		}

		void vdiClose()
		{
	   		::close(fd);
	  		delete[] map; 						//deallocate the map, as it has been allocated.
	  		//cout<<"I am closing...!!!"<<endl;
		}		
};

void fetchBlock(VDIFile* vf, u8* buf, u32 b)
{
	::lseek(vf->fd,vf->vdiTranslate(blockSize*b+startPartition),SEEK_SET);
	vf->vdiRead(buf,blockSize);
}

void fetchSuperblock(VDIFile* vf, ext2_super_block* sb1, int i)
{
	if(i<0)
         {
	   cout << "Trying to access superblock with negative index" << endl;
	}
	else if(i==0)
	{
		::lseek(vf->fd, vf->vdiTranslate(i*nBlocksPerGroup*blockSize+startPartition+1024), SEEK_SET);
		vf->vdiRead(sb1,1024);
	}
	else
	{
		::lseek(vf->fd, vf->vdiTranslate(i*nBlocksPerGroup*blockSize+startPartition), SEEK_SET);
		vf->vdiRead(sb1,1024);
	}
}
u32 superblockAddress(VDIFile* vf,int i)
{
	if(i<0)
         {
	   cout << "Negative index for superblock" << endl;
		 return -1;
	}
	else if(i==0)
	{
		 return vf->vdiTranslate(startPartition)+1024;
		
	}
	else
	{
		return vf->vdiTranslate(startPartition)+i*nBlocksPerGroup*blockSize;
	}
}

//--------------main function----------------//

int main(int argc, char* argv[])
{

	char* filename=argv[1];			 //filename is to store the name of the file passed
						 //through command line
	
	VDIFile vf;
	
	if(vf.open(filename)==1){
	    cout<< "ERROR!!! CANNOT OPEN THE FILE: "<< filename << endl;
		 return 0;
	}else{

	vf.displayStatistics();

	//--------------------reading the MBR ---------------------//

	BootSector bs;		           //reads the bootsector
        vf.vdiSeek(vf.vh.offset_data);
	vf.vdiRead(&bs, sizeof(bs));

	//check if the start of the partition is of type 0x83
	int i;
	for(i=0; i<4; i++){
		if(bs.partitionTable[i].type==0x83){	
			startPartition = bs.partitionTable[i].firstSector*512;
			break;
		}
	}
	if(i==4){
		vf.vdiClose();
		cout << "There is no boot partition!!!" << endl;
		return 0;
        }

	//*********** Master Boot Record Content *************//
//	cout << "------------------------------------------------------------------------"<< endl;
	cout << "Partition" << setw(width-5) << right << "Type" << setw(width-4) << right << "Start" << endl;
//	cout << "------------------------------------------------------------------------"<< endl;
	for(int i=0; i<4; i++)
	{
		cout << "Partition["   << i+1   << "]" << setw(width-10) 
		     << right << hex   << "0x"  << (int)bs.partitionTable[i].type 
		     << setw(width-4)  << right << dec
		     << bs.partitionTable[i].firstSector   << endl;
	}
	
	cout << "MBR MAGIC: " << hex << bs.magic <<endl;
	cout << endl;
//	cout << "-----------------------------------------------------------------------"<< endl;
	
   	//----------------------Getting into the superblock-------------------//
	
	u32 startOfSuperBlock=vf.vdiTranslate(startPartition+1024);
	ext2_super_block sb;
	vf.vdiSeek(startOfSuperBlock);
	vf.vdiRead(&sb, sizeof(sb));
		
	if((u16)sb.s_magic!=(u16)0xef53)
           {
		cout << "Bad superblock magic" << endl;
		vf.vdiClose();
		return 0;
	}
	cout << "Superblock MAGIC: " << hex << sb.s_magic <<endl;
	cout << endl;

       //--------------------state of the file system; 1=clean and 2=dirty(or Error)-------------//
       string fileSystemState=(sb.s_state==1)?"CLEAN":"DIRTY";        
 	
	//--------------------code for ext2 filesystem-------------------------//
	u32
	//	blockSize,	 //blockSize is made global
	//	nBlockGroups,
		nGDTBlocks,
		inodesPerBlock,
		addrPerBlock,
		maxBlocks,
		gdPerBlock,
		gdtBlocksUsed;

		nBlocksPerGroup= sb.s_blocks_per_group;	
	blockSize = 1024 << sb.s_log_block_size;

	nBlockGroups = (sb.s_blocks_count + sb.s_blocks_per_group - 1) /
						sb.s_blocks_per_group;
	
	addrPerBlock = blockSize / sizeof(u32);
	inodesPerBlock = blockSize / sb.s_inode_size;

	gdPerBlock = blockSize / sizeof(ext2_group_desc);
	gdtBlocksUsed = (nBlockGroups + gdPerBlock - 1) / gdPerBlock;	
	

	maxBlocks = 0x003fffff;
	if (maxBlocks > sb.s_blocks_count)
		maxBlocks = sb.s_blocks_count;
	maxBlocks <<= 10;
	maxBlocks = (maxBlocks + sb.s_blocks_per_group - 1 -
					sb.s_first_data_block) / sb.s_blocks_per_group;
	nGDTBlocks = (maxBlocks + gdPerBlock - 1) / gdPerBlock - gdtBlocksUsed;
	if (nGDTBlocks > addrPerBlock)
		nGDTBlocks = addrPerBlock;
	nGDTBlocks += gdtBlocksUsed;


        //-----------------reading block group descriptor table-----------------------//

	u32 addOfBGDT= vf.vdiTranslate(startPartition + 1024 + sizeof(sb));
	ext2_group_desc bgdt[nBlockGroups];       				  //block group descriptor table

	for(int i=0; i< nBlockGroups; i++)
	{
		vf.vdiSeek(addOfBGDT+(i*sizeof(bgdt[i])));
		vf.vdiRead(&bgdt[i], sizeof(bgdt[i]));		
	}

	//---------------function for finding the number of files and directories----------------//
	u32 numDirectory=0;                 //number of existing directories
	

	for(int i=0; i< nBlockGroups; i++){
		numDirectory+=bgdt[i].bg_used_dirs_count;
	}
	
	u32 numFiles=sb.s_inodes_count-sb.s_free_inodes_count-numDirectory;	//number of existing files

	//------------------Displaying the File info------------------------//
	
	cout << setw(width - 10) << right << "   Total Filesystem size: " << dec << (sb.s_blocks_count * blockSize) << " bytes" << endl;
	cout << setw(width - 10) << right << "    Space currently used: " << dec << (sb.s_blocks_count-sb.s_free_blocks_count)*blockSize 
	     << " bytes" << endl;
	cout << setw(width - 10) << right << "Size available for files: " << dec << (sb.s_free_blocks_count*blockSize) << "  bytes" << 			endl;
	cout << endl;
	cout << setw(width - 10) << right << "Blocks" << setw(width-15)  << right << "Inodes" << endl;
	cout << setw(width - 20) << right << "Total:  " << setw(width-20) << right << dec << sb.s_blocks_count
     	     << setw(width - 15) << right << dec << sb.s_inodes_count << endl;

	cout << setw(width - 20) << right << "Free:   " << setw(width-20) << right << dec << sb.s_free_blocks_count 
             << setw(width - 15) << right << dec << sb.s_free_inodes_count << endl;
	cout << setw(width - 20) << right << "Reserved:" << setw(width-20) << right << dec << sb.s_r_blocks_count << endl;
        cout << endl;   
	cout << setw(width - 10) << right << "First data block:  " << dec << sb.s_first_data_block << endl;
	cout << setw(width - 10) << right << "Block size:  " << dec << blockSize << endl;
	cout << setw(width - 10) << right << "Block groups:  " << dec << nBlockGroups << endl;
	cout << setw(width - 10) << right << "Blocks per group:  " << dec << sb.s_blocks_per_group << endl;
	cout << setw(width - 10) << right << "Inodes per group:  " << dec << sb.s_inodes_per_group << endl;
	cout << setw(width - 10) << right << "GDT blocks:  " << dec << nGDTBlocks << endl;
	cout << setw(width - 10) << right << "Inodes per block:  " << dec << inodesPerBlock << endl;
	cout << setw(width - 10) << right << "Addrs per block:  " << dec << addrPerBlock << endl;
	cout << setw(width - 8) << right << "      Number of existing Files: " << numFiles << endl;
	cout << setw(width - 8) << right << "Number of existing directories: " << numDirectory << endl;
	cout << setw(width - 8) << right << "          State of File System: " << fileSystemState << endl;

	cout << endl;

	//-------------------Displaying Block Group contents-----------------------//

	cout << "Group    Block map    Inode map    Inode tbl   bFree     iFree "<< endl;
	for (int i=0;i<nBlockGroups;i++)
	{
		cout << setw(5)  <<  i
		     << setw(13) << bgdt[i].bg_block_bitmap
		     << setw(13) << bgdt[i].bg_inode_bitmap
		     << setw(10) << bgdt[i].bg_inode_table
		     << setw(10) << bgdt[i].bg_free_blocks_count
		     << setw(10) << bgdt[i].bg_free_inodes_count << endl;
	}
	cout << endl;

	//--------------------fetching block-------------------//

	u32 blockNum=12;
	u8 buf[blockNum*blockSize];
	fetchBlock(&vf,buf, blockNum); 

	//-------------fetching and correcting superblock------------------//

	ext2_super_block sb1;
	for(int i=1; i<8; i+=2)
	{
	      if(blockSize==1024){
        
		fetchSuperblock(&vf, &sb1, i);
		if(!compareSuperblock(&sb,&sb1)){
		
	  	cout << "Superblock at Blockgroup " << i << " is NOT consistent with the Master Superblock" << endl;
		vf.vdiSeek(superblockAddress(&vf,i));
		vf.vdiWrite(&sb,1024);
	}
	else{
		cout << "Superblock at BlockGroup "<< i << " is consistent with the Master Superblock" << endl;
	 }
	}else{
	//	cout << "There are no copies of superblock" << endl;
		break;	
	}
    }

	vf.vdiClose();
	return 0;

	}
	
	
}
