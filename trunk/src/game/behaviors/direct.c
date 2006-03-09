#include "direct.h"
#include "sprite.h"

static int16 abs(int16 x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

static uint8 updateDirectFlat(Updateable* who, World* world)
{
    Direct* d = who->behavior;

    int16 dx = d->target.x - d->start.x;
    int16 dy = d->target.y - d->start.y;

    int8 stepsLeft = d->speed;

    int16 direction;
    if (dx > 0)
        direction = 1;
    else
        direction = -1;

    while (stepsLeft > 0)
    {
        --stepsLeft;
        who->position.x += (int16)direction;

        d->error += dy;
        if (2 * d->error > dx)
        {
            --stepsLeft;
            ++who->position.y;
            d->error -= dx;
        }

        if (dy > 0)
        {
            //
            // heading downscreen
            //
            if (spriteTop(who) > SCREEN_HEIGHT)
                return UPDATE_REMOVE;
        }
        else
        {
            //
            // heading upscreen
            //
            if (spriteBottom(who) < 0)
                return UPDATE_REMOVE;
        }

        if (direction > 0)
        {
            if (spriteLeft(who) > SCREEN_WIDTH)
                return UPDATE_REMOVE;
        }
        else
        {
            if (spriteRight(who) < 0)
                return UPDATE_REMOVE;
        }
    }

    return UPDATE_KEEP;
}

static uint8 updateDirectSteep(Updateable* who, World* world)
{
    Direct* d = who->behavior;

    int16 dx = d->target.x - d->start.x;
    int16 dy = d->target.y - d->start.y;

    int8 stepsLeft = d->speed;

    int16 direction;
    if (dx > 0)
        direction = 1;
    else
        direction = -1;

    while (stepsLeft > 0)
    {
        --stepsLeft;
        ++who->position.y;

        d->error += (int16)direction * dx;
        if (2 * d->error > dy)
        {
            --stepsLeft;
            who->position.x += (int16)direction;
            d->error -= dy;
        }

        if (dy > 0)
        {
            //
            // heading downscreen
            //
            if (spriteTop(who) > SCREEN_HEIGHT)
                return UPDATE_REMOVE;
        }
        else
        {
            //
            // heading upscreen
            //
            if (spriteBottom(who) < 0)
                return UPDATE_REMOVE;
        }

        if (direction > 0)
        {
            if (spriteLeft(who) > SCREEN_WIDTH)
                return UPDATE_REMOVE;
        }
        else
        {
            if (spriteRight(who) < 0)
                return UPDATE_REMOVE;
        }
    }

    return UPDATE_KEEP;
}

Direct* createDirect(Point start, Point target, uint8 speed)
{
    Direct* d = new(Direct);
    d->destroy = nullDestroy;

    if (abs(target.x - start.x) > abs(target.y - start.y))
        d->update = updateDirectFlat;
    else
        d->update = updateDirectSteep;

    d->start = start;
    d->target = target;
    d->error = 0;
    d->speed = speed;

    return d;
}
