#include<wiringPi.h>
#include<stdio.h>
#include<wiringSerial.h>
#include<unistd.h>
#include<string.h>
unsigned char TOF_data[32] = {0}; 
unsigned char TOF_length = 16;
unsigned char TOF_header[3]={0x57,0x00,0xFF};
unsigned long TOF_system_time = 0;
unsigned long TOF_distance = 0;
unsigned char TOF_status = 0;
unsigned int TOF_signal = 0;
unsigned char TOF_check = 0;
int verifyCheckSum(unsigned char data[], unsigned char len);
int fd;
int i;
int flag=0;

int Init();

int main(){
    Init(); 
    while(1){ 
    
        usleep(200000);
        read(fd,TOF_data,32);
        
        for(int j=0;j<16;j++)
        {
          if( (TOF_data[j]==TOF_header[0] && TOF_data[j+1]==TOF_header[1] && TOF_data[j+2]==TOF_header[2]) && (verifyCheckSum(&TOF_data[j],TOF_length)))
          {
            if(((TOF_data[j+12]) | (TOF_data[j+13]<<8) )==0)
            {
               printf("Out of range!\n");
             }else{
               printf("TOF id is: ");
               printf("%d\n",TOF_data[j+3]);
        
               TOF_system_time = TOF_data[j+4] | TOF_data[j+5]<<8 | TOF_data[j+6]<<16 | TOF_data[j+7]<<24;
               printf("TOF system time is: ");
               printf("%d",TOF_system_time);
               printf("ms\n");
        
               TOF_distance = (TOF_data[j+8]) | (TOF_data[j+9]<<8) | (TOF_data[j+10]<<16);
               printf("TOF distance is: ");
               printf("%d",TOF_distance);
               printf("mm\n");
        
               TOF_status = TOF_data[j+11];
               printf("TOF status is: ");
               printf("%d\n",TOF_status);
        
               TOF_signal = TOF_data[j+12] | TOF_data[j+13]<<8;
               printf("TOF signal is: ");
               printf("%d\n",TOF_signal);
        
               printf("");
             
            }
            break;
          }
        } 
        
    }
  
   
    usleep(50*10000);
    serialClose(fd);
    return 0;
}

int Init(){
    if(wiringPiSetup() == -1){  
		printf("init pin failed\n");
		return 0;
	}
    
	fd = serialOpen("/dev/ttyS0",115200); //Open serial file
	if(fd == -1){
		printf("open serial failed\n");
		return -1;
	}

}
int verifyCheckSum(unsigned char data[], unsigned char len){
  TOF_check = 0;
  for(int k=0;k<len-1;k++)
  {
      TOF_check += data[k];
  }
  if(TOF_check == data[len-1])
  {
      printf("TOF data is ok!\n");
      return 1;    
  }else{
      printf("TOF data is error!");
      return 0;  
  }

}


