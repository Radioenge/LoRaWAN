/* Exemplo: uso do m5stack (https://shop.m5stack.com/products/atom-lite-esp32-development-kit?variant=32259605200986)
 *          em conjunto com o módulo LoRaWAN Radioenge (https://www.radioenge.com.br/produto/modulo-lorawan/) e sensor 
 *          de temperatura e umidade DHT11. Neste exemplo, o módulo LoRaWAN opera como ABP.
 *          Este exemplo envia um pacote de 2 bytes para a rede LoRaWAN da ATC/Everynet e, após o envio, faz o m5stack
 *          entrar em deep sleep (em segundos) definido em TEMPO_EM_SLEEP. Dessa forma, o exemplo busca ser o mais low
 *          power possível.
 * 
 * Ligações: 
 *       GPIO 23: pino de dados do DHT11
 *       3V3: pino VCC do DHT11
 *       GND: pino GND do DHT11
 *       5V: pinos 4 e 5 do módulo LoRaWAN Radioenge
 *       GND: pino 1 do módulo LoRaWAN Radioenge
 *       GPIO 22: pino 2 do módulo LoRaWAN Radioenge
 *       GPIO 19: pino 3 do módulo LoRaWAN Radioenge
 *
 * Bibliotecas:
 *       m5stack: https://github.com/m5stack/M5Atom
 *       DHT11: https://github.com/adafruit/DHT-sensor-library 
 * 
 * Autor: Pedro Bertoleti
 */

#include "M5Atom.h"
#include "DHT.h"

/* Definições - DHT11 (sensor de temperatura e umidade) */
#define DHTPIN                        23
#define DHTTYPE                       DHT11

/* Definições - cores do LED RGB do m5stack */
#define LED_RGB_VERMELHO          0xff0000
#define LED_RGB_VERDE             0x00ff00
#define LED_RGB_AZUL              0x0000ff
#define LED_RGB_AMARELO           0xfff000
#define LED_RGB_APAGADO           0x000000

/* Definições - GPIOs usados na comunicação com o modulo lorawan (Radioenge) */
#define UART_LORAWAN_TX                 22
#define UART_LORAWAN_RX                 19
#define UART_LORAWAN_BAUDRATE           9600
#define UART_LORAWAN_BITS_E_PARIDADE    SERIAL_8N1

/* Definições - baudrates das comunicações seriais 
 *              (debug verboso e comunicação com módulo LoRaWAN) 
 */
#define BAUDRATE_SERIAL_DEBUG          115200
#define BAUDRATE_SERIAL_MOD_LORAWAN    9600

/* Definição - tempo de resposta máximo a um comando AT enviado
 *             para o módulo LoRaWAN 
 */
#define TEMPO_RESPOSTA_MOD_LORAWAN     1500 //ms

/* Definição - tamanho maximo da string referente a um comando AT */
#define TAM_MAX_CMD_AT                 80

/* Definições - deep sleep do m5stack */
#define FATOR_US_PARA_S   (uint64_t )1000000
#define TEMPO_EM_SLEEP    (uint64_t)300

/* Arrays de chaves e endereços do modulo LoRaWAN */
/* Substitua pelas suas chaves/endereços (referentes a sua conta e dispositivo na Everynet) */
char APPSKEY[60] = "\0";
char NWSKEY[60] = "\0";
char APPEUI[30] = "\0";
char DEVADDR[15] = "\0";

/* Array com máscara de canais utilizada */
char CHMASK[35] = "00FF:0000:0000:0000:0000:0000\0"; //máscara referente a Everynet (LA915)

/* Arrays que armazenam comandos AT */
char cmd_at[TAM_MAX_CMD_AT] = {0};
char cmd_buffer[TAM_MAX_CMD_AT] = {0};

/* Variáveis que armazenam temperatura e umidades lidas do DHT11 */
uint8_t temperatura = 0;
uint8_t umidade = 0;

/* Objeto para leitura do DHT11 */
DHT dht(DHTPIN, DHTTYPE);

/* Protótipos */
void envia_comando_mod_lorawan(char * pt_comando, int tam_string);
unsigned long diferenca_tempo(unsigned long tref);

/* Função: envia comando AT para o modulo lorawan
 * Parâmetros: - ponteiro para string de comando
  *            - tamanho da string de comando
 * Retorno: nenhum
 */
