#include <stdio.h>
#include <uv.h>
#include "co.h"

co_define(idle, uv_idle_t *, co_none_t, int64_t);
void idle_co(co_t *co) {
  co_begin(idle, co, idler, count);
  *count = 0;
  do {
    uv_await(NULL, idle, *idler);
    printf("c=%lu\n", *count);
  } while (++*count < 10);
  co_end({});
}

typedef struct {
  uv_idle_t idler;
} idle_with_cleanup_state_t;
co_define(idle_with_cleanup, co_none_t, co_none_t, idle_with_cleanup_state_t);
void idle_with_cleanup_co(co_t *co) {
  co_begin(idle_with_cleanup, co, _, state);
  uv_idle_init(co->loop, &state->idler);
  co_await(NULL, idle, &state->idler);
  uv_await(NULL, close, (uv_handle_t *)&state->idler);
  co_end({});
}

int main() {
  __auto_type loop = uv_default_loop();
  printf("Idling...\n");
  co_launch(loop, NULL, NULL, idle_with_cleanup, {});
  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);
  return 0;
}
