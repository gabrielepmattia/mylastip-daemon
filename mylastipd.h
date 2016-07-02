/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mylastipd.h
 * Author: gaby3795 <gabry dot gabry at hotmail dot com>
 *
 * Created on June 20, 2016, 4:31 PM
 */

#ifndef MYLASTIPD_H
#define MYLASTIPD_H

#ifdef __cplusplus
extern "C" {
#endif

    
char* read_file(char* file_path);

typedef struct settings{
    char* server;       // Server name provided
    char* key;          // Key provided
    int delay;          // Delay provided
    // Machine data
    char* uname;
} settings_t;
settings_t* check_parsed_data(cJSON* root);

#ifdef __cplusplus
}
#endif

#endif /* MYLASTIPD_H */

