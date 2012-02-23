#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "MyPlayer.h"

/*
 * Functions Declaration
 */

/**
 * @brief This is MyPlayer's open function.
 * @memberof MyPlayer
 * @fn bool open(MyPlayer* self, const char* url);
 *
 * @param self MyPlayer instance
 * @param url The target URL
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool MyPlayer_open(MyPlayer* self, const char* url);

/**
 * @brief This is MyPlayer's play function.
 * @memberof MyPlayer
 * @fn bool play(MyPlayer* self);
 *
 * @param self MyPlayer instance
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool MyPlayer_play(MyPlayer* self);

/**
 * @brief This is MyPlayer's close function.
 * @memberof MyPlayer
 * @fn bool close(MyPlayer* self);
 *
 * @param self MyPlayer instance
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool MyPlayer_close(MyPlayer* self);

/**
 * @brief This is MyPlayer's destroy function.
 * @memberof MyPlayer
 * @fn bool destroy(MyPlayer* self);
 *
 * @param self MyPlayer instance
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool MyPlayer_destroy(MyPlayer* self);

/**
 * @brief MyPlayer's pause
 * @memberof MyPlayer
 * @fn bool pause(MyPlayer* self);
 *
 * @param self MyPlayer's instance
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool MyPlayer_pause(MyPlayer* self);

/*
 * Inner Utilities
 */

static const MyPlayer object_template = {
    .open    = MyPlayer_open,
    .play    = MyPlayer_play,
    .close   = MyPlayer_close,
    .destroy = MyPlayer_destroy,
    .pause   = MyPlayer_pause,
};

/*
 * Functions Definition
 */

MyPlayer* MyPlayer_create(void)
{
    logger_trace("%s", __FUNCTION__);
    MyPlayer* self = malloc(sizeof(MyPlayer));

    if (self == NULL) {
        logger_error("Memory not enough");
        return NULL;
    }

    memcpy(self, &object_template, sizeof(MyPlayer));

    return self;
}

static bool MyPlayer_destroy(MyPlayer* self)
{
    logger_trace("%s", __FUNCTION__);
    free(self);
    return true;
}

static bool MyPlayer_pause(MyPlayer* self)
{
    logger_trace("%s", __FUNCTION__);
    return true;
}

static bool MyPlayer_open(MyPlayer* self, const char* url)
{
    logger_trace("%s %s", __FUNCTION__, url);
    return true;
}

static bool MyPlayer_play(MyPlayer* self)
{
    logger_trace("%s", __FUNCTION__);
    return true;
}

static bool MyPlayer_close(MyPlayer* self)
{
    logger_trace("%s", __FUNCTION__);
    return true;
}
