# Exercícios Capítulo 2

## 1 - O que diferencia o núcleo do restante do sistema operacional?

Um S.O. é composto por diversos componentes, como **núcleo**, **código de inicialização**, **drivers** e **programas utilitários**. O núcleo (*kernel*) é o coração do sistema operacional, responsável pela gerência dos recursos do hardware usados pelas aplicações. Também implementa as principais abstrações utilizadas pelos aplicativos e programas utilitários.

Usualmente, o núcleo executa em um modo especial de operação do processador, denomidado **modo privilegiado** ou modo sistema. Os demais programas e aplicações executam em **modo usuário**.


## 2 -  Seria possível construir um sistema operacional seguro usando um processador que não tenha níveis de privilégio? Por quê?

Seria muito difícil construir um S.O. sem os níveis de privilégio, pois estes níveis garantem uma camada de proteção aos recursos de hardware e outras seções do sistema. Aplicações com pleno acesso ao hardware seriam um risco à segurança (acesso a arquivos e áreas de memória).

O nível usuário não possui acesso às instruções perigosas, como RESET - reiniciar processador e IN/OUT - acessar entrada/saída. Além disso, o núcleo pode configurar a MMU para criar área de memória exclusiva para casa aplicação. Isso isola erros, falhas e comportamentos indevidos.


## 3 - Os processadores da família x86 possuem dois bits para definir o nível de privilégio, resultando em 4 níveis distintos. A maioria dos sistemas operacionais para esses processadores usam somente os níveis extremos (0 e 3, ou 002 e 112). Haveria alguma utilidade para os níveis intermediários?

Pode ser útil em algumas situações, como em S.O.'s que executam processos de diferentes níveis de segurança em um único processador. Em tais sistemas, os níveis intermediários podem ser usados para fornecer um grau adicional de separação entre processos de diferentes níveis de segurança, melhorando a segurança geral do sistema. Porém, a maioria dos sistemas operacionais modernos não precisa dessa funcionalidade.

## 4 - Quais as diferenças entre interrupções, exceções e traps?

* `Interrupções`: desviam a execução por um evento **externo**, gerado por um periférico. Isso pode incluir entradas de dispositivos, como teclado, mouse, ou redes, que exigem uma resposta rápida.

* `Exceções`: desviam a execução por um evento **interno**, gerado por uma instrução executada pelo processador. Por exemplo, erro numérico, uma divisão por zero ou uma tentativa de acesso a uma área de memória não alocada.

* `Traps`: desviam a execução a **pedido do software**. São interrupções de software geradas por instruções especiais, como INT e INTO. Elas são usadas para implementar chamadas de sistema e depuração. Objetivo: invocar, a partir da aplicação, as rotinas oferecidas pelo núcleo para o acesso ao hardware e demais serviços do S.O.


## 5 - O comando em linguagem C fopen é uma chamada de sistema ou uma função de biblioteca? Por quê?

O comando `fopen` é uma função de biblioteca, pois a linguagem C não possui nenhum comando de entrada/saída, essas operações ocorrem mediante chamadas as funções da biblioteca C.

## 6 - A operação em modo usuário permite ao processador executar somente parte das instruções disponíveis em seu conjunto de instruções. Quais das seguintes operações não deveriam ser permitidas em nível usuário? Por quê?

```markdown
(a) Ler uma porta de entrada/saída: Essa operação pode ser usada para acessar recursos de hardware, o que pode levar a comportamentos imprevisíveis ou danificar o hardware. 
(b) Efetuar uma divisão inteira
(c) Escrever um valor em uma posição de memória
(d) Ajustar o valor do relógio do hardware: O ajuste do relógio do hardware é uma operação crítica que pode afetar a estabilidade do sistema.
(e) Ler o valor dos registradores do processador: Os valores dos registradores do processador contêm informações confidenciais do sistema, como o estado de execução do processo e informações de registro de endereço.
(f) Mascarar uma ou mais interrupções: As interrupções são uma parte crítica do sistema e são usadas para lidar com eventos assíncronos, como entrada/saída e erros de hardware. Mascarar as interrupções pode levar a comportamentos imprevisíveis e pode afetar a estabilidade do sistema.
```

