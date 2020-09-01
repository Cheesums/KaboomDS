#ifndef INPUT_H
#define INPUT_H

#include <nds.h>
#include <stdio.h>
#include <nds/arm9/paddle.h>

class Paddle {
    protected:
    bool inserted_;
    int lastRead_ = paddleRead();
    int currentRead_ = paddleRead();
    int paddleDis_ = 0;

    public:
    int getPaddleDis();
    bool isInserted();
};

#endif