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

#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>

#include "common.h"
#include "cJSON.h"

int main(int argc, char** argv) {
    fprintf(stderr, "[INFO] Starting MyLasytIPd\n");
    CURL *curl;
    CURLcode res;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if (curl) {
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        curl_easy_setopt(curl, CURLOPT_URL, "http://postit.example.com/moo.cgi");
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return (EXIT_SUCCESS);
}

