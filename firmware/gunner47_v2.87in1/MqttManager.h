#ifdef USE_MQTT
/*
 * Библиотека асинхронных MQTT запросов https://github.com/marvinroger/async-mqtt-client
 * Не умеет автоматически восстанавливать разорванное соединение с MQTT брокером, поэтому требует периодической проверки подключения
 * Зависит от библиотек:
 *   ESPAsyncTCP https://github.com/me-no-dev/ESPAsyncTCP
 *   AsyncTCP https://github.com/me-no-dev/AsyncTCP
 * Лампа подписана на топики:
 *   LedLamp/LedLamp_xxxxxxxx/cmnd - общий топик для включения/выключения и других команд
 *   LedLamp/LedLamp_xxxxxxxx/bri/cmnd - топик для регулировки яркости
 *   LedLamp/LedLamp_xxxxxxxx/spd/cmnd - топик для регулировки скорости
 *   LedLamp/LedLamp_xxxxxxxx/scl/cmnd - топик для регулировки масштаба
 *   LedLamp/LedLamp_xxxxxxxx/eff/cmnd - топик для установки номера эффекта
 * Все топики будут подписаны на соответствующие команды и значения.
*/

#include <AsyncMqttClient.h>
#include "pgmspace.h"
#include "Constants.h"
#include "Types.h"

static const char TopicBase[]          PROGMEM = "LedLamp";                     // базовая часть топиков
static const char TopicCmnd[]          PROGMEM = "cmnd";                        // часть командных топиков (входящие команды лампе)
static const char TopicBriCmnd[]       PROGMEM = "bri/cmnd";                    // топик для регулировки яркости
static const char TopicSpdCmnd[]       PROGMEM = "spd/cmnd";                    // топик для регулировки скорости
static const char TopicSclCmnd[]       PROGMEM = "scl/cmnd";                    // топик для регулировки масштаба
static const char TopicEffCmnd[]       PROGMEM = "eff/cmnd";                    // топик для установки номера эффекта
static const char TopicState[]         PROGMEM = "state";                       // часть топиков состояния (ответ от лампы)

static const char MqttServer[]         PROGMEM = "192.168.0.100";               // строка с IP адресом MQTT брокера
static const uint16_t MqttPort                 = 1883U;                         // порт MQTT брокера
static const char MqttUser[]           PROGMEM = "";                            // пользователь MQTT брокера
static const char MqttPassword[]       PROGMEM = "";                            // пароль пользователя MQTT брокера
static const char MqttClientIdPrefix[] PROGMEM = "LedLamp_";                    // id клиента MQTT брокера (к нему будет добавлен ESP.getChipId)

