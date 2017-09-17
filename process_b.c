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

#define SHM_KEY 0x5678

typedef struct {
  unsigned int msg_num;
  char text[100];
} message;

typedef struct {
  long type_msg;
  char msg[sizeof(message)];
} message_buffer;

void getMessage(int id_message){
  printf("\nPai B lendo mensagens da fila\n");

  message_buffer msg_buffer;

  message *msg_ptr = (message *)(msg_buffer.msg);

for(int i = 0; i < 10;i++){
  if( msgrcv(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message), 1, 0) == -1){
    fprintf(stderr, "Impossivel receber mensagem!\n");
    exit(1);
  }

  printf("\n%s", msg_ptr->text);
  printf("\n%d", msg_ptr->msg_num);
}
}

void sendMessage(int id_message, message *msg_shared){

  printf("\nFilho B lendo mem shared e mandando para o pai B\n");
  message_buffer msg_buffer;

  message *msg_ptr = (message *)(msg_buffer.msg);

for(int i = 0; i < 10;i++){
  msg_buffer.type_msg = 1;
  msg_ptr->msg_num = msg_shared->msg_num;
  strcpy(msg_ptr->text, msg_shared->text);
  msg_shared++;
  if(msgsnd(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message),0) == -1){
      fprintf(stderr, "Impossivel enviar mensagem!\n");
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
    int g_shm_id;
    message *msg_shared;

    g_shm_id = shmget(SHM_KEY, 10*sizeof(message), IPC_CREAT | 0666);
    msg_shared = (message *) shmat(g_shm_id, NULL, 0);

    //while(1){
    char input[100];
    int count_msg = 0;
         count_msg++;
         sendMessage(id_message, msg_shared);
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
