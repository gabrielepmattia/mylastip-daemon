/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   utils.h
 * Author: gaby3795 <gabry dot gabry at hotmail.it>
 *
 * Created on June 20, 2016, 4:13 PM
 */

#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif
    
char* getTime();

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
struct MemoryStruct {
    char *memory;
    size_t size;
};

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */

