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

  if( msgrcv(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message), 1, 0) == -1){
    //printf("Um erro inesperado aconteceu!\n");
    fprintf(stderr, "Impossivel receber mensagem!\n");
    exit(1);
  }

  //printf("%s", msg_ptr->text);
  printf("\nOla mundo\n");
}

void sendMessage(int id_message, char *input, int count_msg){

  printf("\nsend Entrou: %s\n", input);
  message_buffer msg_buffer;

  message *msg_ptr = (message *)(msg_buffer.msg);

  msg_buffer.type_msg = 1;
  msg_ptr->msg_num = count_msg;
  strcpy(msg_ptr->text, input);

  if(msgsnd(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message),0) == -1){
      fprintf(stderr, "Impossivel enviar mensagem!\n");
    //printf("Um erro inesperado aconteceu!\n");
    exit(1);
  }

  usleep(50);
}

int main(){

  int pid;
  int id_message;

  key_t random_key = 1546;

  if((id_message = msgget(random_key, IPC_CREAT | 0666)) == -1){
    printf("Um erro inesperado aconteceu.\n");
    exit(1);
  }

  pid = fork();

  if(pid == 0){ /*Processo filho*/
    usleep(10);

    printf("\nEntrou no filho\n");
    //while(1){
      getMessage(id_message);
    //}

  } else { /*Processo Pai*/

    char input[100];
    int count_msg = 0;
    //while(strcmp(input, "sair") != 0){
      printf("\nEntre com alguma mensagem de no maximo 100 caracteres: ");
      //scanf("%s", input);
      //if((strcmp(input, "\n") != 0) || (strlen(input) > 1)){
         count_msg++;
         sendMessage(id_message, "xablau55", count_msg);
      //}
    //}

    if( msgctl(id_message,IPC_RMID,NULL) != 0 ) {
      fprintf(stderr,"Impossivel remover a fila!\n");
      exit(1);
    }
  }
}
