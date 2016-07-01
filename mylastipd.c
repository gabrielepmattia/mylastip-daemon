/*
    This file is part of MyLastIPd.

    MyLastIPd is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MyLastIPd is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MyLastIPd.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   mylastipd.c
 * Author: gabry3795
 *
 * Created on June 20, 2016, 3:30 PM
 */

#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/errno.h>

#include <curl/curl.h>

#include "common.h"
#include "cJSON.h"
#include "utils.h"

int main(int argc, char** argv) {
    fprintf(stderr, "[%s][INFO] Starting MyLasytIPd\n", getTime());

    // Perfom reading with descriptor, we will know how many bytes we have to read..
    int file_desc = open(SETTING_FILENAME, O_RDONLY);
    if (file_desc < 0) {
        fprintf(stderr, "[%s][ERROR] Setting file not found at %s. Exiting...\n", getTime(), SETTING_FILENAME);
        exit(EXIT_FAILURE);
    }
    // Get the filesize
    struct stat st;
    stat(SETTING_FILENAME, &st);
    ssize_t filesize = st.st_size;
    fprintf(stderr, "[%s][INFO] %s size is: %ld bytes\n", getTime(), SETTING_FILENAME, filesize);
    // Start reading
    int bytes_read = 0;
    long bytes_left = filesize;
    char* buf = (char*) malloc(filesize * sizeof (char));
    int ret;
    while (bytes_left) {
        ret = read(file_desc, buf + bytes_read, bytes_left);
        // File ends
        if (ret == 0) break;
        if (ret == -1) {
            if (errno == EINTR) continue;
            fprintf(stderr, "[%s][ERROR] There was an error reading the setting file at %s. Exiting...\n", getTime(), SETTING_FILENAME);
            exit(EXIT_FAILURE);
        }
        bytes_read += ret;
        bytes_left -= ret;
    }
    close(file_desc);
    printf("Read content is:\n%s", buf);

    /*
    char buf[2048];
    char* setting_file = (char*)malloc(5024*sizeof(char));
    
    // Read the configuration file once
    FILE* fp = fopen("test.json", "r");
    if(fp == NULL) fprintf(stderr,"[ERROR] No setting file\n");
    
    int bytes_read = 0;
    while(fgets(buf, sizeof(buf), fp)) {
        sprintf(setting_file + bytes_read, buf);
        bytes_read += strlen(buf);
    }
    fclose(fp);
    
    printf("Read \n%s", setting_file);
     */ /*
    CURL *curl;
    CURLcode res;

    // In windows, this will init the winsock stuff
    curl_global_init(CURL_GLOBAL_ALL);

    // get a curl handle
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://local");
        // Now specify the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        // always cleanup 
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    */
    return (EXIT_SUCCESS);
}