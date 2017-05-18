//vdiHeader.h
#ifndef VDIHEADER_H
#define VDIHEADER_H
typedef unsigned int _uint32;
typedef long long int _uint64;

//----------- VDI Header Structure -----------//
struct vdiHeader{
	   char text[64];		     //VDI TEXT 
	_uint32 signature;           //Image Signature
	_uint32	version;			 //Version
	_uint32 header_size;		 //Size of Header
	_uint32 image_type;			 //Image Type; 1: Dynamic VDI and O: Fixed VDI
	_uint32 image_flags;		 //Image Flags;
	   char description[256];	 //Image Description
	_uint32 offset_blocks;		 //offsetBlocks
	_uint32 offset_data;		 //offsetData
	_uint32 cylinders;			 //#Cylinders
	_uint32 heads;				 //#Heads
	_uint32 sectors;			 //#Sectors
	_uint32 sector_size;		 //SectorSize
	_uint32 unused1;			 //-- unused --
	_uint64 disk_size;			 //DiskSize (Bytes)
	_uint32 block_size;			 //BlockSize
	_uint32 block_extra;		 //Block Extra Data
	_uint32 blocks_in_HDD;  	 //#BlocksInHDD
	_uint32 blocks_allocated;	 //#BlocksAllocated
	};
	
#endif