void envia_comando_mod_lorawan(char * pt_comando, int tam_string)
{
    unsigned long timestamp_resposta;
    char c;

    /* Envia comando */
    Serial.println("Enviando comando...");
    memset(cmd_buffer, 0x00, TAM_MAX_CMD_AT);
    memcpy(cmd_buffer, pt_comando, tam_string);
    Serial1.write(cmd_buffer, tam_string);
    Serial1.flush();
    Serial.print("Comando enviado! Cmd: ");
    Serial.println(cmd_buffer);

    /* Aguarda resposta */
    Serial.println("Aguardando resposta (modulo LoRaWAN)...");
    timestamp_resposta = millis();

    do 
    {
        /* Escreve dados recebidos via UART do módulo LoRaWAN */
        if (Serial1.available()) 
        {
            c = Serial1.read(); 
            Serial.print(c);
        }

        if (c == '\n')
        {
            break;
        }
    }while (diferenca_tempo(timestamp_resposta) <= TEMPO_RESPOSTA_MOD_LORAWAN);

    delay(200);
}

/* Função: calcula a diferença entre instante atual e
 *         uma referência de tempo
 * Parâmetros: referência de tempo
 * Retorno: diferença de tempo calculada
 */
unsigned long diferenca_tempo(unsigned long tref)
{
    return (millis() - tref);
}

void setup() 
{
    /* Inicializa m5stack */
    M5.begin(true, false, true); 
    delay(10);

    M5.dis.drawpix(0, LED_RGB_AZUL);

    /* Inicializa DHT11 */
    dht.begin();
    
    /* Inicialização da UART de comunicação com o módulo lorawan */
    Serial1.begin(UART_LORAWAN_BAUDRATE,
                  UART_LORAWAN_BITS_E_PARIDADE,
                  UART_LORAWAN_RX,
                  UART_LORAWAN_TX);
    
    Serial.println("Device LoRaWAN - inicio");
    Serial.println("--- Inicio da configuracao do modulo LoRaWAN ---");
    Serial.println("--- Envio e confirmacao de cada comando AT de configuracao ---");

    Serial.print("Configurando Channel Mask em ");
    Serial.print(CHMASK);
    Serial.println("...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+CHMASK=%s\n\r", CHMASK);
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+CHMASK=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    /* Configura módulo LoRaWAN */
    Serial.print("Obtendo versão de firmware...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+VER=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");
    
    Serial.print("Configurando Join Mode como ABP...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+NJM=0\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+NJM=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");
    
    Serial.println("Dev EUI: ");
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+DEUI=?");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Configurando Dev ADDR em ");
    Serial.print(DEVADDR);
    Serial.println("...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+DADDR=%s\n\r", DEVADDR);
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+DADDR=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Configurando APP EUI em ");
    Serial.print(APPEUI);
    Serial.println("...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+APPEUI=%s\n\r", APPEUI);
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+APPEUI=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Configurando APP Session key em ");
    Serial.print(APPSKEY);
    Serial.println("...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+APPSKEY=%s\n\r", APPSKEY);
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+APPSKEY=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Configurando Network Session key em ");
    Serial.print(NWSKEY);
    Serial.println("...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+NWKSKEY=%s\n\r", NWSKEY);
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+NWKSKEY=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Configurando ADR em 0...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+ADR=0\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+ADR=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Configurando DR para menor payload e maior alcance...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+DR=0\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+DR=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Configurando LoRaWAN como classe A...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+CLASS=A\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+CLASS=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.print("Desliga confirmação de envio...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+CFM=0\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+CFM=?\n\r");
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.println("---");

    Serial.println("--- Fim da configuracao do modulo LoRaWAN ---");
    M5.dis.drawpix(0, LED_RGB_VERDE);
}

void loop() 
{
    uint64_t tempo_em_sleep_us;
    unsigned long timestamp_envio_lorawan;
    
    /* Le sensor, faz envio dos dados via lorawan
     * e entra em deep sleep
     */
    
    /* Le a temperatura atual */
    do
    {
        temperatura = (uint8_t)dht.readTemperature();
    }while( (isnan(temperatura)) || (temperatura == 0) || (temperatura > 60) );

    /* Le a umidade atual */
    do
    {
        umidade = (uint8_t)dht.readHumidity();
    }while( (isnan(umidade)) || (umidade == 0) || (umidade > 100) );
    
    Serial.print("Enviando dados...");
    memset(cmd_at, 0x00, TAM_MAX_CMD_AT);
    
    snprintf(cmd_at, TAM_MAX_CMD_AT, "AT+SENDB=5:%02X%02X\r\n", (uint8_t)temperatura, 
                                                                (uint8_t)umidade);
    
    Serial.print("Comando de envio: ");
    Serial.println(cmd_at);
    envia_comando_mod_lorawan(cmd_at, strlen(cmd_at));
    Serial.print("Dados enviados!");
    M5.dis.drawpix(0, LED_RGB_APAGADO);

    timestamp_envio_lorawan = millis();
    while(diferenca_tempo(timestamp_envio_lorawan) <= 3000);

    Serial.print("------");
    Serial.print("Entrando em deep sleep...");

    tempo_em_sleep_us = FATOR_US_PARA_S * TEMPO_EM_SLEEP;
    esp_sleep_enable_timer_wakeup(tempo_em_sleep_us);    
    esp_deep_sleep_start();        
}