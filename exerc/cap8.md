# Exercícios Capítulo 8

## 1 - Quais são as vantagens e desvantagens das abordagens a seguir, sob as óticas do sistema operacional e do programador de aplicativos?

### (a) comunicação bloqueante ou não-bloqueante

#### Vantagens: Bloqueantes

* Menor sobrecarga de CPU, já que o processo ou *thread* é suspenso até que a operação de E/S seja concluída.

* Gerenciamento mais simples de recursos, já que apenas um processo ou *thread* pode acessar um recurso de cada vez.

#### Desantagens: Bloqueantes

* Pode levar a tempos de resposta mais lentos para o usuário final, especialmente em casos de operações de E/S lentas.

* Pode levar a problemas de bloqueio ou *deadlock*, em que vários processos ou *threads* ficam presos aguardando um recurso que nunca é liberado.

#### Vantagens: Não-bloqueantes

* Pode fornecer um melhor desempenho em geral, já que a CPU pode ser usada para executar outras instruções enquanto aguarda uma resposta de E/S.

* Menor risco de bloqueio ou deadlock, já que o processo ou thread pode continuar executando outras instruções enquanto espera por uma resposta.

#### Desvantagens: Não-bloqueantes

* Requer mais gerenciamento de estados e verificação de erros no código, já que a resposta da E/S não é garantida imediatamente. Desperdício de processamento no envios de mensagens e na tentativa de receber.

* Pode levar a um código mais complexo e difícil de manter devido ao uso de *callbacks* ou outras técnicas assíncronas.

### (b) canais com buffering ou sem buffering

#### Vantagens: Com buffering

* Podem melhorar o desempenho geral do sistema, já que o processo de envio não precisa esperar pelo processo de recebimento antes de continuar a execução.

* Permitem que os processos de envio e recebimento executem de forma assíncrona e independente, reduzindo a possibilidade de bloqueio ou deadlock.

#### Desvantagens: Com buffering

* A necessidade de gerenciar o tamanho do *buffer* e lidar com possíveis problemas de sobreposição de dados.

* Possibilidade de perda de dados, se o *buffer* ficar cheio antes que o processo de recebimento esteja pronto para consumir os dados.

#### Vantagens: Sem buffering

* Gerenciamento mais simples de recursos, já que a memória do *buffer* não precisa ser alocada.

* Menor risco de perda de dados, já que o processo de envio espera pelo processo de recebimento antes de enviar mais dados.

#### Desvantagens: Sem buffering

* Pode levar a tempos de resposta mais lentos para o usuário final, especialmente em casos de processos de recebimento lentos.

* A necessidade de gerenciar explicitamente a sincronização entre os processos de envio e recebimento, o que pode aumentar a complexidade do código.

### (c) comunicação por mensagens ou por fluxo

#### Vantagens: Por mensagens

* Menor risco de bloqueio ou *deadlock*, já que os processos de envio e recebimento podem executar de forma assíncrona e independente. A comunicação por mensagens permite que os processos ou threads troquem informações complexas em um formato estruturado e fácil de entender.

* Menor risco de perda de dados, já que os dados são armazenados em uma fila até que o processo de recebimento esteja pronto para consumi-los. Não é possível receber meia mensagem.

#### Desvantagens: Por mensagens

* Pode ser necessário escrever código adicional para lidar com a entrega de mensagens e garantir que elas cheguem na ordem correta.

* Pode haver um desempenho mais lento se o sistema tiver que lidar com muitas mensagens pequenas.

#### Vantagens: Por fluxo

* Os processos ou threads podem transmitir dados continuamente, sem a necessidade de quebrá-los em mensagens separadas.

* Pode ser mais eficiente em termos de desempenho se o sistema tiver que lidar com muitos dados grandes.

#### Desvantagens: Por fluxo

* Pode ser necessário escrever código adicional para garantir que os dados sejam transmitidos na ordem correta.

* Pode haver uma maior complexidade em lidar com a sincronização e os *buffers* de dados.


### (d) mensagens de tamanho fixo ou variável

#### Vantagens: Tamanho fixo

* A previsibilidade do tamanho da mensagem permite que o sistema gerencie mais facilmente o espaço necessário para o buffer e evite problemas de sobrecarga de memória.

* Pode ser mais fácil de implementar, já que o tamanho da mensagem é conhecido e pode ser usado para ajudar na validação e verificação.

#### Desvantagens: Tamanho fixo

* Pode ser necessário usar preenchimento para atingir o tamanho máximo, o que pode desperdiçar recursos e espaço em memória.

* Pode ser difícil transmitir dados que não se encaixam perfeitamente no tamanho da mensagem. Uma mensagem maior que o tamanho máximo deve ser dividida em mensagens menores.

#### Vantagens: Tamanho variável

* Pode acomodar uma ampla gama de dados e tamanhos de mensagem, tornando-o mais flexível em termos de uso.

* Pode ser mais eficiente em termos de uso de memória, já que apenas o espaço necessário para a mensagem precisa ser alocado.

#### Desvantagens: Tamanho variável

* A verificação e validação das mensagens podem ser mais complexas, pois o tamanho da mensagem não é pré-determinado.

* Pode haver um risco maior de sobrecarga de memória se as mensagens forem muito grandes e o sistema não tiver uma maneira eficiente de lidar com elas.

### (e) comunicação 1:1 ou M:N

#### Vantagens: 1:1

* É geralmente mais simples de implementar e gerenciar, pois há apenas um único processo ou thread de destino envolvido na comunicação.

