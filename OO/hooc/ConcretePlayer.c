#include <stdlib.h>
#include <string.h>

#include "logger.h"
#include "ConcretePlayer.h"

/*
 * Functions Declaration
 */

/**
 * @brief This is ConcretePlayer's open function.
 * @public @memberof ConcretePlayer
 * @fn bool open(ConcretePlayer* self, const char* url);
 *
 * @param self ConcretePlayer instance
 * @param url The target URL
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool ConcretePlayer_open(ConcretePlayer* self, const char* url);

/**
 * @brief This is ConcretePlayer's play function.
 * @public @memberof ConcretePlayer
 * @fn bool play(ConcretePlayer* self);
 *
 * @param self ConcretePlayer instance
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool ConcretePlayer_play(ConcretePlayer* self);

/**
 * @brief This is ConcretePlayer's close function.
 * @public @memberof ConcretePlayer
 * @fn bool close(ConcretePlayer* self);
 *
 * @param self ConcretePlayer instance
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool ConcretePlayer_close(ConcretePlayer* self);

/**
 * @brief This is ConcretePlayer's destroy function.
 * @public @memberof ConcretePlayer
 * @fn bool destroy(ConcretePlayer* self);
 *
 * @param self ConcretePlayer instance
 *
 * @return bool
 * @retval true If it works.
 * @retval false If it fails.
 */
static bool ConcretePlayer_destroy(ConcretePlayer* self);

/*
 * Inner Utilities
 */

static const ConcretePlayer object_template = {
    .open    = ConcretePlayer_open,
    .play    = ConcretePlayer_play,
    .close   = ConcretePlayer_close,
    .destroy = ConcretePlayer_destroy,
};

/*
 * Functions Definition
 */

ConcretePlayer* ConcretePlayer_create(void)
{
    logger_trace("%s", __FUNCTION__);
    ConcretePlayer* self = malloc(sizeof(ConcretePlayer));

    if (self == NULL) {
        logger_error("Memory not enough");
        return NULL;
    }

    memcpy(self, &object_template, sizeof(ConcretePlayer));

    return self;
}

static bool ConcretePlayer_destroy(ConcretePlayer* self)
{
    logger_trace("%s", __FUNCTION__);
    free(self);
    return true;
}

static bool ConcretePlayer_open(ConcretePlayer* self, const char* url)
{
    logger_trace("%s %s", __FUNCTION__, url);
    return true;
}

static bool ConcretePlayer_play(ConcretePlayer* self)
{
    logger_trace("%s", __FUNCTION__);
    return true;
}

static bool ConcretePlayer_close(ConcretePlayer* self)
{
    logger_trace("%s", __FUNCTION__);
    return true;
}
