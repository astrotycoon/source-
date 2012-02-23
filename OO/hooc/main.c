#include <stdio.h>
#include "Object.h"
#include "MyPlayer.h"
#include "AbstractPlayer.h"
#include "ChildPlayer.h"
#include "ConcretePlayer.h"

/**
 * @mainpage Handy Object Oriented C
 *
 * <H2>Introduction</H2>
 *
 * This sample attempts to implement C as Object Oriented look-like.
 *
 * <H2>Constractor &amp; Constractor</H2>
 * - Handy Object Oriented C
 *  - Constractor
 *    @code Object* instance = Object_create(); @endcode
 *  - Destructor
 *    @code instance->destroy(instance); @endcode
 * - C++
 *  - Constractor
 *    @code Object* instance = new Object(); @endcode
 *  - Destructor
 *    @code delete instance; @endcode
 *
 * <H2>Interface</H2>
 *
 * Class @ref ConcretePlayer implements Interface @ref AbstractPlayer
 *
 * @code
 * AbstractPlayer* player = ConcretePlayer_create();
 * player->open(player, "video.avi");
 * player->play(player);
 * player->close(player);
 * player->destroy(player);
 * @endcode
 *
 * <H2>Inheritance</H2>
 *
 * Class @ref ChildPlayer extends Class @ref MyPlayer
 *
 * @code
 * MyPlayer* player = MyPlayer_create();
 * player->open(player, "video.avi");
 * player->play(player);
 * player->pause(player);
 * player->close(player);
 * player->destroy(player);
 *
 * ChildPlayer* player2 = ChildPlayer_create();
 * player2->open(player2, "video.avi");
 * player2->play(player2);
 * player2->pause(player2);
 * player2->setSpeed(player2, 1024);
 * player2->close(player2);
 * player2->destroy(player2);
 * @endcode
 */

int main(int argc, char* argv[])
{
#if 1
    Object* objA = Object_create();
    Object* objB = objA->ref(objA);
    Object* objC = objB->ref(objB);
    //*/
    objA->unref(objA);
    objA = NULL;
    objB->unref(objB);
    objB = NULL;
    objC->unref(objC);
    objC = NULL;
    /*/
    objA->destroy(objA);
    //*/
#else
    AbstractPlayer* con = NULL;
    MyPlayer* player = NULL;
    ChildPlayer* child = NULL;

    /* The first sample. */
    con = ConcretePlayer_create();
    if (con->open(con, "Abstract & Concrete") == true) {
        con->play(con);
        con->close(con);
    }
    if (con->destroy(con) == true) {
        con = NULL;
    }

    /* The second sample. */
    player = MyPlayer_create();
    if (player->open(player, "This is my player.") == true) {
        player->play(player);
        player->pause(player);
        player->close(player);
    }
    if (player->destroy(player) == true) {
        player = NULL;
    }

    /* The third sample. */
    child = ChildPlayer_create();
    if (child->open(child, "This is Mr. Child.") == true) {
        child->play(child);
        child->pause(child);
        child->play(child);
        child->setSpeed(child, 1024);
        child->close(child);
    }
    if (child->destroy(child) == true) {
        child = NULL;
    }
#endif
    return 0;
}
