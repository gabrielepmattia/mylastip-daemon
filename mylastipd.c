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

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *) userdata;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(int argc, char** argv) {
    fprintf(stderr, "[%s][INFO] Starting MyLasytIPd | by gabry3795 - gabry dot gabry at hotmail.it\n", getTime());

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
    char* setting_file_str = (char*) malloc(filesize * sizeof (char));
    int ret;
    while (bytes_left) {
        ret = read(file_desc, setting_file_str + bytes_read, bytes_left);
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
    fprintf(stderr, "[%s][INFO] Setting file succesfully read\n", getTime());
    fprintf(stderr, "[%s][INFO] Parsing JSON in setting file\n", getTime());
    // Parse JSON
    cJSON* root = cJSON_Parse(setting_file_str);
    free(setting_file_str);
    char* key = root->child->valuestring;
    int delay = root->child->next->valueint;
    fprintf(stderr, "[%s][INFO] ==> Given key is %s\n", getTime(), key);
    fprintf(stderr, "[%s][INFO] ==> Given delay is %ds\n", getTime(), delay);
    // Start web loop
    fprintf(stderr, "[%s][INFO] Starting web loop with delay given\n", getTime());
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
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/api/check_in");
            // Now specify the POST data

            // Prepare the string
            fprintf(stderr, "[%s][INFO] Preparing the query string\n", getTime());
            char* post_string = (char*) malloc(1000 * sizeof (char));
            sprintf(post_string, "key=%s&delay=%d", key, delay);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_string);

            // Change to output function of curl
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response_data);

            // Perform the request, res will get the return code
            fprintf(stderr, "[%s][INFO] Sending data to server\n", getTime());
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
        fprintf(stderr, "[%s][INFO] Sleeping for %ds\n", getTime(), delay);
        sleep(delay);
    }

    return (EXIT_SUCCESS);
}