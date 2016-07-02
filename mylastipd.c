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

/*
 * Seen:
 *  - https://curl.haxx.se/libcurl/c/getinmemory.html
 *  - https://curl.haxx.se/libcurl/c/http-post.html
 */

#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/errno.h>

#include <curl/curl.h>

#include "common.h"
#include "cJSON.h"
#include "utils.h"
#include "mylastipd.h"

int main(int argc, char** argv) {
    fprintf(stderr, "[%s][INFO] Starting MyLasytIPd | by gabry3795 - gabry dot gabry at hotmail.it\n", getTime());
    char* setting_file_str = read_file(SETTING_FILENAME);
    fprintf(stderr, "[%s][INFO] Parsing JSON in setting file\n", getTime());
    // Parse JSON and check it
    cJSON* root = cJSON_Parse(setting_file_str);
    settings_t* settings = check_parsed_data(root);
    // Clean before loop
    free(setting_file_str);
    free(root);
    // Start web loop
    fprintf(stderr, "[%s][INFO] Starting web loop with data given\n", getTime());
    struct MemoryStruct response_data;
    while (1) {
        fprintf(stderr, "=========================== Main L O O P ===========================\n");
        CURL *curl;
        CURLcode res;

        response_data.memory = malloc(1); /* will be grown as needed by the realloc above */
        response_data.size = 0;

        // In windows, this will init the winsock stuff
        curl_global_init(CURL_GLOBAL_ALL);

        // get a curl handle
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, settings->server);
            // Now specify the POST data

            // Prepare the string
            fprintf(stderr, "[%s][INFO] Preparing the query string\n", getTime());
            char* post_string = (char*) malloc(1000 * sizeof (char));
            sprintf(post_string, "key=%s&delay=%d", settings->key, settings->delay);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_string);

            // Change to output function of curl
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response_data);

            // Perform the request, res will get the return code
            fprintf(stderr, "[%s][INFO] Sending data to server %s\n", getTime(), settings->server);
            res = curl_easy_perform(curl);

            // Check for errors
            if (res != CURLE_OK)
                fprintf(stderr, "[%s][ERROR] Unable to send data to server :: %s\n", getTime(), curl_easy_strerror(res));
                //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            else
                fprintf(stderr, "[%s][INFO] OK! Response received :: %s\n", getTime(), response_data.memory);

            /* How to get IP from cURL
            char* ip;
            curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &ip);
            printf("Local ip %s\n", ip);
             */

            // always cleanup 
            curl_easy_cleanup(curl);
            free(post_string);
            free(response_data.memory);
        }
        curl_global_cleanup();
        fprintf(stderr, "[%s][INFO] Sleeping for %ds\n", getTime(), settings->delay);
        sleep(settings->delay);
    }

    return (EXIT_SUCCESS);
}

char* read_file(char* file_path) {
    // Perfom reading with descriptor, we will know how many bytes we have to read..
    int file_desc = open(SETTING_FILENAME, O_RDONLY);
    if (file_desc < 0) {
        fprintf(stderr, "[%s][ERROR] Setting file not found at %s. Exiting...\n", getTime(), file_path);
        exit(EXIT_FAILURE);
    }
    // Get the filesize
    struct stat st;
    stat(SETTING_FILENAME, &st);
    ssize_t filesize = st.st_size;
    fprintf(stderr, "[%s][INFO] %s size is: %ld bytes\n", getTime(), file_path, filesize);
    // Read
    int bytes_read = 0;
    long bytes_left = filesize;
    char* setting_file_str = (char*) malloc(filesize * sizeof (char));
    int ret;
    while (bytes_left) {
        ret = read(file_desc, setting_file_str + bytes_read, bytes_left);
        // File ends
        if (ret == 0) break;
        if (ret == -1) {
            if (errno == EINTR) continue;
            fprintf(stderr, "[%s][ERROR] There was an error reading the setting file at %s. Exiting...\n", getTime(), file_path);
            exit(EXIT_FAILURE);
        }
        bytes_read += ret;
        bytes_left -= ret;
    }
    close(file_desc);
    fprintf(stderr, "[%s][INFO] Setting file succesfully read\n", getTime());
    return setting_file_str;
}

/**
 * Parse settings
 */
settings_t* check_parsed_data(cJSON* root) {
    fprintf(stderr, "[%s][INFO] Checking parsed data\n", getTime());
    settings_t* settings = (settings_t*) malloc(sizeof (settings_t));
    // Parse the entire tree
    cJSON* current_child = root->child;
    while (current_child) {
        if (!strcmp(current_child->string, "server")) settings->server = current_child->valuestring;
        if (!strcmp(current_child->string, "key")) settings->key = current_child->valuestring;
        if (!strcmp(current_child->string, "delay")) settings->delay = current_child->valueint;
        current_child = current_child->next;
    }
    if (!settings->server || !settings->key || !settings->delay) {
        if (!settings->server) fprintf(stderr, "[%s][ERROR] ==> Server not provided. Exiting...\n", getTime());
        if (!settings->key) fprintf(stderr, "[%s][ERROR] ==> Key not provided. Exiting...\n", getTime());
        if (!settings->delay) fprintf(stderr, "[%s][ERROR] ==> Delay not provided. Exiting...\n", getTime());
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "[%s][INFO] ==> Given server is %s\n", getTime(), settings->server);
    fprintf(stderr, "[%s][INFO] ==> Given key is %s\n", getTime(), settings->key);
    fprintf(stderr, "[%s][INFO] ==> Given delay is %ds\n", getTime(), settings->delay);
    
    // Prepare server address
    char* server_addr = (char*)malloc(2000*sizeof(char));
    sprintf(server_addr, "%s/api/check-in",settings->server);
    free(settings->server);
    if(realloc(server_addr, strlen(server_addr)));
    settings->server = server_addr;
    return settings;
}