
# Projeto 3 - Sistema de Controle de Tráfego com FreeRTOS

---

## Descrição Geral

    Este projeto implementa um sistema de controle de semáforos para quatro cruzamentos (A, B, C, D)
    utilizando o sistema operacional em tempo real FreeRTOS. O sistema simula o fluxo de veículos
    através desses cruzamentos, controlando os semáforos para garantir um tráfego seguro e eficiente.
    Cada cruzamento possui quatro semáforos que controlam as direções norte-sul e leste-oeste.

  ## Funcionalidades

    ### Múltiplas Tasks: O projeto cria tasks separadas para cada cruzamento e para os veículos. Essas
    tasks são responsáveis por controlar o estado dos semáforos e simular o movimento dos carros.
    ### Controle com Semáforos (FreeRTOS): São utilizados semáforos para gerenciar o acesso aos recursos
    compartilhados, garantindo que apenas um veículo atravesse o cruzamento de cada vez.
    ### Geração Aleatória de Veículos: Os veículos são gerados aleatoriamente, com diferentes velocidades
    e rotas pelos cruzamentos.
    ### Controle em Tempo Real: O sistema simula o comportamento em tempo real utilizando funcionalidades
    do FreeRTOS, como tasks, semáforos e atrasos (delays).
    ###Estados dos Semáforos: Cada semáforo pode ter três estados: verde, amarelo e vermelho. O sistema
    altera esses estados com base nas condições atuais de tráfego e nas posições dos veículos.

  
