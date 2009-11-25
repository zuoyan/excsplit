#ifndef _FILE_DEFS_H_
#define _FILE_DEFS_H_ 1
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>

#ifdef LOG_MALLOC_FREE
# define XMALLOC_INFO(p, s)                       \
  fprintf(stderr, "malloc %p size=%zd, @%s:%d\n", \
          p, (size_t)s, __FILE__, __LINE__)
# define XREALLOC_INFO(q, p, s)                           \
  fprintf(stderr, "realloc %p to %p, size=%zd, @%s:%d\n", \
          p, q, s, __FILE__, __LINE__)
# define XCALLOC_INFO(p, is, n)                         \
  fprintf(stderr, "calloc %p, size=%zx, @%s:%d\n",  \
          p, is, __FILE__, __LINE__)
# define XFREE_INFO(p) fprintf(stderr, "free %p @%s:%d\n",  \
                               p, __FILE__, __LINE__)
#else
# define XMALLOC_INFO(p, s)
# define XREALLOC_INFO(q, p, s)
# define XCALLOC_INFO(p, is, n)
# define XFREE_INFO(p)
#endif

#ifdef NO_XMALLOC
# define xmalloc malloc
# define xrealloc realloc
# define xcalloc calloc
# define xfree free
#else

# define xmalloc(s) ({                                                  \
      size_t sz__ = (s); void *p__ = malloc(sz__);                      \
      if (p__ == NULL && sz__ != 0) {                                   \
        fprintf(stderr, "malloc failed @" __FILE__ ":%d\n", __LINE__);  \
        XMALLOC_INFO(p__, sz__);                                        \
        exit(13);                                                       \
      }                                                                 \
      XMALLOC_INFO(p__, sz__);                                          \
      p__;})

# define xfree(p) ({                                                    \
      void *p__ = (p);                                                  \
      XFREE_INFO(p__);                                                  \
      free(p__);                                                        \
    })

# define xrealloc(p, s) ({                                              \
      size_t sz__ = (s);                                                \
      void *p__ = (p);                                                  \
      void *q__ = realloc(p__, sz__);                                   \
      if (q__ == NULL && sz__ != 0) {                                   \
        fprintf(stderr, "realloc failed @" __FILE__ ":%d\n", __LINE__); \
        XREALLOC_INFO(q__, p__, sz__);                                  \
        exit(13);                                                       \
      }                                                                 \
      XREALLOC_INFO(q__, p__, sz__);                                    \
      q__;                                                              \
    })

# define xcalloc(n, is)({                                               \
      size_t is__ = (is), n__ = (n);                                    \
      void *p__ = calloc(n__, is__);                                    \
      if (p__ == NULL) {                                                \
        fprintf(stderr, "calloc failed @" __FILE__ ":%d\n", __LINE__);  \
        XCALLOC_INFO(p__, n__, is__);                                   \
        exit(13);                                                       \
      }                                                                 \
      XCALLOC_INFO(p__, n__, is__);                                     \
      p__;                                                              \
    })

#endif

#ifndef NDEBUG
# define mesg_assert(cond, format, ...) ({                              \
      if (!(cond)) {                                                    \
        fprintf(stderr, "\nerror:@%s:%d:%s," format "\n", __FILE__, __LINE__, \
                #cond, ##__VA_ARGS__);                                  \
        exit(13);                                                       \
      }                                                                 \
      (void)0;})
