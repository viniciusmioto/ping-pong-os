# Exercícios Capítulo 9

## 1 - Classifique as filas de mensagens UNIX de acordo com os tipos de comunicação discutidos no Capítulo 8.

Implementam o conceito de *mailbox*. As filas de mensagens são mecanismos de comunicação assíncrona entre vários processos (N:M ou N:1, dependendo da implementação), confiáveis, orientadas a mensagens e com capacidade finita. 

As operações de envio e recepção podem ser síncronas ou assíncronas, dependendo da implementação e a critério do programador.

## 2 - Classifique os pipes UNIX de acordo com os tipos de comunicação discutidos no Capítulo 8.

O pipe pode ser classificado como um canal de comunicação local entre dois processos (1:1), unidirecional, síncrono, orientado a fluxo, confiável e com capacidade finita (os pipes do Linux armazenam 64 KBytes por default).

## 3 - Classifique as áreas de memória compartilhadas de acordo com os tipos de comunicação discutidos no Capítulo 8.

As áreas de memória compartilhada são uma forma de comunicação direta e podem ser utilizadas para implementar diferentes modelos de sincronização, de acordo com as necessidades do aplicativo.

## 4 - Sobre as afirmações a seguir, relativas aos mecanismos de comunicação, indique quais são incorretas, justificando sua resposta:

### (a) As filas de mensagens POSIX são um exemplo de canal de comunicação com capacidade nula.

**Incorreta**. As filas de mensagens POSIX são um exemplo de canal de comunicação com capacidade finita.

### (b) A memória compartilhada provê mecanismos de sincronização para facilitar a comunicação entre os processos.

**Incorreta**. A memória compartilhada provê mecanismos para compartilhar uma região de memória entre processos, mas o programador deve implementar os mecanismos de sincronização necessários para garantir a consistência dos dados compartilhados.

### (c) A troca de dados através de memória compartilhada é mais adequada para a comunicação em rede.

**Incorreta**. A troca de dados através de memória compartilhada é mais adequada para a comunicação entre processos em um mesmo sistema.

### (d) Processos que se comunicam por memória compartilhada podem acessar a mesma área da RAM.

**Correta**. O núcleo do sistema operacional mapeia a mesma área de memória virtual para os processos que compartilham a memória.

### (e) Os pipes Unix são um bom exemplo de comunicação M:N.

**Incorreta**. Os pipes Unix são um bom exemplo de comunicação 1:1, unidirecional, síncrono, orientado a fluxo, confiável e com capacidade finita.

### (f) A comunicação através de memória compartilhada é particularmente indicada para compartilhar grandes volumes de dados entre dois ou mais processos.

**Correta**. A comunicação através do núcleo pode ser ineficiente caso seja frequente e o volume de dados a transferir seja elevado, por causa das trocas de contexto envolvidas nas chamadas de sistema. Para essas situações, seria conveniente ter uma área de memória compartilhada.

### (g) As filas de mensagens POSIX são um bom exemplo de canal de eventos.

**Incorreta**. As filas de mensagens POSIX são um exemplo de canal de comunicação de mensagens, não de eventos.

### (h) Nas filas de mensagens POSIX, as mensagens transitam através de arquivos em disco criados especialmente para essa finalidade.

**Incorreta**. O arquivo `/my_queue` referenciado em ambas as listagens serve unicamente como identificador comum para a fila de mensagens; nenhum arquivo de dados com esse nome será criado pelo sistema. As mensagens não transitam por arquivos, apenas pela memória do núcleo.

### (i) Em UNIX, um pipe é um canal de comunicação unidirecional que liga a saída padrão de um processo à entrada padrão de outro.

**Correta**. Em UNIX, um *pipe* é um canal de comunicação unidirecional que liga a saída padrão de um processo à entrada padrão de outro.