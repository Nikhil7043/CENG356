/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   header.h
 *
 *
 * Created on February 10, 2023, 8:47 PM
 */

#ifndef HEADER_H
#define HEADER_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void setup_memory();

#define MEM_SIZE 1024*1024   // reserve 1 M bytes to play.
#define MIN_DUMP_SIZE  256   // minimum size of the dump size is set to 256 bytes.
#define DUMP_LINE 16    // the size of the memory to be dumped on each row.

#endif /* HEADER_H */

