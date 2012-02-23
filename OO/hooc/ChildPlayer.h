#include <features.h>
#include <stdbool.h>
#ifndef __CHILD_PLAYER_H__
#define __CHILD_PLAYER_H__
__BEGIN_DECLS

/**
 * @class ChildPlayer
 * @extends MyPlayer
 */
typedef struct ChildPlayer ChildPlayer;

/**
 * @brief Use this to create object of ChildPlayer.
 *
 * @return ChildPlayer* The object instance of ChildPlayer.
 * @retval NULL If it fails.
 */
ChildPlayer* ChildPlayer_create(void);

struct ChildPlayer {
    /** @cond */
    void* _private;
    /** @endcond */

    /** @cond */
    bool (*open)(ChildPlayer* self, const char* url);
    /** @endcond */

    /** @cond */
    bool (*play)(ChildPlayer* self);
    /** @endcond */

    /** @cond */
    bool (*close)(ChildPlayer* self);
    /** @endcond */

    /** @cond */
    bool (*destroy)(ChildPlayer* self);
    /** @endcond */

    /** @cond */
    bool (*pause)(ChildPlayer* self);
    /** @endcond */

    /** @cond */
    bool (*setSpeed)(ChildPlayer* self, int speed);
    /** @endcond */
};

__END_DECLS
#endif //__CHILD_PLAYER_H__
