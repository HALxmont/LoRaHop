

#include <string.h>
#include <stdio.h>

int8_t rx_buffer[] = "$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F";

int main(void)
{

const char delim[2] = ",";
char *token = NULL;
uint8_t tokenIndex = 0;
char t1[10];
char t2[10];
unsigned char hour, min, sec;

    if(!strncmp(rx_buffer, "$GPGGA", 6)){
        /* get the first token */
        token = strtok(rx_buffer, delim);
        
    /* walk through other tokens */
    while( token != NULL ) {
        token = strtok(NULL, delim);
        tokenIndex++;
        switch(tokenIndex){
            case 1: t1[0] = token[0];
                    t1[1] = token[1];
                    t1[2] = '\0';
                    hour = atoi(t1);
                    printf("tok = %s, tmp = %s, h = %d", token, t1, h);
            
            
            break;

            case 2: 
            
            break;

        }
   } 
  
 
}