As operações (a), (d), (e) e (f) não deveriam ser permitidas em nível usuário, pois elas apresentam riscos de segurança e podem causar danos ao sistema operacional e/ou a outros processos em execução. As demais operações são permitidas em nível usuário.

## 7 - Considerando um processo em um sistema operacional com proteção de memória entre o núcleo e as aplicações, indique quais das seguintes ações do processo teriam de ser realizadas através de chamadas de sistema, justificando suas respostas:

```markdown
(a) Ler o relógio de tempo real do hardware: O relógio de tempo real é uma parte crítica do sistema e deve ser acessado apenas pelo kernel do sistema operacional. 
(b) Enviar um pacote através da rede: O envio de pacotes através da rede requer acesso ao hardware de rede e deve ser restrito apenas ao kernel do sistema operacional.
(c) Calcular uma exponenciação.
(d) Preencher uma área de memória do processo com zeros: A alocação de memória para um processo é gerenciada pelo kernel do sistema operacional, e o acesso à memória deve ser restrito para garantir a integridade dos dados e a segurança do sistema.
(e) Remover um arquivo do disco: O acesso ao sistema de arquivos também deve ser restrito para garantir a integridade dos dados e a segurança do sistema.
```

As operações (a), (b), (d) e (e) devem ser realizadas através de chamadas de sistema, pois elas requerem acesso a recursos do sistema operacional, como o relógio de tempo real, a rede e o disco. As demais operações podem ser realizadas diretamente pelo processo.

## 8 - Coloque na ordem correta as ações abaixo, que ocorrem durante a execução da função printf("Hello world") por um processo (observe que nem todas as ações indicadas fazem parte da sequência).

```markdown
(a) A rotina de tratamento da interrupção de software é ativada dentro do núcleo.
(b) A função printf finaliza sua execução e devolve o controle ao código do processo.
(c) A função de biblioteca printf recebe e processa os parâmetros de entrada (a string “Hello world”).
(d) A função de biblioteca printf prepara os registradores para solicitar a chamada de sistema write()
(e) O disco rígido gera uma interrupção indicando a conclusão da operação.
(f) O escalonador escolhe o processo mais prioritário para execução.
(g) Uma interrupção de software é acionada.
(h) O processo chama a função printf da biblioteca C.
(i) A operação de escrita no terminal é efetuada ou agendada pela rotina de tratamento da interrupção.
(j) O controle volta para a função printf em modo usuário.
```

A sequência correta é: (h), (c), (d), (g), (a), (i), (j), (b).

As ações (e) e (f) não fazem parte da sequência.

# Atividades 

## 1 - O utilitário strace do UNIX permite observar a sequência de chamadas de sistema efetuadas por uma aplicação. Em um terminal, execute strace date para descobrir quais os arquivos abertos pela execução do utilitário date (que indica a data e hora correntes). Por que o utilitário date precisa fazer chamadas de sistema?

O utilitário date precisa fazer chamadas de sistema para obter informações de tempo e data do sistema operacional em que está sendo executado. O strace é útil para entender como os programas interagem com o sistema operacional e pode ajudar os desenvolvedores a depurar problemas e otimizar o desempenho do programa.

## 2 - O utilitário ltrace do UNIX permite observar a sequência de chamadas de biblioteca efetuadas por uma aplicação. Em um terminal, execute ltrace date para descobrir as funções de biblioteca chamadas pela execução do utilitário date (que indica a data e hora correntes). Pode ser observada alguma relação entre as chamadas de biblioteca e as chamadas de sistema observadas no item anterior?

Sim, as chamadas de biblioteca e as chamadas de sistema estão relacionadas, uma vez que as chamadas de biblioteca frequentemente envolvem o uso de chamadas de sistema para realizar suas tarefas. O uso do ltrace pode ajudar a identificar as funções específicas da biblioteca que um programa está usando e pode ajudar a entender melhor como o programa funciona e identificar problemas ou ineficiências em seu uso da biblioteca