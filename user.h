struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int clone(void*);

// ulib.c
int stat(char*, struct stat*);
char* strcpy(char*, char*);
void *memmove(void*, void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, char*, ...);
char* gets(char*, int max);
uint strlen(char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

//thread.c
int thread_create(void(*)(void*), void *);
void spinlock_init(sploc *lock);
void spinlock_acquire(sploc *lock);
void spinlock_release(sploc *lock);
void arrlock_init(arrloc *lock);
void arrlock_acquire(arrloc *lock, int i);
void arrlock_release(arrloc *lock, int i, int num);
void seqlock_init(seqloc *lock);
void seqlock_acquire(seqloc *lock);
void seqlock_release(seqloc *lock);
