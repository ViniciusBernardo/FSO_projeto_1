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

void getMessage(int id_message, message *msg_shared, int num_messages){
  printf("\nFilho A recebendo mensagem e escrevendo na mem shared\n");

  message_buffer msg_buffer;

  message *msg_ptr = (message *)(msg_buffer.msg);

for(int i = 0; i < num_messages;i++){
  if( msgrcv(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message), 1, 0) == -1){
    fprintf(stderr, "Impossivel receber mensagem!\n");
    exit(1);
  }
  strcpy(msg_shared->text, msg_ptr->text);
  msg_shared->msg_num = i;
  msg_shared++;
}
}

void sendMessage(int id_message, char *input, int num_message){

  printf("\nPai A mandando lista de mensagens na fila\n");
  message_buffer msg_buffer;

  message *msg_ptr = (message *)(msg_buffer.msg);

  msg_buffer.type_msg = 1;
  msg_ptr->msg_num = num_message;
  strcpy(msg_ptr->text, input);
  if(msgsnd(id_message, (struct msgbuf *) &msg_buffer,
             sizeof(message),0) == -1){
    fprintf(stderr, "Impossivel enviar mensagem!\n");
    exit(1);
  }
}
int main(){

  int pid;
  int id_message;

  key_t random_key = 1546;

  if((id_message = msgget(random_key, IPC_CREAT | 0666)) == -1){
    printf("Um erro inesperado aconteceu.\n");
    exit(1);
  }

  //pid = fork();

    char input[100];
    int count_msg = 0;
    while(1){
      scanf("%s", input);
      if(strcmp(input, "sair") == 0) break;
      if((strcmp(input, "\n") != 0) || (strlen(input) > 1)){
         sendMessage(id_message, input, count_msg);
         count_msg++;
      }
    }

    pid = fork();

    if(pid == 0){
        int g_shm_id;
        message *msg_shared;

        g_shm_id = shmget(SHM_KEY, 10*sizeof(message), IPC_CREAT | 0666);
        msg_shared = (message *) shmat(g_shm_id, NULL, 0);

        getMessage(id_message, msg_shared, count_msg);
        exit(0);
    } else {
      wait(NULL);
    }
		
    //if( msgctl(id_message,IPC_RMID,NULL) != 0 ) {
    //  fprintf(stderr,"Impossivel remover a fila!\n");
    //  exit(1);
    //}
}
