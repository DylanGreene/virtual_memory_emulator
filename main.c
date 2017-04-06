/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// Global Variables
int npages;
int nframes;
char *algorithm;
struct disk *disk;
int *frame_table;


void page_fault_handler( struct page_table *pt, int page ){
	printf("page fault on page #%d\n",page);

	if(!strcmp(algorithm, "rand")) {

		int curr_bits, curr_frame;
		page_table_get_entry(pt, page, &curr_frame, &curr_bits);

		// Check if the page table entry is not loaded in physmem
		if(!curr_bits){
			int frame = rand() % nframes;
			char *physmem = page_table_get_physmem(pt);
			int prev_bits, prev_frame;

			// Remove the old page from physmem frame
			if(frame_table[frame] >= 0){
				page_table_get_entry(pt, frame_table[frame], &prev_frame, &prev_bits);
				// If it is dirty write it to disk
				if(prev_bits == (PROT_READ|PROT_WRITE)){
					disk_write(disk, frame_table[frame], &physmem[frame*PAGE_SIZE]);
				}
				// Update to signify no longer loaded in physmem
				page_table_set_entry(pt, frame_table[frame], 0, 0);
			}

			// Put the new page into the the memory and update the tables
			page_table_set_entry(pt, page, frame, PROT_READ);
			disk_read(disk, page, &physmem[frame*PAGE_SIZE]);
			frame_table[frame] = page;

		// Add the WRITE dirty bit
		}else if(curr_bits == PROT_READ){
			page_table_set_entry(pt, page, curr_frame, PROT_READ|PROT_WRITE);
		}
		page_table_print(pt);

	}else if(!strcmp(algorithm, "fifo")) {


	}else if(!strcmp(algorithm, "lru")) {

	}else{
		fprintf(stderr,"unknown algorithm: %s\n", algorithm);
		return;
	}
}

int main( int argc, char *argv[] ){
	if(argc!=5){
		printf("use: virtmem <npages> <nframes> <rand|fifo|lru> <sort|scan|focus>\n");
		return 1;
	}

	// Parse the cl args
	npages = atoi(argv[1]);
	nframes = atoi(argv[2]);
	algorithm = argv[3];
	const char *program = argv[4];

	// Create frame table to look up what page is in each frame
	frame_table = malloc(nframes * sizeof(int));
	int i;
	for(i = 0; i < nframes; i++){
		frame_table[i] = -1;
	}

	// Seed the rand function with time for more randomness
	//srand(time(NULL));

	disk = disk_open("myvirtualdisk",npages);
	if(!disk){
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt){
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	char *physmem = page_table_get_physmem(pt);

	if(!strcmp(program, "sort")){
		sort_program(virtmem,npages*PAGE_SIZE);

	}else if(!strcmp(program, "scan")){
		scan_program(virtmem,npages*PAGE_SIZE);

	}else if(!strcmp(program, "focus")){
		focus_program(virtmem,npages*PAGE_SIZE);

	}else{
		fprintf(stderr,"unknown program: %s\n",argv[4]);
		return 1;
	}

	page_table_delete(pt);
	disk_close(disk);
	free(frame_table);

	return 0;
}
