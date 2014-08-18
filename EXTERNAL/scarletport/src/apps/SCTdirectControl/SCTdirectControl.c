
#include "SCTlibs.h"


int main(){
   int link;
   char buff[9];
   link = open("/dev/legousbtower0", O_RDONLY);
   while(1){
      // TODO: put this stuff into the lib.
      if(read(link,buff,9)!=-1){
        fprintf(stdout,"%d\n",SCT_parse_message(buff));
        //fprintf(stdout,"%d\n",*buff);
      }
      usleep(100000);
   }
   return(0);
}
