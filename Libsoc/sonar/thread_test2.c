#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
 
/*Rotina da thread 1*/
void * routine1(void *arg){
    int i;

    /*exibe o argumento recebido*/
    printf("Argumento: %s\n", (char *)arg);

    for(i=0; i <= 10; i++)
        printf("THREAD UM\n");
    
    /*finaliza a função retornando o argumento que foi recebido*/
    pthread_exit(arg);
}

/*Rotina da thread 2*/
void * routine2(void *arg){
    int i;

    /*exibe o argumento recebido*/
    printf("Argumento: %s\n", (char *)arg);

    for(i=0; i <= 10; i++)
        printf("THREAD DOIS\n");
    
    /*finaliza a função retornando o argumento que foi recebido*/
    pthread_exit(arg);
}

int main (int argc, char *argv[])
{
    pthread_t thread_1, thread_2;
    void *thread_res1, *thread_res2;
    int rstatus1, rstatus2;

    //====== Criando thread UM ==============================================================================================================================

    /*tenta iniciar o thread, indicando a função 'routine' e passando como argumento a string "Minha primeira Thread!"*/
    rstatus1 = pthread_create (&thread_1, NULL, routine1, (void*)("THREAD UM!"));
    
    /*verificar se ocorreu algum erro na chamada de pthread_create*/
    if(rstatus1 != 0)
    {
    printf ("Erro ao criar o thread UM.\n");
    exit(EXIT_FAILURE);
    }
    
    printf ("Thread UM criado com sucesso!\n");

    //====== Criando thread DOIS ==============================================================================================================================

    /*tenta iniciar o thread, indicando a função 'routine' e passando como argumento a string "Minha primeira Thread!"*/
    rstatus2 = pthread_create (&thread_2, NULL, routine2, (void*)("THREAD DOIS!"));
    
    /*verificar se ocorreu algum erro na chamada de pthread_create*/
    if(rstatus2 != 0)
    {
    printf ("Erro ao criar o thread DOIS.\n");
    exit(EXIT_FAILURE);
    }
    
    printf ("Thread DOIS criado com sucesso!\n");

    //====================================================================================================================================

    /*aguarda finalização do thread identificado por thread_id. O retorno é passado pelo ponteiro thread_res*/
    rstatus1 = pthread_join (thread_1, &thread_res1);

    /*aguarda finalização do thread identificado por thread_id. O retorno é passado pelo ponteiro thread_res*/
    rstatus2 = pthread_join (thread_2, &thread_res2);
    
    /*verificar se ocorreu algum erro na chamada de pthread_join*/
    if(rstatus1 != 0)
    {
    printf ("Erro ao aguardar finalização do thread UM.\n");
    exit(EXIT_FAILURE);
    }
    
    /*verificar se ocorreu algum erro na chamada de pthread_join*/
    if(rstatus2 != 0)
    {
    printf ("Erro ao aguardar finalização do thread DOIS.\n");
    exit(EXIT_FAILURE);
    }

    /*exibe o valor de retorno da função 'routine'*/
    printf ("Thread UM finalizado! Retorno = %s\n", (char *)thread_res1);

    /*exibe o valor de retorno da função 'routine'*/
    printf ("Thread DOIS finalizado! Retorno = %s\n", (char *)thread_res2);
    
    return 0;
}