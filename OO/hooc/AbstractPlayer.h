#include <features.h>
#include <stdbool.h>
#ifndef __ABSTRACT_PLAYER_H__
#define __ABSTRACT_PLAYER_H__
__BEGIN_DECLS

/**
 * @interface AbstractPlayer
 * @brief AbstractPlayer is an abstract class.
 *
 * Abstract class usually is used as Interface or Prototype.
 */
typedef struct AbstractPlayer AbstractPlayer; //!< An abstract class.

struct AbstractPlayer {
    /** @cond */
    void* _private;
    /** @endcond */

    /**
     * @brief 'open' function prototype
     * @public @memberof AbstractPlayer
     * @fn bool open(AbstractPlayer* self, const char* url);
     *
     * @sa ConcretePlayer::open
     */
    /** @cond */
    bool (*open)(AbstractPlayer* self, const char* url);
    /** @endcond */

    /**
     * @brief 'play' function prototype
     * @public @memberof AbstractPlayer
     * @fn bool play(AbstractPlayer* self);
     *
     * @sa ConcretePlayer::play
     */
    /** @cond */
    bool (*play)(AbstractPlayer* self);
    /** @endcond */

    /**
     * @brief 'close' function prototype
     * @public @memberof AbstractPlayer
     * @fn bool close(AbstractPlayer* self);
     *
     * @sa ConcretePlayer::close
     */
    /** @cond */
    bool (*close)(AbstractPlayer* self);
    /** @endcond */

    /**
     * @brief 'destroy' function prototype
     * @public @memberof AbstractPlayer
     * @fn bool destroy(AbstractPlayer* self);
     *
     * @sa ConcretePlayer::destroy
     */
    /** @cond */
    bool (*destroy)(AbstractPlayer* self);
    /** @endcond */
};

__END_DECLS
#endif //__ABSTRACT_PLAYER_H__
