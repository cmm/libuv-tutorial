#include <stdio.h>
#include <uv.h>
#include "co.h"

typedef struct {
  uv_idle_t idler;
  int64_t count;
} idle_state_t;
co(idle, co_future_t *, co_none_t, idle_state_t) {
  co_begin(parent_future, s);
  uv_idle_init(co_loop, &s->idler);
  s->count = 0;
  do {
    uv_await(NULL, idle, &s->idler);
    printf("c=%lu\n", s->count);
    if (s->count == 2)
      co_cancel(*parent_future);
  } while (++s->count < 10);

  co_end_with_deferred_cleanup({});
  printf("idle co cleaning up\n");
  uv_await(NULL, close, (uv_handle_t *)&s->idler);
  printf("idle co done cleaning up\n");
  co_cleanup_end;
}

co(idle_wrap, co_none_t, co_none_t, co_none_t) {
  co_begin(_, __);
  co_await(NULL, idle, (co_future_t *)_co_future);
  printf("idle co done\n");
  co_end_with_cleanup({});
  printf("idle_wrap would clean up if it needed to\n");
  co_cleanup_end;
}

int main() {
  __auto_type loop = uv_default_loop();
  printf("idling...\n");

  idle_wrap_future_t wrapper_future;
  co_future_init((co_future_t *)&wrapper_future, NULL, NULL, 0);

  co_launch(loop, &wrapper_future, idle_wrap, {});

  uv_run(loop, UV_RUN_DEFAULT);
  printf("cancelled: %d\n", wrapper_future.task == NULL);
  uv_loop_close(loop);
  return 0;
}
