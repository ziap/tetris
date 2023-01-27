#ifndef EXPORTS_H
#define EXPORTS_H

#include "key.h"
#define export __attribute__((visibility("default")))

export void press(Key);
export void release(Key);

export void app_init();
export void app_update(float);

export void resize(int, int);
export void update_mouse(float, float);

#endif
