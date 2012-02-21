
typedef enum {
   ERR_UTILS_ERROR_HIGH = -512, /* fencepost */
   ERR_UTILS_MALLOC_FAIL,
   ERR_UTILS_INVALID_PARAM,
   ERR_UTILS_ERR = -1, /* generic operational error */
   ERR_UTILS_OK = 0 /* routine returned without any errors */
} ERR_UTILS_E;

void dslib_srandom(void);

unsigned long dslib_random(unsigned int max);
