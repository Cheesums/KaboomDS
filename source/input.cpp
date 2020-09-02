#include "input.h"

bool Paddle::isInserted() {
  inserted_ = paddleIsInserted();
  return inserted_;
}

int Paddle::getPaddleDis() {
    currentRead_ = paddleRead();
    paddleDis_ = currentRead_ - lastRead_;
	lastRead_ = currentRead_;

    if (paddleDis_ > 3000)
	{
		paddleDis_ = paddleDis_ - 4095;
	} else if (paddleDis_ <-3000)
	{
		paddleDis_ = paddleDis_ + 4095;
	}

    return paddleDis_;
}

void Paddle::setZero() {
	lastRead_ = paddleRead();
}