/**
 * \file selfctl.h
 * \author Potsawat Thinkanwatthana (potsawattkw@outlook.com)
 * \brief Monitoring and controlling the execution time of process(es), if a process reaches its limit then kill it.
 * \version 0.0.0
 * \date 2022-06-10
 * 
 */

#ifndef SELFCTL_H
#define SELFCTL_H

#ifdef __cplusplus
extern "C" {
#endif

#define PROGRAM_NAME "selfctl"
#define SOMETHING_WENT_WRONG 0

#if __STDC_VERSION__ < 199901L
#error "unsupported C standard"
#endif

#if (defined __GNU__ && __gnu_hurd__    \
     || defined __gnu_linux__           )
#define TRUE_PTIME 1
#else
#define TRUE_PTIME 0
#endif

#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>

#define FMODE_1S        1111111
#define FMODE_DIR       S_IFDIR                         /* 0040000 */
#define FMODE_FULL      S_IRWXU                         /* 0000700 */
#define FMODE_UREAD     S_IRUSR                         /* 0000400 */
#define FMODE_UWRIT     S_IWUSR                         /* 0000200 */
#define FMODE_UEXEC     S_IXUSR                         /* 0000100 */
#define MAX_THREAD 2
#define MAX_JOBNAME 2
#define N_SUPPORTED_DFT_SIG_HANDLING 2
#define N_SUPPORTED_CUS_SIG_HANDLING 0
#define PTHREAD_INIT (pthread_t)(MAX_THREAD + 1)

#if defined NDEBUG
#define TRACE(fmt, ...)
#else
#define TRACE(msg) fprintf (stderr, "%s:%s:%d: %s\n", __FILE__, __func__, __LINE__, msg)
#endif

typedef enum
{
    T_UNKNOWN_TYPE,
    T_BOOL,
    T_SHORT_INT,
    T_UNSIGNED_SHORT_INT,
    T_INT,
    T_UNSIGNED_INT,
    T_LONG_INT,
    T_UNSIGNED_LONG_INT,
    T_LONG_LONG_INT,
    T_UNSIGNED_LONG_LONG_INT,
    T_FLOAT,
    T_DOUBLE,
    T_LONG_DOUBLE,
    T_CHAR,
    T_SIGNED_CHAR,
    T_UNSIGNED_CHAR,
    T_INT_PTR_ARR,
    T_INT_PTR,
    T_CHAR_PTR_ARR,
    T_CHAR_PTR,
    T_VOID_PTR
}
self_type;

#define TYPE_OF(x) _Generic((x),                                                                \
                            _Bool:                     (self_type T_BOOL ),                     \
                            short int:                 (self_type T_SHORT_INT ),                \
                            unsigned short int:        (self_type T_UNSIGNED_SHORT_INT ),       \
                            int:                       (self_type T_INT ),                      \
                            unsigned int:              (self_type T_UNSIGNED_INT ),             \
                            long int:                  (self_type T_LONG_INT ),                 \
                            unsigned long int:         (self_type T_UNSIGNED_LONG_INT ),        \
                            long long int:             (self_type T_LONG_LONG_INT ),            \
                            unsigned long long int:    (self_type T_UNSIGNED_LONG_LONG_INT ),   \
                            float:                     (self_type T_FLOAT ),                    \
                            double:                    (self_type T_DOUBLE ),                   \
                            long double:               (self_type T_LONG_DOUBLE ),              \
                            char:                      (self_type T_CHAR ),                     \
                            signed char:               (self_type T_SIGNED_CHAR ),              \
                            unsigned char:             (self_type T_UNSIGNED_CHAR ),            \
                            int[]:                     (self_type T_INT_PTR_ARR ),              \
                            int *:                     (self_type T_INT_PTR ),                  \
                            char[]:                    (self_type T_CHAR_PTR_ARR ),             \
                            char *:                    (self_type T_CHAR_PTR ),                 \
                            void *:                    (self_type T_VOID_PTR ),                 \
                            default:                   (self_type T_UNKNOWN_TYPE )              )

/* >>> tmp */
#define MAX_JOBPRD_INMIN 12
#define MAX_JOBPRD_INSEC (MAX_JOBPRD_INMIN * 60)
#define MAX_JOBPRD_TICKS (MAX_JOBPRD_INSEC * CLOCKS_PER_SEC)
/* <<< tmp */

typedef void (*V_PF_VS)(void *);
typedef void *(*VS_PF_VS)(void *);

typedef struct
{
    const V_PF_VS (*my_default)[];
    VS_PF_VS (*my_custom)[];
}
sighandler_table;

typedef struct
{
    char name[MAX_JOBNAME];
    unsigned int id;
    volatile VS_PF_VS callbacks[]; /*  */
}
job_table;

typedef struct thread_table
{
    int creation_return;
    pthread_t id;
    struct thread_table *prev;
    struct thread_table *next;
}
thread_table;

/* DIAG */
extern FILE *program_log_file_ptr;

/* FILES */
/* int checkperm (char *path, const int mode = FMODE_FULL | FMODE_DIR | FMODE_UREAD | FMODE_UWRIT | FMODE_UEXEC); */
int checkperm (char *, const int);


/* JOBS */

/* int gen_args_table(void *arg, ...); */

void init_job_table (pthread_t *);

int auto_create_thread (thread_table *);

unsigned int hash (int);

int timemgr (const clock_t);

void *main_job (int, char **);

void prep_exit (const int, void *) __attribute__((noreturn));


/* SIGNALS */

void *sighandler (int);

extern V_PF_VS my_default_sighandlers[N_SUPPORTED_DFT_SIG_HANDLING];

#ifdef __cplusplus
}
#endif

#endif /* SELFCTL_H */
