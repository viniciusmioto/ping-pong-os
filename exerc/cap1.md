# Exercícios Capítulo 1

## 1 - Quais os dois principais objetivos de um sistema operacional?

Oferecer aos programas aplicativos uma forma de acesso homogênea aos dispositivos físicos, que permita abstrair sua complexidade e as diferenças tecnológicas entre eles.

* `ABSTRAÇÃO`: definir interfaces abstratas para os recursos do hardware, com os seguintes obejetivos: 
Prover interfaces de acesso aos dispositivos, mais simples de usar que as interfaces de baixo nível; 
Tornar os aplicativos independentes do hardware; 
Definir interfaces de acesso homogêneas para dispositivos com tecnologias distintas

* `GERÊNCIA`: Em um sistema com várias atividadessimultâneas, podem surgir conflitos no uso do hardware, quando dois ou mais aplicativos precisam dos mesmos recursos para poder executar. 
Cabe ao sistema operacional definir políticas para gerenciar o uso dos recursos de hardware pelos aplicativos, e resolver eventuais disputas e conflitos


Assim, um sistema operacional visa **abstrair** o acesso e **gerenciar** os recursos de hardware, provendo aos aplicativos um ambiente de execução abstrato, no qual o acesso aos recursos se faz através de interfaces simples, independentes das características e detalhes de baixo nível, e no qual os conflitos no uso do hardware são minimizados.

## 2 - Por que a abstração de recursos é importante para os desenvolvedores de aplicações? Ela tem alguma utilidade para os desenvolvedores do próprio sistema operacional?

### Desenvolvedores de Apps

Para os desenvolvedores de aplicações fica muito mais simples criar aplicações que utilizam os recursos de hardware. Com isso, o desenvolvedor não se precisa se preocupar com detalhes de baixo nível ao acessar o hardware.

### Desenvolvedores do S.O.

Para os desenvolvedores do S.O. a vantagem é a padronização na forma de acesso aos recursos de hardware. Dessa forma, o S.O. estará mais sólido para que os aplicativos de terceiro acessem os recursos necessários com as abstrações já desenvolvidas.

Além disso, a abstração permite que os desenvolvedores do S.O. criem camadas abstratas sobre o hardware subjacente para facilitar o desenvolvimento de drivers e outras partes do sistema operacional. Por exemplo, um driver de dispositivo pode usar uma API abstrata para interagir com um dispositivo de entrada, em vez de ter que lidar diretamente com a eletrônica do dispositivo e todos os detalhes técnicos de baixo nível. Isso torna o desenvolvimento do sistema operacional mais modular e facilita a manutenção e a atualização do sistema como um todo.

## 3 - A gerência de tarefas permite compartilhar o processador, executando mais de uma aplicação ao mesmo tempo. Identifique as principais vantagens trazidas por essa funcionalidade e os desafios a resolver para implementá-la.

### Vantagens

* Distribuição da capacidade de processamento de forma justa entre as aplicações, evitando que uma aplicação monopolize esse recurso e respeitando as prioridades definidas pelos usuários.

* Aumento da eficiência: Com a execução simultânea de várias tarefas, o processador é melhor utilizado, o que resulta em uma maior eficiência do sistema.

* Melhor uso dos recursos do sistema: permite que várias aplicações compartilhem os recursos do sistema, como memória, disco e dispositivos de entrada/saída, de forma eficiente e equilibrada.

* Maior produtividade: A possibilidade de executar várias tarefas simultaneamente permite que os usuários realizem mais coisas em menos tempo, aumentando a produtividade.

### Desafios

* Alocação de recursos: alocar recursos de forma eficiente entre as tarefas. Isso inclui gerenciar a alocação de memória, processador e dispositivos de entrada e saída.

* Escalonamento de processos: decidir qual tarefa executar em um determinado momento. Deve ser justo e garantir que as tarefas importantes recebam a atenção necessária.

* Concorrência: garantir que essas tarefas sejam executadas de forma segura e sem interferência umas nas outras.

* Sincronização: A execução simultânea de várias tarefas pode levar a problemas de sincronização, onde duas ou mais tarefas tentam acessar o mesmo recurso ao mesmo tempo. O sistema operacional deve fornecer mecanismos para sincronizar o acesso a recursos compartilhados.

## 4 - O que caracteriza um sistema operacional de tempo real? Quais as duas classificações de sistemas operacionais de tempo real e suas diferenças?

são sistemas nos quais o tempo é essencial. Um sistema operacional de tempo real não precisa ser necessariamente ultrarrápido; sua característica essencial é ter um comportamento temporal **previsível**, ou seja, seu tempo de resposta deve ser previsível no melhor e no pior caso de operação.

* `CRÍTICOS`: (*hard real-time systems*), nos quais a perda de um prazo pelo sistema pode perturbar seriamente o sistema físico sob seu controle, com graves consequências humanas, econômicas ou ambientais. Exemplos desse tipo de sistema seriam o controle de funcionamento de uma turbina de avião ou de um freio ABS.

* `NÃO CRÍTICOS`: (*soft real-time systems*) a perda de um prazo é perceptível e degrada o serviço prestado, sem maiores consequências. Exemplos desse tipo de sistema são os softwares de reprodução de mídia: em caso de atrasos, podem ocorrer falhas na música que está sendo tocada.

## 5 - Relacione as afirmações aos respectivos tipos de sistemas operacionais: distribuído (`D`), multi-usuário (`M`), desktop (`K`), servidor (`S`), embarcado (`E`) ou de tempo-real (`T`):

(`T`) Deve ter um comportamento temporal previsível, com prazos de resposta claramente definidos.

(`D`) Sistema operacional usado por uma empresa para executar seu banco de dados corporativo.

(`E`) São tipicamente usados em telefones celulares e sistemas eletrônicos dedicados.

(`D`) Neste tipo de sistema, a localização física dos recursos do sistema computacional é transparente para os usuários.

(`M`) Todos os recursos do sistema têm proprietários e existem regras controlando o acesso aos mesmos pelos usuários.

(`E`) A gerência de energia é muito importante neste tipo de sistema.

(`K`) Sistema que prioriza a gerência da interface gráfica e a interação com o usuário.

(`S`) Construído para gerenciar de forma eficiente grandes volumes de recursos.

(`K`) O MacOS X é um exemplo típico deste tipo de sistema.

(`E`) São sistemas operacionais compactos, construídos para executar aplicações específicas sobre plataformas com poucos recursos.

## 6 - Sobre as afirmações a seguir, relativas aos diversos tipos de sistemas operacionais, indique quais são incorretas, justificando sua resposta:

(`V`) Em um sistema operacional de tempo real, a rapidez de resposta é menos importante que a previsibilidade do tempo de resposta.

(`V`) Um sistema operacional multi-usuários associa um proprietário a cada recurso do sistema e gerencia as permissões de acesso a esses recursos.

(`F`) Nos sistemas operacionais de rede a localização dos recursos é transparente para os usuários.

* A localização dos recursos é transparente para os usuários no sistema **distribuído**. O sistema operacional de **rede** deve possuir suporte à operação em rede e capacidade de utilizar os recursos da rede (imporessoras, arquivos e outros computadores).

(`F`) Um sistema operacional de tempo real deve priorizar as tarefas que interagem com o usuário.

* As tarefas que interagem com o usuário são prioridades para um sistema operacional **desktop**. O sistema operacional de **tempo real** prioriza a previsibilidade do tempo de resposta de uma operação.

(`V`) Um sistema operacional embarcado é projetado para operar em hardware com poucos recursos.