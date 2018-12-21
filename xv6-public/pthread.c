#include "types.h"
#include "user.h"
#include "x86.h"
#define NULL 0
#define PGSIZE (4096)
struct thread_mutex{
  uint locked;
};
struct thread_cond{
  int threadId;
  struct thread_cond* next ;
};
void thread_mutex_init(struct thread_mutex *lk){
  lk->locked=0;
}
void thread_mutex_lock(struct thread_mutex *lk){
  while(xchg(&lk->locked, 1) != 0)
    sleep(1);
}
void thread_mutex_unlock(struct thread_mutex *lk){
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
}

void thread_cond_init(thread_cond *cond){
  cond->threadId = -1;
  cond->next = NULL;
  return;
}

void thread_cond_wait(thread_cond *cond, thread_mutex *mutex){
  thread_cond *mycond = cond;
  thread_mutex_unlock(mutex);
  
  //iterate the linked-list pointer to the end;
  while(mycond->next != NULL)
    mycond = mycond->next;  

  mycond->next = (thread_cond *)malloc(sizeof(thread_cond));
  mycond->next->threadId = getpid();
  mycond->next->next = NULL;
  sleep(1);
  thread_mutex_lock(mutex);
  return;
}

void thread_cond_signal(thread_cond *cond){
  thread_cond *mycond = cond;
  while(mycond->threadId==-1)
    mycond = mycond->next;
  if(mycond!=NULL){
    int threadId = mycond->threadId;
    thread_cond *garbage = mycond;
    cond->next = cond->next->next;
    free(garbage);
  }
  return;
}

struct q {
   struct thread_cond cv;
   struct thread_mutex m;
   void *ptr;
};

void* transmit_item;


// Thread 1 (sender)
void
send(void *arg)
{
  struct content*c=(struct content*)arg;
   struct q*q=(struct q*)c->q;
   void*p=(void*)c->p;
   thread_mutex_lock(&q->m);
   while(q->ptr != 0)
      ;
   q->ptr = p;
   thread_cond_signal(&q->cv);
   thread_mutex_unlock(&q->m);
}

// Thread 2 (receiver)

void
recv(void*arg)
{
  struct content*c=(struct content*)arg;
   struct q*q=(struct q*)c->q;
  void *p;

  thread_mutex_lock(&q->m);

  while((p = q->ptr) == 0)
    pthread_cond_wait(&q->cv, &q->m);
  q->ptr = 0;

  thread_mutex_unlock(&q->m);
  transmit_item=p;
  return ;
}
struct content{
  struct q*q;
  void* p;
};
int main(int argc, char *argv[]) {
  void* p="hello";
  struct q;
  &q->ptr=0;
  void *s1, *s2;
  // Initialize
  thread_cond_init(&q->cv);
  thread_mutex_init(&q->m);
  struct content c={&q,p};
  s1 = malloc(4096);
  s2 = malloc(4096);

  thread_create(recv, (void*)&c, s1);
  thread_create(send, (void*)&c, s2); 

  thread_join();
  thread_join();
  
  printf(1, "transmitted message is:%s",transmit_item);

  exit();
}
