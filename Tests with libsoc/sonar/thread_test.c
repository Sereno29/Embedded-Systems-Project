#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
 
/*Rotina que será executada*/
void * routine(void *arg)
{
    /*exibe o argumento recebido*/
    printf("Argumento: %s\n", (char *)arg);
    
    /*finaliza a função retornando o argumento que foi recebido*/
    pthread_exit(arg);
}
 
int main (int argc, char *argv[])
{
    pthread_t thread_id;
    void * thread_res;
    int rstatus;
    
    /*tenta iniciar o thread, indicando a função 'routine' e passando como argumento a string "Minha primeira Thread!"*/
    rstatus = pthread_create (&thread_id, NULL, routine, (void*)("Minha primeira Thread!"));
    
    /*verificar se ocorreu algum erro na chamada de pthread_create*/
    if(rstatus != 0)
    {
    printf ("Erro ao criar o thread.\n");
    exit(EXIT_FAILURE);
    }
    
    printf ("Thread criado com sucesso!\n");
    
    /*aguarda finalização do thread identificado por thread_id. O retorno é passado pelo ponteiro thread_res*/
    rstatus = pthread_join (thread_id, &thread_res);
    
    /*verificar se ocorreu algum erro na chamada de pthread_join*/
    if(rstatus != 0)
    {
    printf ("Erro ao aguardar finalização do thread.\n");
    exit(EXIT_FAILURE);
    }
    
    /*exibe o valor de retorno da função 'routine'*/
    printf ("Thread finalizado! Retorno = %s\n", (char *)thread_res);
    
    return 0;
}