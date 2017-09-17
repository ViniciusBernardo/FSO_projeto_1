#include <errno.h>              /* errno and error codes */
#include <sys/time.h>           /* for gettimeofday() */
#include <unistd.h>             /* for gettimeofday(), getpid() */
#include <stdio.h>              /* for printf() */
#include <unistd.h>             /* for fork() */
#include <sys/types.h>          /* for wait() */
#include <sys/wait.h>           /* for wait() */
#include <signal.h>             /* for kill(), sigsuspend(), others */
#include <sys/ipc.h>            /* for all IPC function calls */
#include <sys/shm.h>            /* for shmget(), shmat(), shmctl() */
#include <sys/msg.h>            /* for msgget(), msgctl() */
#include <string.h>
#include <stdlib.h>


typedef struct {
  unsigned int msg_num;
  char text[100];
} message;

typedef struct {
  long type_msg;
  char msg[sizeof(message)];
} message_buffer;

void getMessage(int id_message){
  printf("\nget Entrou\n");

  message_buffer msg_buffer;

  message *msg_ptr = (message *)(msg_buffer.msg);

for(int i = 0; i < 10;i++){
  if( msgrcv(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message), 1, 0) == -1){
    //printf("Um erro inesperado aconteceu!\n");
    fprintf(stderr, "Impossivel receber mensagem!\n");
  //  exit(1);
  }

  printf("%s", msg_ptr->text);
  printf("\nOla mundo\n");
}
}

void sendMessage(int id_message, char *input, int count_msg){

  printf("\nsend Entrou: %s\n", input);
  message_buffer msg_buffer;

  message *msg_ptr = (message *)(msg_buffer.msg);

for(int i = 0; i < 10;i++){
  msg_buffer.type_msg = 1;
  msg_ptr->msg_num = i;
  strcpy(msg_ptr->text, input);
  if(msgsnd(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message),0) == -1){
      fprintf(stderr, "Impossivel enviar mensagem!\n");
    //printf("Um erro inesperado aconteceu!\n");
    exit(1);
  }

  usleep(20);
}
}
int main(){

  int pid;
  int id_message;

  key_t random_key = 42;

  if((id_message = msgget(random_key, IPC_CREAT | 0666)) == -1){
    printf("Um erro inesperado aconteceu.\n");
    exit(1);
  }

  pid = fork();

  if(pid == 0){ /*Processo filho*/
    //usleep(10);

    printf("\nEntrou no filho\n");
    //while(1){
    char input[100];
    int count_msg = 0;
         count_msg++;
         sendMessage(id_message, "xablau55", count_msg);
		exit(0);
    //}

  } else { /*Processo Pai*/
	sleep(5);
    //while(strcmp(input, "sair") != 0){
      //scanf("%s", input);
      //if((strcmp(input, "\n") != 0) || (strlen(input) > 1)){
      getMessage(id_message);
      //}
    //}
		
/*    if( msgctl(id_message,IPC_RMID,NULL) != 0 ) {
      fprintf(stderr,"Impossivel remover a fila!\n");
      exit(1);
    }*/
  }
}
