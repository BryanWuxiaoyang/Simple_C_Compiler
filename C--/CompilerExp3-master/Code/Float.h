#pragma once
#include <stdlib.h>

struct _Float_ {
	float value;
};
typedef struct _Float_* Float;

Float createFloat(float value) {
	Float float_ = (Float)malloc(sizeof(struct _Float_));
	if (float_) {
		float_->value = value;
	}
	return float_;
}