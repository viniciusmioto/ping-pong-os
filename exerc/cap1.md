# Exercícios Capítulo 1

## 1- Quais os dois principais objetivos de um sistema operacional?

Oferecer aos programas aplicativos uma forma de acesso homogênea aos dispositivos físicos, que permita abstrair sua complexidade e as diferenças tecnológicas entre eles.

* `ABSTRAÇÃO`: definir interfaces abstratas para os recursos do hardware, com os seguintes obejetivos: 
Prover interfaces de acesso aos dispositivos, mais simples de usar que as interfaces de baixo nível; 
Tornar os aplicativos independentes do hardware; 
Definir interfaces de acesso homogêneas para dispositivos com tecnologias distintas

* `GERÊNCIA`: Em um sistema com várias atividadessimultâneas, podem surgir conflitos no uso do hardware, quando dois ou mais aplicativos precisam dos mesmos recursos para poder executar. 
Cabe ao sistema operacional definir políticas para gerenciar o uso dos recursos de hardware pelos aplicativos, e resolver eventuais disputas e conflitos


Assim, um sistema operacional visa **abstrair** o acesso e **gerenciar** os recursos de hardware, provendo aos aplicativos um ambiente de execução abstrato, no qual o acesso aos recursos se faz através de interfaces simples, independentes das características e detalhes de baixo nível, e no qual os conflitos no uso do hardware são minimizados.

## Por que a abstração de recursos é importante para os desenvolvedores de aplicações? Ela tem alguma utilidade para os desenvolvedores do próprio sistema operacional?

Para os desenvolvedores de aplicações fica muito mais simples criar aplicações que utilizam os recursos de hardware. Com isso, o desenvolvedor não se precisa se preocupar com detalhes de baixo nível ao acessar o hardware.

Para os desenvolvedores do S.O. a vantagem é a padronização na forma de acesso aos recursos de hardware. Dessa forma, o S.O. estará mais sólido para que os aplicativos de terceiro acessem os recursos necessários com as abstrações já desenvolvidas.

Além disso, a abstração permite que os desenvolvedores do S.O. criem camadas abstratas sobre o hardware subjacente para facilitar o desenvolvimento de drivers e outras partes do sistema operacional. Por exemplo, um driver de dispositivo pode usar uma API abstrata para interagir com um dispositivo de entrada, em vez de ter que lidar diretamente com a eletrônica do dispositivo e todos os detalhes técnicos de baixo nível. Isso torna o desenvolvimento do sistema operacional mais modular e facilita a manutenção e a atualização do sistema como um todo.