<h1>Módulo LoRaWAN da Radioenge</h1>

<h2>Informações sobre a biblioteca</h2>
Esta biblioteca conta com todas as funcionalidades descrita no datasheet como também tem algumas funcionalidades a mais a fim de facilitar o uso do módulo com microcontroladores compativeis com Arduino IDE.

<h3>Compatibilidade</h3>
  A biblioteca é compatível com diversos microcontroladores que tem a capacidade de rodar a biblioteca SoftwareSerial ou tem uma serial a mais para se comunicar com o módulo, porém é interessante se atentar aos níveis de tensão para diferentes métodos de conexão com o network server. 
  
 <h3>Sobre o LoRaWAN</h3>
  O LoRaWAN nada mais é do que uma camada adicional a mddulação LoRa com o intuito de ampliar a área de alcance e manter o baixo consumo de energia, porém com uma rede muito bem estruturada de modo que os end devices possam enviar a qualquer momento e o cocentrador enviar para o network server. 

 ![image](https://i0.wp.com/elcereza.com/wp-content/uploads/2023/01/Estrutura-de-rede-LoRaWAN-da-Radioenge.png?resize=1024%2C759&ssl=1)

  Caso você caiu nesse repositório de paraquedas e não faz a mínima ideia que módulo é esse, mas teve o interesse de conhecer sobre, acesse este link: https://elcereza.com/modulo-lorawan-da-radioenge-tutorial-completo/

<h2>Conectando módulo</h2>
  Para conectar o módulo a qualquer network server é necessário incializar a biblioteca, para isso você poderá pegar qualquer exemplo da própria biblioteca. O mais importante é o comando "LoRa.printParameters();", pois através dele que você iriá ler todos os parâmetros de conexão LoRaWAN que já vem gravado de fábrica.
  
  Depois você precisará cadastrar seu end device em um network server, mas neste exemplo vamos usar a The Things Network e caso você ainda não saiba cadastrar o módulo LoRaWAN da Radioenge, recomendo ler esse tutorial: https://elcereza.com/the-things-network-primeiros-passos/

  Agora vamos destrichar o comando "JoinNetWork", pois através dele você irá se conectar com o servidor com configuração automática ou manual.
  LoRa.JoinNetwork(OTAA, TTN, true, !true)
  
```
  bool JoinNetwork(uint8_t njm = NULL,       // Método de conexão (OTAA/APB)
                   uint8_t net = NULL,       // Network Server (TTN [The Things Network] / EN [Everynet] / CS [ChirpStack])
                   bool autoconfig = true,   // Configura o módulo todo de acordo com o network server 
                   bool automatic = NULL,    // Habilita ou desabilta conexão automática
                   String appkey = "",       // Para OTAA [não é necessário por a menos que queria mudar]
                   String appeui = "",       // Para OTAA [não é necessário por a menos que queria mudar]
                   String nwkskey = "",      // Para APB [não é necessário por a menos que queria mudar]
                   String daddr = "")        // Para APB [não é necessário por a menos que queria mudar]
                   
```

<h3>GPIO</h3>


```
  bool pinMode(uint8_t pin,                  // Pino do módulo vai de 0 a 9
               uint8_t modo)                 /* Modo de operação do pino que pode ser:
                                                   OUTPUT
                                                   OUTPUT_OPENDRAIN    
                                                   OUTPUT_FA_PUSHPULL  
                                                   OUTPUT_FA_OPENDRAIN
                                                   INPUT
                                                   INPUT_ADC          
                                                   INTERRUPT_RISING   
                                                   INTERRUPT_FALLING   
                                                   INTERRUPT_CHANGE 
                                               
                                               Veja a imagem a seguir para saber das regras de cada função
                                            */
```

 ![image](https://i0.wp.com/elcereza.com/wp-content/uploads/2023/01/Modos-de-oprecao-das-GPIOs-do-LoRaWAN-da-Radioenge.png?w=950&ssl=1)


<h4>Escrita digital</h4>

```
  uint8_t digitalWrite(uint8_t pin,          // Pino a ser escrito (qualquer um que esteja configurado como saída)
                       uint8_t val)          // Valor desejado, 0 ou 1
```

<h4>Leitura digital</h4>

```
  uint8_t digitalRead(uint8_t pin)           // Pino a ser lido (qualquer um que esteja configurado como entrada digital)
```

<h4>Leitura Analógica</h4>

```
  uint16_t analogRead(uint8_t pin)           // Pino a ser lido (qualquer um que esteja configurado como entrada ADC)
```