* Pode ser mais eficiente em termos de desempenho, pois não há overhead adicional necessário para gerenciar múltiplos processos ou threads envolvidos na comunicação.

#### Desvantagens: 1:1

* Pode haver uma limitação na escalabilidade do sistema, já que apenas um único processo ou thread pode ser usado para comunicação.

* Pode ser difícil de implementar para sistemas distribuídos, onde há múltiplos nós de processamento.

#### Vantagens: M:N

* Pode acomodar um grande número de processos ou *threads* se comunicando, tornando-o mais escalável em sistemas distribuídos.

* Pode permitir que o sistema aproveite melhor o *hardware* de processamento disponível, já que vários processos ou *threads* podem ser usados para comunicação.

#### Desvantagens: M:N

* Pode ser mais complexo de implementar e gerenciar, já que há múltiplos processos ou *threads* envolvidos na comunicação.

* Pode haver um *overhead* adicional para gerenciar múltiplos processos ou *threads* envolvidos na comunicação, o que pode afetar o desempenho.

## 2 - Explique como processos que comunicam por troca de mensagens se comportam em relação à capacidade do canal de comunicação, considerando as semânticas de chamada síncrona e assíncrona.

Quando processos se comunicam por troca de mensagens, o comportamento em relação à capacidade do canal de comunicação pode variar dependendo das semânticas de chamada síncrona e assíncrona. 

Na chamada **síncrona**, o processo remetente aguarda pela confirmação da entrega da mensagem e pode ser bloqueado se o canal de comunicação estiver cheio. 

Na chamada **assíncrona**, o processo remetente envia a mensagem e continua a execução sem esperar pela confirmação, podendo até enviar mais mensagens, mas pode haver perda de mensagens se o canal estiver congestionado por um período prolongado. 

É importante que os processos estejam sincronizados quanto ao uso do canal de comunicação, o que pode ser alcançado por meio de protocolos de controle de fluxo e de congestionamento.

## 3 - Sobre as afirmações a seguir, relativas mecanismos de comunicação, indique quais são incorretas, justificando sua resposta:

### (a) A comunicação indireta (por canais) é mais adequada para sistemas distribuídos.

**Incorreta**. A comunicação indireta (por canais) pode ser mais adequada para sistemas centralizados, onde o controle e gerenciamento do canal de comunicação é centralizado, mas pode ser menos eficiente para sistemas distribuídos, onde os canais podem passar por diversas camadas de rede e ter alta latência.

### (b) Canais com capacidade finita somente são usados na definição de algoritmos, não sendo implementáveis na prática.

**Correta** essa situação não existe na prática, pois todos os sistemas de computação têm capacidade de memória e de armazenamento finitas.

### (c) Na comunicação direta, o emissor envia os dados diretamente a um canal de comunicação.

**Incorreta**. Na comunicação direta, o emissor envia diretamente os dados ao processo receptor, sem passar por um canal de comunicação.

### (d) Na comunicação por fluxo, a ordem dos dados enviados pelo emissor é mantida do lado receptor.

**Correta**. A ordem dos dados enviados pelo emissor é mantida do lado receptor, garantindo que a mensagem seja recebida de forma coesa e sem erros.

### (e) Na comunicação por troca de mensagens, o núcleo transfere pacotes de dados do processo emissor para o processo receptor.

**Incorreta**. Na comunicação por troca de mensagens, o núcleo do sistema operacional é responsável por gerenciar a transferência de mensagens entre os processos, mas não transfere diretamente pacotes de dados do processo emissor para o processo receptor.

## 4 - Sobre as afirmações a seguir, relativas à sincronização na comunicação entre processos, indique quais são incorretas, justificando sua resposta:

### (a) Na comunicação semi-bloqueante, o emissor espera indefinidamente pela possibilidade de enviar os dados.

**Incorreta**. Na comunicação semi-bloqueante, o emissor espera por um tempo limitado pela possibilidade de enviar os dados, retornando uma mensagem de erro caso o tempo limite seja atingido.

### (b) Na comunicação síncrona ou bloqueante, o receptor espera até receber a mensagem.

**Correta**. O receptor espera até receber a mensagem, bloqueando sua execução enquanto isso.

### (c) Um mecanismo de comunicação semi-bloqueante com prazo t = ∞ equivale a um mecanismo bloqueante.

**Correta**. Se espera por um tempo limite infinito, o mecanismo de comunicação semi-bloqueante se comporta como um mecanismo bloqueante, pois bloqueia os processos até receber a mensagem.

### (d) Na comunicação síncrona ou bloqueante, o emissor retorna uma mensagem de erro caso o receptor não esteja pronto para receber a mensagem.

**Incorreta**. Na comunicação síncrona ou bloqueante, o emissor simplesmente aguarda até que o receptor esteja pronto para receber a mensagem, sem retornar uma mensagem de erro.

### (e) Se o canal de comunicação tiver capacidade nula, emissor e receptor devem usar mecanismos não-bloqueantes.

**Correta**. Se o canal de comunicação tiver capacidade nula, emissor e receptor devem usar mecanismos não-bloqueantes para evitar que a execução seja interrompida indefinidamente.

### (f) A comunicação não-bloqueante em ambos os participantes só é viável usando canais de comunicação com buffer não-nulo.

**Incorreta**. A comunicação não-bloqueante em ambos os participantes pode ser viável mesmo em canais de comunicação com buffer nulo, desde que haja um mecanismo de notificação de eventos disponível para indicar quando há dados disponíveis para serem lidos ou espaço disponível para serem escritos.