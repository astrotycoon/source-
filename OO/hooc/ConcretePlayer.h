#include <features.h>
#include <stdbool.h>
#include "AbstractPlayer.h"
#ifndef __CONCRETE_PLAYER_H__
#define __CONCRETE_PLAYER_H__
__BEGIN_DECLS

/**
 * @class ConcretePlayer
 * @implements AbstractPlayer
 * @brief A concrete class.
 *
 * A class can extend from another class or implement some interface.
 * This ConcretePlayer implement AbstractPlayer.
 */
typedef AbstractPlayer ConcretePlayer; //!< A concrete class that implement some abstract class.

/**
 * @brief Use this to create object of ConcretePlayer.
 *
 * @return ConcretePlayer* The object instance of ConcretePlayer.
 * @retval NULL If it fails.
 */
ConcretePlayer* ConcretePlayer_create(void);

__END_DECLS
#endif //__CONCRECTE_PLAYER_H__
