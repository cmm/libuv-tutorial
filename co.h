#pragma once

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <uv.h>

#ifndef co_printf
static __attribute__((format(printf, 1, 2), unused))
void co_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}
#endif

#ifndef container_of
#define container_of(ptr, type, member)                                        \
  ({                                                                           \
    const typeof(((type *)0)->member) *__mptr = (ptr);                         \
    (type *)((char *)__mptr - offsetof(type, member));                         \
  })
#endif

#define _co_count(...)                                                         \
  _co_count_(_, ##__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4,   \
             3, 2, 1, 0)
#define _co_count_(_, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,  \
                   _14, _15, _16, N, ...)                                      \
  N

#define _co_tn_fields_0()
#define _co_tn_fields_2(TYPE, NAME) TYPE NAME;
#define _co_tn_fields_4(TYPE, NAME, ...)                                       \
  TYPE NAME;                                                                   \
  _co_tn_fields_2(__VA_ARGS__)
#define _co_tn_fields_6(TYPE, NAME, ...)                                       \
  TYPE NAME;                                                                   \
  _co_tn_fields_4(__VA_ARGS__)
#define _co_tn_fields_8(TYPE, NAME, ...)                                       \
  TYPE NAME;                                                                   \
  _co_tn_fields_6(__VA_ARGS__)
#define _co_tn_fields_10(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_8(__VA_ARGS__)
#define _co_tn_fields_12(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_10(__VA_ARGS__)
#define _co_tn_fields_14(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_12(__VA_ARGS__)
#define _co_tn_fields_16(TYPE, NAME, ...)                                      \
  TYPE NAME;                                                                   \
  _co_tn_fields_14(__VA_ARGS__)
