#include <stdio.h>

#include "../source/box2d/include/box2d/b2_math.h"
#include "../source/box2d/include/box2d/b2_settings.h"

int main(int argc, char **argv)
{
    b2Vec2 vt1 = b2Vec2(6, 7);

    printf("%3.2f,%3.2f\n", vt1.x, vt1.y);

    vt1.Normalize();
    printf("%3.2f,%3.2f\n", vt1.x, vt1.y);

    vt1 *= 3;
    printf("%3.2f,%3.2f\n", vt1.x, vt1.y);

    vt1.Set(1, 0);
    b2Transform tr1 = b2Transform();
    tr1.Set(b2Vec2(0, 0), (b2_pi / 180) * 45);
    b2Vec2 vt2 = b2Mul(tr1, vt1);
    printf("%3.2f,%3.2f\n", vt2.x, vt2.y);

    {
        vt1.Set(1, 0);
        b2Transform tr1 = b2Transform();
        tr1.Set(b2Vec2(1, 2), (b2_pi / 180) * 45);
        b2Vec2 vt2 = b2Mul(tr1, vt1);
        printf("%3.2f,%3.2f\n", vt2.x, vt2.y);
    }

    return 0;
}