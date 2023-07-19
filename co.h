#pragma once

// TODO uv cancellation (reqs are supposed to be cancellable)

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

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

#define _co_fields_0()
#define _co_fields_1(X) X;
#define _co_fields_2(X, ...) X; _co_fields_1(__VA_ARGS__)
#define _co_fields_3(X, ...) X; _co_fields_2(__VA_ARGS__)
#define _co_fields_4(X, ...) X; _co_fields_3(__VA_ARGS__)
#define _co_fields_5(X, ...) X; _co_fields_4(__VA_ARGS__)
#define _co_fields_6(X, ...) X; _co_fields_5(__VA_ARGS__)
#define _co_fields_7(X, ...) X; _co_fields_6(__VA_ARGS__)
#define _co_fields_8(X, ...) X; _co_fields_7(__VA_ARGS__)
#define _co_fields_9(X, ...) X; _co_fields_8(__VA_ARGS__)
#define _co_fields_10(X, ...) X; _co_fields_9(__VA_ARGS__)
#define _co_fields_11(X, ...) X; _co_fields_10(__VA_ARGS__)
#define _co_fields_12(X, ...) X; _co_fields_11(__VA_ARGS__)
#define _co_fields_13(X, ...) X; _co_fields_12(__VA_ARGS__)
#define _co_fields_14(X, ...) X; _co_fields_13(__VA_ARGS__)
#define _co_fields_15(X, ...) X; _co_fields_14(__VA_ARGS__)
#define _co_fields_16(X, ...) X; _co_fields_15(__VA_ARGS__)
#define _co_fields(N, ...) _co_fields_(N, ##__VA_ARGS__)
#define _co_fields_(N, ...) _co_fields_##N(__VA_ARGS__)
#define _co_struct_body(...)                                                   \
  { _co_fields(_co_count(__VA_ARGS__), ##__VA_ARGS__) }

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

struct co_base;
typedef void (co_fn_t)(struct co_base *);
typedef void (co_cleanup_fn_t)(struct co_base *);
typedef void (co_cancel_fn_t)(void *);

typedef struct co_base {
  void *promise;
  uv_loop_t *loop;
  co_fn_t *fn;
  int line;
  void *nested_promise;
  struct promise_base *np_base;
  co_cleanup_fn_t *cleanup_fn;
} co_base_t;

typedef struct promise_base {
  co_base_t *waiter;
  void *proc;
  co_cancel_fn_t *cancel_fn;
  bool ready;
} promise_base_t;

static void _co_cancel(void *);
static void __attribute__((unused))
_co_base_init(co_base_t *co, promise_base_t *promise, uv_loop_t *loop,
              co_fn_t *fn) {
  co->promise = promise;
  co->loop = loop;
  co->fn = fn;
  co->line = 0;
  co->nested_promise = NULL;
  co->np_base = NULL;
  if (promise) {
    promise->proc = co;
    promise->cancel_fn = _co_cancel;
  }
}

static inline __attribute__((unused))
void co_cancel(promise_base_t *promise) {
  if (!promise || !promise->proc)
    return;
  if (promise->cancel_fn)
    promise->cancel_fn(promise->proc);
  promise->proc = NULL;
}

static __attribute__((unused))
void _co_cancel(void *co_) {
  __auto_type co = (co_base_t *)co_;
  if (co->np_base)
    co_cancel(co->np_base);
  co->line = -1;
}

#define _co_declare(LINKAGE, NAME, IN_TYPE, OUT_TYPE)                          \
  typedef IN_TYPE NAME##__in_t;                                                \
  typedef OUT_TYPE NAME##__out_t;                                              \
  typedef struct {                                                             \
    promise_base_t base;                                                       \
    NAME##__out_t out;                                                         \
  } NAME##_promise_t;                                                          \
  typedef struct {                                                             \
    co_base_t base;                                                            \
    NAME##__in_t in;                                                           \
  } NAME##__public_t;                                                          \
  LINKAGE NAME##__public_t *NAME##__new(void);                                 \
  LINKAGE void NAME##_co(co_base_t *);                                         \
  static void __attribute__((unused))                                          \
  NAME##__launch(uv_loop_t *loop, promise_base_t *promise, IN_TYPE in) {       \
    NAME##__public_t *co = NAME##__new();                                      \
    _co_base_init(&co->base, promise, loop, NAME##_co);                        \
    co->in = in;                                                               \
    co->base.fn(&co->base);                                                    \
  }

#define co_declare(NAME, IN_TYPE, OUT_TYPE)                                    \
  _co_declare(extern, NAME, IN_TYPE, OUT_TYPE)

#define co_implement(NAME, STATE_TYPE, CLEANUP_FN)                             \
  typedef STATE_TYPE NAME##__state_t;                                          \
  typedef struct {                                                             \
    NAME##__public_t public;                                                   \
    NAME##__state_t state;                                                     \
  } NAME##__private_t;                                                         \
  NAME##__public_t *NAME##__new(void) {                                        \
    NAME##__private_t *co = malloc(sizeof(NAME##__private_t));                 \
    co->public.base.cleanup_fn = CLEANUP_FN;                                   \
    return &co->public;                                                        \
  }

#define co_define(NAME, IN_TYPE, OUT_TYPE, STATE_TYPE, CLEANUP_FN)             \
  _co_declare(static, NAME, IN_TYPE, OUT_TYPE);                                \
  co_implement(NAME, STATE_TYPE, CLEANUP_FN);

#define _co_destroy                                                            \
  do {                                                                         \
    free(_co->public.base.nested_promise);                                     \
    if (_co->public.base.cleanup_fn)                                           \
      _co->public.base.cleanup_fn(&_co->public.base);                          \
    free(_co);                                                                 \
  } while (false)

#define co_return(OUT)                                                         \
  do {                                                                         \
    _co_destroy;                                                               \
    if (_promise) {                                                            \
      typeof(_promise->out) out_ = OUT;                                        \
      _promise->out = out_;                                                    \
      _promise->base.ready = true;                                             \
      co_base_t *_waiter = _promise->base.waiter;                              \
      _waiter->fn(_waiter);                                                    \
    }                                                                          \
    return;                                                                    \
  } while (false)

#define co_launch(LOOP, PROMISE, NAME, IN)                                     \
  do {                                                                         \
    NAME##__in_t in_ = IN;                                                     \
    NAME##__launch(LOOP, PROMISE, in_);                                        \
  } while (false)

#define co_prologue(NAME, CO)                                                  \
  if (strcmp(__func__, #NAME "_co"))                                           \
    abort();                                                                   \
  __auto_type _co = container_of(CO, NAME##__private_t, public.base);          \
  __auto_type _promise =                                                       \
      container_of(_co->public.base.promise, NAME##_promise_t, base);          \
  __auto_type in = &_co->public.in;                                            \
  __auto_type state = &_co->state;                                             \
  int co_errno = 0;                                                            \
  switch (_co->public.base.line) {                                             \
  case -1:                                                                     \
    _co_destroy;                                                               \
    return;                                                                    \
  case 0:

#define co_epilogue(OUT)                                                       \
  }                                                                            \
  co_return(OUT)

#define co_await0(NAME, IN)                                                    \
  do {                                                                         \
    __auto_type _np = (NAME##__promise_t)realloc(                              \
        _co->public.base.nested_promise, sizeof(NAME##__promise_t));           \
    _co->public.base.nested_promise = _np;                                     \
    _co->public.base.np_base = &_np->base;                                     \
    _co->public.base.np_base.waiter = &_co->public.base;                       \
    _co->public.base.np_base.ready = false;                                    \
    _co->public.base.line = __LINE__;                                          \
    co_launch(_co->public.base.loop, _co->public.base.np_base, NAME, IN);      \
    return;                                                                    \
  case __LINE__:;                                                              \
  } while (false)

#define co_await(OUT_VAR, NAME, IN)                                            \
  co_await0(NAME, IN);                                                         \
  __auto_type OUT_VAR =                                                        \
      &container_of(_co->public.base.np_base, NAME##_promise_t, base)->out;

// *** UV ***
#define __co_define_uv(NAME, SET_CANCEL_FN, HorR_TYPE, HorR_NAME, ...)         \
  typedef struct {                                                             \
    promise_base_t base;                                                       \
    struct _co_tn_struct_body(HorR_TYPE, HorR_NAME, ##__VA_ARGS__) out;        \
  } uv_##NAME##__promise_t;                                                    \
  static inline __attribute__((unused))                                        \
  uv_##NAME##__promise_t *uv_##NAME##__promise_new(void *old, void *req) {     \
    __auto_type promise = (uv_##NAME##__promise_t *)realloc(                   \
        old, sizeof(uv_##NAME##__promise_t));                                  \
    SET_CANCEL_FN(promise);                                                    \
    promise->base.proc = req;                                                  \
    return promise;                                                            \
  }                                                                            \
  static void __attribute__((unused)) uv_##NAME##__cb _co_tn_arglist(          \
      HorR_TYPE, HorR_NAME, ##__VA_ARGS__) {                                   \
    __auto_type promise_ = (uv_##NAME##__promise_t *)HorR_NAME->data;          \
    promise_->base.proc = promise_;                                            \
    co_base_t *waiter_ = promise_->base.waiter;                                \
    promise_->out =                                                            \
        ((typeof(promise_->out))_co_tn_initform(_, HorR_NAME, ##__VA_ARGS__)); \
    promise_->base.ready = true;                                               \
    waiter_->fn(waiter_);                                                      \
  }

#define _co_uv_cancellable(PROMISE)                                            \
  do {                                                                         \
    PROMISE->base.cancel_fn = (co_cancel_fn_t *)uv_cancel;                     \
  } while (false)
#define _co_uv_non_cancellable(PROMISE)

#define _co_define_uv(NAME, ...)                                               \
  __co_define_uv(NAME, _co_uv_non_cancellable, ##__VA_ARGS__)
#define _co_define_cancellable_uv(NAME, ...)                                   \
  __co_define_uv(NAME, _co_uv_cancellable, ##__VA_ARGS__)

_co_define_uv(alloc, uv_handle_t *, handle, size_t, suggested_size, uv_buf_t,
              buf);
_co_define_uv(read, uv_stream_t *, stream, ssize_t, nread, const uv_buf_t *,
              buf);
_co_define_cancellable_uv(write, uv_write_t *, req, int, status);
_co_define_cancellable_uv(connect, uv_connect_t *, req, int, status);
_co_define_cancellable_uv(shutdown, uv_shutdown_t *, req, int, status);
_co_define_uv(connection, uv_stream_t *, server, int, status);
_co_define_uv(close, uv_handle_t *, handle);
_co_define_uv(poll, uv_poll_t *, handle, int, status, int, events);
_co_define_uv(timer, uv_timer_t *, handle);
_co_define_uv(async, uv_async_t *, handle);
_co_define_uv(prepare, uv_prepare_t *, handle);
_co_define_uv(check, uv_check_t *, handle);
_co_define_uv(idle_start, uv_idle_t *, handle);
_co_define_uv(exit, uv_process_t *, process, int64_t, exit_status, int,
              term_signal);
_co_define_uv(walk, uv_handle_t *, handle, void *, arg);
_co_define_cancellable_uv(fs, uv_fs_t *, req);
_co_define_cancellable_uv(work, uv_work_t *, req);
_co_define_uv(after_work, uv_work_t *, req, int, status);
_co_define_cancellable_uv(getaddrinfo, uv_getaddrinfo_t *, req, int, status,
                          struct addrinfo *, res);
_co_define_cancellable_uv(getnameinfo, uv_getnameinfo_t *, req, int, status,
                          const char *, hostname, const char *, service);
_co_define_cancellable_uv(random, uv_random_t *, req, int, status, void *, buf,
                          size_t, buflen);

#define uv_await0(NAME, HANDLE_OR_REQ, ...)                                    \
  do {                                                                         \
    __auto_type _handle_or_req = HANDLE_OR_REQ;                                \
    __auto_type _np = uv_##NAME##__promise_new(                                \
        _co->public.base.nested_promise, _handle_or_req);                      \
    _co->public.base.nested_promise = _np;                                     \
    _co->public.base.np_base = &_np->base;                                     \
    _np->base.waiter = &_co->public.base;                                      \
    _np->base.ready = false;                                                   \
    _handle_or_req->data = _np;                                                \
    _co->public.base.line = __LINE__;                                          \
    co_errno = uv_##NAME(HANDLE_OR_REQ, ##__VA_ARGS__, uv_##NAME##__cb);       \
    if (!co_errno)                                                             \
      return;                                                                  \
  case __LINE__:;                                                              \
  } while (false)

#define uv_await(OUT_VAR, NAME, ...)                                           \
  uv_await0(NAME, ##__VA_ARGS__);                                              \
  __auto_type OUT_VAR =                                                        \
      &container_of(_co->public.base.np_base, uv_##NAME##__promise_t, base)    \
           ->out;