class MqttManager
{
  public:
    static uint32_t mqttLastConnectingAttempt;
    static void setupMqtt(AsyncMqttClient* mqttClient, char* lampInputBuffer, SendCurrentDelegate sendCurrentDelegate);
    static void mqttConnect();
    static void onMqttConnect(bool sessionPresent);
    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    static void onMqttMessage(char *topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    static bool publish(const char *topic, const char *value);
    static void publishState();
    static bool needToPublish;
    static char mqttBuffer[MAX_UDP_BUFFER_SIZE];
  private:
    static char* mqttServer;
    static char* mqttUser;
    static char* mqttPassword;
    static char* topicInput;                                                    // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicCmnd
    static char* topicBriInput;                                                 // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicBriCmnd
    static char* topicSpdInput;                                                 // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicSpdCmnd
    static char* topicSclInput;                                                 // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicSclCmnd
    static char* topicEffInput;                                                 // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicEffCmnd
    static char* topicOutput;                                                   // TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicState
    static char* clientId;
    static char* lampInputBuffer;                                               // ссылка на inputBuffer для записи в него пришедшей MQTT команды и последующей её обработки лампой
    static AsyncMqttClient* mqttClient;
    static const uint8_t qos = 0U;                                              // MQTT quality of service
    static const uint32_t connectionTimeout = MQTT_RECONNECT_TIME * 1000U;      // период времени для проверки (пере)подключения к MQTT брокеру, 10 секунд
    static char* byteToHex(char *out, uint8_t value);
    static bool allocStr(char **str, const char *src);
    static bool allocStr_P(char **str, PGM_P src);
    static SendCurrentDelegate sendCurrentDelegate;
};

void MqttManager::setupMqtt(AsyncMqttClient* mqttClient, char* lampInputBuffer, SendCurrentDelegate sendCurrentDelegate)
{
  allocStr_P(&MqttManager::mqttServer, MqttServer);
  allocStr_P(&MqttManager::mqttUser, MqttUser);
  allocStr_P(&MqttManager::mqttPassword, MqttPassword);

  MqttManager::mqttClient = mqttClient;
  MqttManager::lampInputBuffer = lampInputBuffer;
  MqttManager::sendCurrentDelegate = sendCurrentDelegate;
  MqttManager::mqttClient->setServer(MqttManager::mqttServer, MqttPort);

  char clientIdBuf[sizeof(MqttClientIdPrefix) + 8];
  strcpy_P(clientIdBuf, MqttClientIdPrefix);
  uint32_t chipId = ESP.getChipId();
  for (uint8_t i = 0; i < 4; ++i)
  {
    byteToHex(&clientIdBuf[i * 2 + sizeof(MqttClientIdPrefix) - 1], chipId >> ((3 - i) * 8));
  }
  allocStr(&clientId, clientIdBuf);
  MqttManager::mqttClient->setClientId(clientId);

  if (MqttManager::mqttUser != NULL)
  {
    MqttManager::mqttClient->setCredentials(MqttManager::mqttUser, MqttManager::mqttPassword);
  }

  uint8_t topicLength = sizeof(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicCmnd) + 1;
  topicInput = (char*)malloc(topicLength);
  sprintf_P(topicInput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicCmnd);      // topicInput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicCmnd

  topicLength = sizeof(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicBriCmnd) + 1;
  topicBriInput = (char*)malloc(topicLength);
  sprintf_P(topicBriInput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicBriCmnd); // topicBriInput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicBriCmnd

  topicLength = sizeof(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicSpdCmnd) + 1;
  topicSpdInput = (char*)malloc(topicLength);
  sprintf_P(topicSpdInput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicSpdCmnd); // topicSpdInput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicSpdCmnd

  topicLength = sizeof(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicSclCmnd) + 1;
  topicSclInput = (char*)malloc(topicLength);
  sprintf_P(topicSclInput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicSclCmnd); // topicSclInput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicSclCmnd

  topicLength = sizeof(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicEffCmnd) + 1;
  topicEffInput = (char*)malloc(topicLength);
  sprintf_P(topicEffInput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicEffCmnd); // topicEffInput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicEffCmnd

  topicLength = sizeof(TopicBase) + 1 + strlen(clientId) + 1 + sizeof(TopicState) + 1;
  topicOutput = (char*)malloc(topicLength);
  sprintf_P(topicOutput, PSTR("%s/%s/%s"), TopicBase, clientId, TopicState);     // topicOutput = TopicBase + '/' + MqttClientIdPrefix + ESP.getChipId + '/' + TopicState

  mqttClient->subscribe(topicInput, 1);  // Подписка на общий топик
  mqttClient->subscribe(topicBriInput, 1);  // Подписка на топик для яркости
  mqttClient->subscribe(topicSpdInput, 1);  // Подписка на топик для скорости
  mqttClient->subscribe(topicSclInput, 1);  // Подписка на топик для масштаба
  mqttClient->subscribe(topicEffInput, 1);  // Подписка на топик для эффекта

  mqttClient->onConnect(onMqttConnect);
  mqttClient->onDisconnect(onMqttDisconnect);
  mqttClient->onMessage(onMqttMessage);
}

void MqttManager::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  if (payload != NULL)
  {
    strncpy(lampInputBuffer, payload, len);
    lampInputBuffer[len] = '\0';
    needToPublish = true;
  }

  // Проверка топика и обработка команд
  if (strncmp(topic, topicInput, strlen(topicInput)) == 0)
  {
    // Обработка команды включения/выключения
    if (strcmp(payload, "ON") == 0)
    {
      // Включение лампы
      turnOnLamp();
    }
    else if (strcmp(payload, "OFF") == 0)
    {
      // Выключение лампы
      turnOffLamp();
    }
  }
  else if (strncmp(topic, topicBriInput, strlen(topicBriInput)) == 0)
  {
    // Обработка команды яркости
    int brightness = atoi(payload);
    setBrightness(brightness);
  }
  else if (strncmp(topic, topicSpdInput, strlen(topicSpdInput)) == 0)
  {
    // Обработка команды скорости
    int speed = atoi(payload);
    setSpeed(speed);
  }
  else if (strncmp(topic, topicSclInput, strlen(topicSclInput)) == 0)
  {
    // Обработка команды масштаба
    int scale = atoi(payload);
    setScale(scale);
  }
  else if (strncmp(topic, topicEffInput, strlen(topicEffInput)) == 0)
  {
    // Обработка команды эффекта
    int effect = atoi(payload);
    setEffect(effect);
  }

  #ifdef GENERAL_DEBUG
  LOG.print(F("Получено MQTT:"));
  LOG.print(F(" топик \""));
  LOG.print(topic);
  LOG.print("\"");
  LOG.print(F(", значение \""));
  LOG.print(lampInputBuffer);
  LOG.println("\"");
  #endif
}

void MqttManager::setBrightness(int value)
{
  // Код для установки яркости
}

void MqttManager::setSpeed(int value)
{
  // Код для установки скорости
}

void MqttManager::setScale(int value)
{
  // Код для установки масштаба
}

void MqttManager::setEffect(int value)
{
  // Код для установки эффекта
}

void MqttManager::turnOnLamp()
{
  // Код для включения лампы
}

void MqttManager::turnOffLamp()
{
  // Код для выключения лампы
}

bool MqttManager::publish(const char *topic, const char *value)
{
  return mqttClient->publish(topic, qos, true, value);
}

void MqttManager::publishState()
{
  // Код для публикации состояния лампы
}

bool MqttManager::needToPublish = false;

