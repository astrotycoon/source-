/**
 * utility functions for dsLib
 * Copyright (c) 2011, Gaurav Mathur <narainmg@gmail.com>
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * See README and COPYING for more details.
 */

#undef DEBUG
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include <config.h>
#include <ds_types.h>
#include <utils.h>

/**
 * @brief seed random number generator
 * 
 * @return n.a.
 */
void dslib_srandom (void)
{
   int fd;
#define SEED_BUF_SIZE   8   
   char buf[SEED_BUF_SIZE];
   unsigned int seed = 0;

   /* todo - try opening other rand devices that might be present on *nix
    * systems
    */   
   fd = open ("/dev/random", O_RDONLY | O_CLOEXEC);
   assert (fd != -1);            
   if (SEED_BUF_SIZE != read (fd, buf, SEED_BUF_SIZE)) /* seed raw material */
      fprintf (stderr, "%s:%d not read %d bytes of seed materialn\n", __FUNCTION__,
               __LINE__, SEED_BUF_SIZE);   
   close (fd);

   /* construct the seed from the raw material */
   seed = seed | buf[0];
   seed = seed << 8;
   seed = seed | buf[1];
   seed = seed << 8;
   seed = seed | buf[2];
   seed = seed << 8;
   seed = seed | buf[3];
   seed = seed << 8;

   /* seed the PRNG */
   srandom (seed);
}

/**
 * @brief generate random number
 * 
 * @return n.a.
 */
unsigned long dslib_random (unsigned int max)
{
   return (random()%(max+1));
}


/**
 * @brief generate random number
 * 
 * @return A handle to the recorded time if successful, NULL if operation failed
 */
void* dslib_record_time (void)
    {
    struct timespec* tp = malloc (sizeof (struct timespec));

    tp->tv_sec = 0;
    tp->tv_nsec = 0;
    
    if (-1 == clock_gettime (CLOCK_MONOTONIC, tp))
        return NULL;

    return tp;
    }

/**
 * @brief calculate the difference in nanoseconds between two time values
 *
 * @param[in] start_time
 * @param[in] end_time 
 * @param[out] diff time difference in nanoseconds
 * @return difference in nanoseconds
 */
ERR_UTILS_E dslib_time_diff (void* start_time, void* end_time, unsigned long* diff)
    {
    unsigned long ldiff;
    struct timespec* endt = (struct timespec*)end_time;
    struct timespec* startt = (struct timespec*)start_time;

    DEBUG_PRINT ("DSLIB[utils]: endt->sec=%ld endt->nsec=%ld\n", endt->tv_sec, endt->tv_nsec);
    DEBUG_PRINT ("DSLIB[utils]: start->sec=%ld start->nsec=%ld\n", startt->tv_sec, startt->tv_nsec);
    if (endt->tv_sec > startt->tv_sec)
        {
        if (endt->tv_nsec > startt->tv_nsec)
            {
            ldiff = endt->tv_nsec - startt->tv_nsec;
            ldiff = ldiff + (endt->tv_sec - startt->tv_sec)*NANOSECONDS;
            }
        else if (endt->tv_nsec < startt->tv_nsec)
            {
            ldiff = (endt->tv_nsec - 1) - startt->tv_nsec;
            ldiff = ldiff + ((endt->tv_sec + NANOSECONDS)- startt->tv_sec)*NANOSECONDS;
            }
        else
            {
            ldiff = 0;
            }        
        }
    else if (endt->tv_sec < startt->tv_sec)        
        {
        return ERR_UTILS_INVALID_PARAM;
        }
    else if (endt->tv_sec == startt->tv_sec)
        {
        if (endt->tv_nsec > startt->tv_nsec)
            {
            ldiff = endt->tv_nsec - startt->tv_nsec;
            }
        else if (endt->tv_nsec < startt->tv_nsec)
            {
            return ERR_UTILS_INVALID_PARAM;
            }
        else
            {
            ldiff = 0;
            }        
        }

    *diff = ldiff;
    return ERR_UTILS_OK;
    }
#undef DEBUG
