#include "types.h"
#include "stat.h"
#include "user.h"
//#include "uproc.h"
int
main(int argc, char *argv[])
{
  printf(1,"Process statistics:\n");
  printf(1,"Name\tpid\tParent\tSize\tWaiting\tKilled\n");
  struct uproc a;  
 
  for(int i=0;i<64;i++){
    getprocinfo(&a);
    if(a.inuse[i]==1)
      printf(1,"%s\t%d\t%d\t%d\t%p\t%d\n",&(a.name[i][0]),a.pid[i],
             a.parent[i],a.sz[i],a.wait[i],a.killed[i]);
  }
  
  exit();
 }