#define _co_tn_fields(N, ...) _co_tn_fields_(N, ##__VA_ARGS__)
#define _co_tn_fields_(N, ...) _co_tn_fields_##N(__VA_ARGS__)
#define _co_tn_struct_body(...)                                                \
  { _co_tn_fields(_co_count(__VA_ARGS__), ##__VA_ARGS__) }

#define _co_tn_args_0()
#define _co_tn_args_2(TYPE, NAME) TYPE NAME
#define _co_tn_args_4(TYPE, NAME, ...) TYPE NAME, _co_tn_args_2(__VA_ARGS__)
#define _co_tn_args_6(TYPE, NAME, ...) TYPE NAME, _co_tn_args_4(__VA_ARGS__)
#define _co_tn_args_8(TYPE, NAME, ...) TYPE NAME, _co_tn_args_6(__VA_ARGS__)
#define _co_tn_args_10(TYPE, NAME, ...) TYPE NAME, _co_tn_args_8(__VA_ARGS__)
#define _co_tn_args_12(TYPE, NAME, ...) TYPE NAME, _co_tn_args_10(__VA_ARGS__)
#define _co_tn_args_14(TYPE, NAME, ...) TYPE NAME, _co_tn_args_12(__VA_ARGS__)
#define _co_tn_args_16(TYPE, NAME, ...) TYPE NAME, _co_tn_args_14(__VA_ARGS__)
#define _co_tn_args(N, ...) _co_tn_args_(N, ##__VA_ARGS__)
#define _co_tn_args_(N, ...) _co_tn_args_##N(__VA_ARGS__)
#define _co_tn_arglist(...) (_co_tn_args(_co_count(__VA_ARGS__), ##__VA_ARGS__))

#define _co_tn_names_0()
#define _co_tn_names_2(_, NAME) NAME
#define _co_tn_names_4(_, NAME, ...) NAME, _co_tn_names_2(__VA_ARGS__)
#define _co_tn_names_6(_, NAME, ...) NAME, _co_tn_names_4(__VA_ARGS__)
#define _co_tn_names_8(_, NAME, ...) NAME, _co_tn_names_6(__VA_ARGS__)
#define _co_tn_names_10(_, NAME, ...) NAME, _co_tn_names_8(__VA_ARGS__)
#define _co_tn_names_12(_, NAME, ...) NAME, _co_tn_names_10(__VA_ARGS__)
#define _co_tn_names_14(_, NAME, ...) NAME, _co_tn_names_12(__VA_ARGS__)
#define _co_tn_names_16(_, NAME, ...) NAME, _co_tn_names_14(__VA_ARGS__)
#define _co_tn_names(N, ...) _co_tn_names_(N, ##__VA_ARGS__)
#define _co_tn_names_(N, ...) _co_tn_names_##N(__VA_ARGS__)
#define _co_tn_initform(...)                                                   \
  { _co_tn_names(_co_count(__VA_ARGS__), ##__VA_ARGS__) }
#define _co_tn_call_args(...)                                                  \
  (_co_tn_names(_co_count(__VA_ARGS__), ##__VA_ARGS__))

typedef struct {} co_none_t;

struct co;
typedef int (co_cancel_fn_t)(void *);

typedef struct {
  struct co *waiter;
  void *proc;
  co_cancel_fn_t *cancel_fn;
  bool ready;
  void *out;
} co_future_t;

// *** UV ***
#define _co_define_uv(TYPE, ...)                                               \
  _co_define_uv_(TYPE, _co_uv_non_cancellable, (void)0, (void)0, ##__VA_ARGS__)
#define _co_define_uv_cancellable(TYPE, ...)                                   \
  _co_define_uv_(TYPE, _co_uv_cancellable, (void)0, (void)0, ##__VA_ARGS__)
#define _co_define_uv_with_bells_on(TYPE, PRE, POST, ...)                      \
  _co_define_uv_(TYPE, _co_uv_non_cancellable, PRE, POST, ##__VA_ARGS__)

#define _co_uv_cancellable(FUTURE)                                             \
  do {                                                                         \
    FUTURE->cancel_fn = (co_cancel_fn_t *)uv_cancel;                           \
  } while (false)
#define _co_uv_non_cancellable(FUTURE)

#define _co_define_uv_(TYPE, SET_CANCEL_FN, PRE, POST, HorR_TYPE, HorR_NAME,   \
                       ...)                                                    \
  typedef struct _co_tn_struct_body(HorR_TYPE, HorR_NAME, ##__VA_ARGS__)       \
      uv_##TYPE##_out_t
#include "co_uv.h"
#undef _co_define_uv_

// we expect uv calls to take loop first & cb last, and to return int
#define _co_uv_wrapper(NAME)                                                   \
  static inline __attribute__((unused)) int _co_uv__##NAME
#define _co_uv_sans_loop(NAME, ...)                                            \
  _co_uv_wrapper(NAME) _co_tn_arglist(uv_loop_t *,, ##__VA_ARGS__) {           \
    return uv_##NAME _co_tn_call_args(__VA_ARGS__);                            \
  }

#define uv_await(OUT, CALL, ...)                                               \
  _uv_await(OUT, CALL, _co_uv_type__##CALL, ##__VA_ARGS__)
#define _uv_await(OUT, CALL, TYPE, ...)                                        \
  _uv_await_(OUT, CALL, TYPE, ##__VA_ARGS__)
#define _uv_await_(OUT, CALL, TYPE, HANDLE_OR_REQ, ...)                        \
  do {                                                                         \
    __auto_type _co_h_or_r = HANDLE_OR_REQ;                                    \
    _co_await_prep(_co_b, OUT, &&_co_label(__LINE__));                         \
    uv_##TYPE##__future_init(&_co_b->nested_future, _co_h_or_r);               \
    _co_h_or_r->data = &_co_b->nested_future;                                  \
    co_status = _co_uv__##CALL(_co_b->loop, _co_h_or_r, ##__VA_ARGS__,         \
                               uv_##TYPE##__cb);                               \
    if (co_status == 0) {                                                      \
      /* all good, uv will call us back */                                     \
      _co_return_guard.respectful = true;                                      \
      return;                                                                  \
    }                                                                          \
  _co_label(__LINE__):;                                                        \
  } while (false)

typedef void (co_fn_t)(struct co *);

typedef struct {
  const char *name;
  co_fn_t * const fn;
} _co_descriptor_t;

typedef struct co {
  const _co_descriptor_t *descriptor;
  co_future_t *future;
  uv_loop_t *loop;
  bool cancelled;
  void *label;
  co_future_t nested_future;
  union {
    uv_buf_t buf;
  } stash;
} co_t;

#define _co_define_uv_(TYPE, SET_CANCEL_FN, PRE, POST, HorR_TYPE, HorR_NAME,   \
                       ...)                                                    \
  static inline __attribute__((unused)) void uv_##TYPE##__future_init(         \
      co_future_t *future, void *req) {                                        \
    SET_CANCEL_FN(future);                                                     \
    future->proc = req;                                                        \
  }                                                                            \
  static void __attribute__((unused)) uv_##TYPE##__cb _co_tn_arglist(          \
      HorR_TYPE, HorR_NAME, ##__VA_ARGS__) {                                   \
    __auto_type future_ = (co_future_t *)HorR_NAME->data;                      \
    future_->proc = future_;                                                   \
    if (future_->out) {                                                        \
      *(uv_##TYPE##_out_t *)future_->out =                                     \
          (uv_##TYPE##_out_t)_co_tn_initform(_, HorR_NAME, ##__VA_ARGS__);     \
    }                                                                          \
    future_->ready = true;                                                     \
    future_->proc = NULL;                                                      \
    PRE;                                                                       \
    future_->waiter->descriptor->fn(future_->waiter);                          \
    POST;                                                                      \
  }
#include "co_uv.h"
#undef _co_define_uv_

static int _co_cancel(void *);
static void __attribute__((unused))
_co_init(co_t *co, co_future_t *future, void *out, uv_loop_t *loop) {
  co->future = future;
  co->loop = loop;
  co->cancelled = false;
  co->label = NULL;
  co->nested_future = (co_future_t){};
  if (future) {
    future->proc = co;
    future->cancel_fn = _co_cancel;
    future->out = out;
  }
}

static inline __attribute__((unused))
void co_cancel(co_future_t *future) {
  if (!future || !future->proc)
    return;
  if (future->cancel_fn)
    (void)future->cancel_fn(future->proc);
  future->proc = NULL;
}

static __attribute__((unused))
int _co_cancel(void *co_) {
  __auto_type co = (co_t *)co_;
  if (co->cancelled)
    return 0;
  co_cancel(&co->nested_future);
  co->cancelled = true;
  co->label = NULL;
  return 0;
}

#ifndef co_malloc
static __attribute__((unused, malloc))
void *co_malloc(size_t size) {
  void *ret = malloc(size);
  if (!ret) {
    co_printf("malloc(%lu) failed\n", size);
    abort();
  }
  return ret;
}
#endif
#ifndef co_realloc
static __attribute__((unused, warn_unused_result))
void *co_realloc(void *p, size_t size) {
  void *ret = realloc(p, size);
  if (!ret) {
    co_printf("realloc(%p, %lu) failed\n", p, size);
    abort();
  }
  return ret;
}
#endif

#define co_declare(NAME, IN_TYPE, OUT_TYPE)                                    \
  _co_declare(extern, extern const _co_descriptor_t NAME##__descriptor, NAME,  \
              IN_TYPE, OUT_TYPE)
#define _co_declare(LINKAGE, DESCRIPTOR_DECL, NAME, IN_TYPE, OUT_TYPE)         \
  typedef IN_TYPE NAME##__in_t;                                                \
  typedef OUT_TYPE NAME##_out_t;                                               \
  typedef struct {                                                             \
    co_t base;                                                                 \
    NAME##__in_t in;                                                           \
  } NAME##__public_t;                                                          \
  LINKAGE NAME##__public_t *NAME##__new(void);                                 \
  LINKAGE void NAME##_co(co_t *);                                              \
  DESCRIPTOR_DECL;                                                             \
  static void __attribute__((unused))                                          \
  NAME##__launch(uv_loop_t *loop, co_future_t *future, void *out,              \
                 IN_TYPE in) {                                                 \
    NAME##__public_t *_co = NAME##__new();                                     \
    _co_init(&_co->base, future, out, loop);                                   \
    _co->in = in;                                                              \
    _co->base.descriptor->fn(&_co->base);                                      \
  }

#define co_implement(NAME, STATE_TYPE)                                         \
  typedef STATE_TYPE NAME##__state_t;                                          \
  typedef struct {                                                             \
    NAME##__public_t public;                                                   \
    NAME##__state_t state;                                                     \
  } NAME##__private_t;                                                         \
  const _co_descriptor_t NAME##__descriptor = {.name = #NAME,                  \
                                               .fn = NAME##_co};               \
  NAME##__public_t *NAME##__new(void) {                                        \
    NAME##__private_t *_co = co_malloc(sizeof(NAME##__private_t));             \
    _co->public.base.descriptor = &NAME##__descriptor;                         \
    return &_co->public;                                                       \
  }

#define co_define(NAME, IN_TYPE, OUT_TYPE, STATE_TYPE)                         \
  _co_declare(static, , NAME, IN_TYPE, OUT_TYPE);                              \
  co_implement(NAME, STATE_TYPE);

typedef struct _co_respectful_return_guard {
  const char *func;
  bool respectful;
} _co_respectful_return_guard_t;

#define _co_destroy                                                            \
  do {                                                                         \
    free(_co);                                                                 \
    _co_return_guard.respectful = true;                                        \
  } while (false)

static void __attribute__((unused))
_co_check_respectful_return(_co_respectful_return_guard_t *returning) {
  if (!returning->respectful) {
    co_printf("%s returned, but it returned without respect\n",
              returning->func);
    abort();
  }
}

// FIXME
#define co_cleanup_begin
#define co_cleanup_end

#define co_return(NAME, OUT)                                                   \
  do {                                                                         \
    _co_check_descriptor(_co_b, &NAME##__descriptor,  __FILE__, __LINE__);     \
    _co_destroy;                                                               \
    if (_co_future) {                                                          \
      if (_co_future->out && sizeof(NAME##_out_t)) {                           \
        NAME##_out_t _co_out = OUT;                                            \
        *(NAME##_out_t *)_co_future->out = _co_out;                            \
      }                                                                        \
      _co_future->ready = true;                                                \
      _co_future->proc = NULL;                                                 \
      _co_future->waiter->descriptor->fn(_co_future->waiter);                  \
    }                                                                          \
    return;                                                                    \
  } while (false)

#define co_launch(LOOP, FUTURE, OUT, NAME, IN)                                 \
  do {                                                                         \
    NAME##__in_t in_ = IN;                                                     \
    NAME##__launch(LOOP, FUTURE, OUT, in_);                                    \
  } while (false)

static void __attribute__((unused))
_co_check_descriptor(co_t *co, const _co_descriptor_t *descriptor,
                     const char *file, int line) {
  if (co->descriptor != descriptor) {
    co_printf("%s:%d: the coroutine is %s, not %s\n", file, line,
              co->descriptor->name, descriptor->name);
    abort();
  }
}

#define co_bind(NAME, CO, IN_VAR, STATE_VAR)                                   \
  __auto_type __attribute__((unused)) _co_b = CO;                              \
  _co_check_descriptor(_co_b, &NAME##__descriptor,  __FILE__, __LINE__);       \
  __auto_type __attribute__((unused)) _co =                                    \
      container_of(_co_b, NAME##__private_t, public.base);                     \
  __auto_type __attribute__((unused)) _co_future = _co_b->future;              \
  __auto_type __attribute__((unused)) IN_VAR = &_co->public.in;                \
  __auto_type __attribute__((unused)) STATE_VAR = &_co->state

#define co_begin(...) co_begin_(_co_count(__VA_ARGS__), ##__VA_ARGS__)
#define co_begin_(N, ...) co_begin__(N, ##__VA_ARGS__)
#define co_begin__(N, ...) co_begin__##N(__VA_ARGS__)
#define co_begin__4(NAME, CO, IN_VAR, STATE_VAR)                               \
  co_begin___(NAME, CO, IN_VAR, STATE_VAR, _co_l_dummy_cleanup, {              \
    _co_l_dummy_cleanup:                                                       \
      co_cleanup_begin;                                                        \
      co_cleanup_end;                                                          \
  })
#define co_begin__5(NAME, CO, IN_VAR, STATE_VAR, CLEANUP_LABEL)                \
  co_begin___(NAME, CO, IN_VAR, STATE_VAR, CLEANUP_LABEL,)
#define co_begin___(NAME, CO, IN_VAR, STATE_VAR, CLEANUP_LABEL,                \
                    DUMMY_CLEANUP_CODE)                                        \
  co_bind(NAME, CO, IN_VAR, STATE_VAR);                                        \
  __attribute__((cleanup(_co_check_respectful_return)))                        \
  _co_respectful_return_guard_t _co_return_guard = {.func = __func__,          \
                                                    .respectful = false};      \
  int __attribute__((unused)) co_status = 0;                                   \
  if (_co_b->cancelled) {                                                      \
    _co_destroy;                                                               \
    return;                                                                    \
  }                                                                            \
  if (_co_b->label)                                                            \
    goto *_co_b->label;                                                        \
  DUMMY_CLEANUP_CODE;                                                          \
  {

#define co_end(NAME, OUT)                                                      \
  }                                                                            \
  co_return(NAME, OUT)

static void __attribute__((unused))
_co_await_prep(co_t *co, void *out, void *label) {
  co->nested_future.waiter = co;
  co->nested_future.ready = false;
  co->nested_future.out = out;
  co->label = label;
}

#define _co_label(X) _co_concat(_co_l, X)
#define _co_concat(X, Y) X##Y

#define co_await(OUT, NAME, IN)                                                \
  do {                                                                         \
    _co_await_prep(_co_b, OUT, &&_co_label(__LINE__));                         \
    co_launch(_co_b->loop, &_co_b->nested_future, &_co_b->nested_future.out,   \
              NAME, IN);                                                       \
    _co_return_guard.respectful = true;                                        \
    return;                                                                    \
  _co_label(__LINE__):;                                                        \
  } while (false)

static __attribute__((unused))
void _co_uv_get_stashed_buf(uv_handle_t *handle, size_t, uv_buf_t *buf) {
  __auto_type future = (co_future_t *)handle->data;
  *buf = future->waiter->stash.buf;
}

_co_uv_sans_loop(shutdown, uv_shutdown_t *, req, uv_stream_t *, handle,
                 uv_shutdown_cb, cb);
#define _co_uv_type__shutdown shutdown

_co_uv_wrapper(close)(uv_loop_t *, uv_handle_t *handle, uv_close_cb cb) {
  uv_close(handle, cb);
  return 0;
}
#define _co_uv_type__close close

_co_uv_sans_loop(listen, uv_stream_t *, stream, int, backlog, uv_connection_cb,
                 cb);
#define _co_uv_type__listen connection

// pretend there is a uv_read(), which is like uv_read_start() +
// automatic uv_read_stop() after the read happens and the callback
// fires (see the read callback definition above), so it can be sanely
// awaited.  it takes a buf instead of alloc_cb
static inline __attribute__((unused))
int _co_uv__read(uv_loop_t *, uv_stream_t *stream, uv_buf_t buf,
                 uv_read_cb cb) {
  __auto_type future = (co_future_t *)stream->data;
  future->waiter->stash.buf = buf;
  return uv_read_start(stream, _co_uv_get_stashed_buf, cb);
}
#define _co_uv_type__read read

_co_uv_sans_loop(write, uv_write_t *, req, uv_stream_t *, handle,
                 const uv_buf_t *, bufs, unsigned int, nbufs, uv_write_cb, cb);
_co_uv_sans_loop(write2, uv_write_t *, req, uv_stream_t *, handle,
                 const uv_buf_t *, bufs, unsigned int, nbufs, uv_stream_t *,
                 send_handle, uv_write_cb, cb);
#define _co_uv_type__write write
#define _co_uv_type__write2 write

_co_uv_sans_loop(tcp_close_reset, uv_tcp_t *, handle, uv_close_cb, cb);
#define _co_uv_type__close_reset close

_co_uv_sans_loop(tcp_connect, uv_connect_t *, req, uv_tcp_t *, handle,
                 const struct sockaddr *, addr, uv_connect_cb, cb);
#define _co_uv_type__tcp_connect connect

_co_uv_sans_loop(udp_send, uv_udp_send_t *, req, uv_udp_t *, handle,
                 const uv_buf_t *, bufs, unsigned int, nbufs,
                 const struct sockaddr *, addr, uv_udp_send_cb, cb);
#define _co_uv_type__udp_send udp_send

// pretend there is a uv_udp_recv(), which is like uv_udp_recv_start()
// + automatic uv_udp_recv_stop() after the recv happens and the
// callback fires (see the udp_recv callback definition above), so it can
// be sanely awaited.  it takes a buf instead of alloc_cb
static inline __attribute__((unused))
int _co_uv__udp_recv(uv_loop_t *, uv_udp_t *handle, uv_buf_t buf,
                     uv_udp_recv_cb cb) {
  __auto_type future = (co_future_t *)handle->data;
  future->waiter->stash.buf = buf;
  return uv_udp_recv_start(handle, _co_uv_get_stashed_buf, cb);
}
#define _co_uv_type__udp_recv udp_recv

_co_uv_wrapper(pipe_connect)(uv_loop_t *, uv_connect_t * req,
                             uv_pipe_t *handle, const char *name,
                             uv_connect_cb cb) {
  uv_pipe_connect(req, handle, name, cb);
  return 0;
}
#define _co_uv_type__pipe_connect connect

// FIXME not wrapping poll because 1. its use is specialized and is
// not really in our scope 2. it is not clear whether events are
// missed while a poll handle is stopped (edge-triggered events may
// be).

// pretend there is a uv_prepare(), which is like uv_prepare_start() +
// automatic uv_prepare_stop() after the callback fires (see the
// prepare callback definition above), so it can be sanely awaited.
static inline __attribute__((unused))
int _co_uv__prepare(uv_loop_t *, uv_prepare_t *prepare, uv_prepare_cb cb) {
  return uv_prepare_start(prepare, cb);
}
#define _co_uv_type__prepare prepare

// pretend there is a uv_check(), which is like uv_check_start() +
// automatic uv_check_stop() after the callback fires (see the
// check callback definition above), so it can be sanely awaited.
static inline __attribute__((unused))
int _co_uv__check(uv_loop_t *, uv_check_t *check, uv_check_cb cb) {
  return uv_check_start(check, cb);
}
#define _co_uv_type__check check

// pretend there is a uv_idle(), which is like uv_idle_start() +
// automatic uv_idle_stop() after the callback fires (see the
// idle callback definition above), so it can be sanely awaited.
static inline __attribute__((unused))
int _co_uv__idle(uv_loop_t *, uv_idle_t *idle, uv_idle_cb cb) {
  return uv_idle_start(idle, cb);
}
#define _co_uv_type__idle idle

#define _co_uv__async_init uv_async_init
#define _co_uv_type__async_init async

_co_uv_wrapper(timer)(uv_loop_t *, uv_timer_t *handle, uint64_t timeout,
                      uv_timer_cb cb) {
  // repeating timers don't make sense with coroutines
  return uv_timer_start(handle, cb, timeout, 0);
}
#define _co_uv_type__timer timer

_co_uv_wrapper(getaddrinfo)(uv_loop_t *loop, uv_getaddrinfo_t *req,
                            const char *node, const char *service,
                            const struct addrinfo *hints,
                            uv_getaddrinfo_cb cb) {
  return uv_getaddrinfo(loop, req, cb, node, service, hints);
}
#define _co_uv_type__getaddrinfo getaddrinfo

_co_uv_wrapper(getnameinfo)(uv_loop_t *loop, uv_getnameinfo_t *req,
                            const struct sockaddr *addr, int flags,
                            uv_getnameinfo_cb cb) {
  return uv_getnameinfo(loop, req, cb, addr, flags);
}
#define _co_uv_type__getnameinfo getnameinfo

#define _co_uv__queue_work uv_queue_work
#define _co_uv_type__queue_work after_work

#define _co_uv__fs_close uv_fs_close
#define _co_uv_type__fs_close fs
#define _co_uv__fs_open uv_fs_open
#define _co_uv_type__fs_open fs
#define _co_uv__fs_read uv_fs_read
#define _co_uv_type__fs_read fs
#define _co_uv__fs_write uv_fs_write
#define _co_uv_type__fs_write fs
#define _co_uv__fs_unlink uv_fs_unlink
#define _co_uv_type__fs_unlink fs
#define _co_uv__fs_copyfile uv_fs_copyfile
#define _co_uv_type__fs_copyfile fs
#define _co_uv__fs_mkdir uv_fs_mkdir
#define _co_uv_type__fs_mkdir fs
#define _co_uv__fs_mkdtemp uv_fs_mkdtemp
#define _co_uv_type__fs_mkdtemp fs
#define _co_uv__fs_mkstemp uv_fs_mkstemp
#define _co_uv_type__fs_mkstemp fs
#define _co_uv__fs_rmdir uv_fs_rmdir
#define _co_uv_type__fs_rmdir fs
// FIXME no idea where's the asynchrony here:
#define _co_uv__fs_scandir uv_fs_scandir
#define _co_uv_type__fs_scandir fs
#define _co_uv__fs_opendir uv_fs_opendir
#define _co_uv_type__fs_opendir fs
#define _co_uv__fs_readdir uv_fs_readdir
#define _co_uv_type__fs_readdir fs
#define _co_uv__fs_closedir uv_fs_closedir
#define _co_uv_type__fs_closedir fs
#define _co_uv__fs_stat uv_fs_stat
#define _co_uv_type__fs_stat fs
#define _co_uv__fs_fstat uv_fs_fstat
#define _co_uv_type__fs_fstat fs
#define _co_uv__fs_rename uv_fs_rename
#define _co_uv_type__fs_rename fs
#define _co_uv__fs_fsync uv_fs_fsync
#define _co_uv_type__fs_fsync fs
#define _co_uv__fs_fdatasync uv_fs_fdatasync
#define _co_uv_type__fs_fdatasync fs
#define _co_uv__fs_ftruncate uv_fs_ftruncate
#define _co_uv_type__fs_ftruncate fs
#define _co_uv__fs_sendfile uv_fs_sendfile
#define _co_uv_type__fs_sendfile fs
#define _co_uv__fs_access uv_fs_access
#define _co_uv_type__fs_access fs
#define _co_uv__fs_chmod uv_fs_chmod
#define _co_uv_type__fs_chmod fs
#define _co_uv__fs_utime uv_fs_utime
#define _co_uv_type__fs_utime fs
#define _co_uv__fs_futime uv_fs_futime
#define _co_uv_type__fs_futime fs
#define _co_uv__fs_lutime uv_fs_lutime
#define _co_uv_type__fs_lutime fs
#define _co_uv__fs_lstat uv_fs_lstat
#define _co_uv_type__fs_lstat fs
#define _co_uv__fs_link uv_fs_link
#define _co_uv_type__fs_link fs
#define _co_uv__fs_symlink uv_fs_symlink
#define _co_uv_type__fs_symlink fs
#define _co_uv__fs_readlink uv_fs_readlink
#define _co_uv_type__fs_readlink fs
#define _co_uv__fs_realpath uv_fs_realpath
#define _co_uv_type__fs_realpath fs
#define _co_uv__fs_fchmod uv_fs_fchmod
#define _co_uv_type__fs_fchmod fs
#define _co_uv__fs_chown uv_fs_chown
#define _co_uv_type__fs_chown fs
#define _co_uv__fs_fchown uv_fs_fchown
#define _co_uv_type__fs_fchown fs
#define _co_uv__fs_lchown uv_fs_lchown
#define _co_uv_type__fs_lchown fs
#define _co_uv__fs_statfs uv_fs_statfs
#define _co_uv_type__fs_statfs fs

// FIXME not wrapping uv_fs_poll_* & uv_fs_event_*, because not clear
// whether changes are missed if you stop the callback (but they
// probably are).

// FIXME not wrapping uv_signal_*, because signals may be missed while
// the callback is stopped.

#define _co_uv__random uv_random
#define _co_uv_type__random random