# define trace_line(format, ...) fprintf(stderr, "@%s:%d:" format "\n", __FILE__,  __LINE__, ##__VA_ARGS__)
# define trace_omp(format, ...)                                       \
  fprintf(stderr, "@%s:%d[omp %d/%d]:" format "\n",                 \
          __FILE__, __LINE__,                                           \
          omp_get_thread_num(), omp_get_num_threads(), ##__VA_ARGS__)
# define trace_mpi(format, ...)                                       \
  fprintf(stderr, "@%s:%d[mpi %d/%d]:" format "\n",                     \
          __FILE__, __LINE__,                                           \
          me, np, ##__VA_ARGS__)
# define should_never_touch_here() fprintf(                \
      stderr, "\nshould never touch here: [%s:%s:%d]\n",   \
      __FILE__, __FUNCTION__, __LINE__)
#else
# define mesg_assert(cond, format, ...)
# define trace_line(format, ...)
# define trace_mpi(format, ...)
# define trace_omp(format, ...)
# define should_never_touch_here()
#endif

#ifndef offsetof
# define offsetof(TYPE, MEMBER)  __builtin_offsetof (TYPE, MEMBER)
#endif

#ifndef MAX
# define MAX(x, y)                              \
  ({                                            \
    typeof(x) x__ = (x);                        \
    typeof(y) y__ = (y);                        \
    x__ > y__ ? x__ : y__;                      \
  })
#endif

#ifndef MIN
# define MIN(x, y)                               \
  ({                                             \
    typeof(x) x__ = (x);                         \
    typeof(y) y__ = (y);                         \
    x__ < y__ ? x__ : y__;                       \
  })
#endif

#define CMP(x, y)                                \
  ({                                             \
    typeof(x) x__ = (x);                         \
    typeof(y) y__ = (y);                         \
    (x__ > y__) - (x__ < y__);                   \
  })

#define SWAP(x, y)                              \
  ({                                            \
    typeof(x) t__ = (x); (x) = (y); (y) = t__;  \
    (void) 0;                                   \
  })

#define SIGN(x) CMP(x, 0)

#define SAME_SIGN(x, y) (SIGN(x) == SIGN(y))

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define DIV_ROUND_UP(n, d)                      \
  ({                                            \
    typeof(d) d__ = (d);                        \
    ((n) + d__ - 1) / d__;                      \
  })

#define FLSL(n)                                             \
  ({                                                        \
    unsigned long n__ = (n);                                \
    n__ > 0 ? ((sizeof(n__)<<3) - __builtin_clzl(n__)) : 0; \
  })

#define FFSL(n) __builtin_ffsl((unsigned long)(n))

#define LLOG2(n) (FLSL(n) - 1)

#define BIT_MASK(nr) (1 << ((nr) & (8 - 1)))
#define BIT_BY_CHAR(nr) ((nr) >> 3)

static inline int bits_get(const char *base, ssize_t i) {
  return (base[BIT_BY_CHAR(i)] & BIT_MASK(i)) > 0;
}

#define BITS_GET(base, i)                                               \
  ({                                                                    \
    typeof(i) i__ = (i);                                                \
    ((char *)(base))[BIT_BY_CHAR(i__)] & BIT_MASK(i__);                    \
  })

static inline void bits_set(char *base, ssize_t i) {
  base[BIT_BY_CHAR(i)] |= BIT_MASK(i);
}

#define BITS_SET(base, i)                                               \
  ({                                                                    \
    typeof(i) i__ = (i);                                                \
    ((char *)(base))[BIT_BY_CHAR(i__)] |= BIT_MASK(i__);                   \
  })

static inline void bits_revert(char *base, ssize_t i) {
  base[BIT_BY_CHAR(i)] ^= BIT_MASK(i);
}

#define BITS_REVERT(base, i)                                            \
  ({                                                                    \
    typeof(i) i__ = (i);                                                \
    ((char *)(base))[BIT_BY_CHAR(i__)] ^= BIT_MASK(i__);           \
  })

static inline void bits_clear(char *base, ssize_t i) {
  base[BIT_BY_CHAR(i)] &= ~BIT_MASK(i);
}

#define BITS_CLEAR(base, i)                                             \
  ({                                                                    \
    typeof(i) i__ = (i);                                                \
    ((char *)(base))[BIT_BY_CHAR(i__)] &= ~BIT_MASK(i__);          \
  })

#ifdef HAVE_CLOCK_GETTIME
#define get_drtime()                            \
  ({                                            \
    struct timespec ts;                         \
    clock_gettime(CLOCK_REALTIME, &ts);         \
    ts.tv_sec + ts.tv_nsec / 1.e9;              \
  })
#else
#define get_drtime()                            \
  ({                                            \
    struct timeval tv;                          \
    gettimeofday(&tv, NULL);                    \
    tv.tv_sec + tv.tv_usec / 1.e6;              \
  })
#endif


#endif /*header file condition*/
