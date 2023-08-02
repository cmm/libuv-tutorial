#include <stdio.h>
#include <uv.h>
#include "co.h"

typedef struct {
  int64_t count;
} idle_state_t;
co_define(idle, uv_idle_t *, co_none_t, idle_state_t);
void idle_co(co_t *co) {
  co_begin(idle, co, idler, state);
  state->count = 0;
  do {
    uv_await0(idle, *idler);
    printf("c=%lu\n", state->count);
  } while (++state->count < 10);
  co_end({});
}

co_define(idle_with_cleanup, co_none_t, co_none_t, uv_idle_t);
void idle_with_cleanup_co(co_t *co) {
  co_begin(idle_with_cleanup, co, _, idler);
  uv_idle_init(co->loop, idler);
  co_await0(idle, idler);
  uv_await0(close, (uv_handle_t *)idler);
  co_end({});
}

int main() {
  __auto_type loop = uv_default_loop();
  printf("Idling...\n");
  co_launch(loop, NULL, idle_with_cleanup, {});
  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);
  return 0;
}
