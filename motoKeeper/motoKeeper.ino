#include <SoftwareSerial.h>
SoftwareSerial GSMport(9, 10); // RX, TX
int char_;

void setup() {
  delay(3000); //дадим время на инициализацию GSM модулю
  Serial.begin(9600);  //скорость порта
  Serial.println("GPRS test");
  GSMport.begin(9600);
  //GSMport.println("ATD9104737606");
  test();

  /*GSMport.println("AT+CMGF=1");  //режим кодировки СМС - обычный (для англ.)
    delay(100);
    GSMport.println("AT+CSCS=\"GSM\"");  //режим кодировки текста
    delay(100);*/
  //sms("hello, fucker!","+79104737606");

  gprs_init();

  showLoc();
  delay(1000);
  //GSMport.println("ATD79104737606");

}

void loop() {
  /*
    SensorState = digitalRead(SensorPin);
    if (SensorState != SensorLastState) {  //если смена состояния датчика
      Serial.print("sensor changed to: ");
      Serial.println(SensorState);
      SensorLastState = SensorState;
      gprs_send(String(SensorState));
      delay(100);
    }*/
  if (GSMport.available()) {  //если GSM модуль что-то послал нам, то
    Serial.println(ReadGSM());  //печатаем в монитор порта пришедшую строку
  }
  delay(100);
}

void gprs_init() {  //Процедура начальной инициализации GSM модуля
  int d = 500;
  int ATsCount = 7;

  /*AT+SAPBR=3,1,"CONTYPE","GPRS"
    ОК
    AT+SAPBR=3,1,"APN","internet.mts.ru"
    ОК
    AT+SAPBR=3,1,"USER","mts"
    ОК
    AT+SAPBR=3,1,"PWD","mts"
    ОК*/
  String ATs[] = {  //массив АТ команд
    "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",  //Установка настроек подключения
    "AT+SAPBR=3,1,\"APN\",\"internet.mts.ru\"",
    "AT+SAPBR=3,1,\"USER\",\"mts\"",
    "AT+SAPBR=3,1,\"PWD\",\"mts\"",
    "AT+SAPBR=1,1",  //Устанавливаем GPRS соединение
    "AT+HTTPINIT",  //Инициализация http сервиса
    "AT+HTTPPARA=\"CID\",1"  //Установка CID параметра для http сессии
  };

  /*String ATs[] = {  //массив АТ команд
    "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",  //Установка настроек подключения
    "AT+SAPBR=3,1,\"APN\",\"internet.tele2.ru\"",
    "AT+SAPBR=3,1,\"USER\",\"tele2\"",
    "AT+SAPBR=3,1,\"PWD\",\"tele2\"",
    "AT+SAPBR=1,1",  //Устанавливаем GPRS соединение
    "AT+HTTPINIT",  //Инициализация http сервиса
    "AT+HTTPPARA=\"CID\",1"  //Установка CID параметра для http сессии
    };*/
  int ATsDelays[] = {6, 1, 1, 1, 3, 3, 1}; //массив задержек
  Serial.println("GPRG init start");
  for (int i = 0; i < ATsCount; i++) {
    Serial.println(">>" + ATs[i]); //посылаем в монитор порта
    GSMport.println(ATs[i]);  //посылаем в GSM модуль
    delay(d * ATsDelays[i]);
    Serial.println(ReadGSM());  //показываем ответ от GSM модуля
    delay(d);
  }
  Serial.println("GPRG init complete");
}

void showLoc()
{
  int ATsCount = 1;
  String ATs[] = {  //массив АТ команд
    "AT+CIPGSMLOC=1,1"
  };
  Serial.println("location start");
  for (int i = 0; i < ATsCount; i++) {
    Serial.println(">>" + ATs[i]); //посылаем в монитор порта
    GSMport.println(ATs[i]);  //посылаем в GSM модуль
    delay(2000);
    Serial.println(ReadGSM());  //показываем ответ от GSM модуля
    delay(2000);
  }
  Serial.println("location complete");

}
void test() {
  int ATsCount = 1;
 /* String ATs[] = {  //массив АТ команд
    // "AT",  //Установка настроек подключения
    "AT+CPAS",
    //  "AT+IPR",
    //  "AT+CSQ",
    //"AT+CBC"
    //  "AT+CREG",
    //  "AT+COPS?",
    //  "AT+COPS=?",
    //  "AT+CIPGSMLOC=1,1"
  };
  Serial.println("test start");
  for (int i = 0; i < ATsCount; i++) {
    delay(2000);
    Serial.println(ReadGSM());  //показываем ответ от GSM модуля
    delay(2000);
  }*/
  String cmd="AT";
  Serial.println("simple test");
  runAT("AT");
  Serial.println("device status");
  Serial.println("possible values:");
  Serial.println(" - 0 ready");
  Serial.println(" - 1 unavailable");
  Serial.println(" - 2 unknown");
  Serial.println(" - 3 ringing");
  Serial.println(" - 4 call in progress");
  Serial.println(" - 5 asleep");
  runAT("AT+CAPS");
  Serial.println("Get baud");
  runAT("AT+IPR");
  Serial.println("Get signal strength");
  runAT("AT+CSQ");
  Serial.println("Get battery level");
  runAT("AT+CBC");
  Serial.println("Get operator");
  runAT("AT+COPS?");
  //Serial.println("Get location");
  //runAT("AT+CIPGSMLOC=1,1");
  
  /*
  
0 not registered, MT is not currently searching a new operator to register to
1 registered, home network
2 not registered, but MT is currently searching a new operator to register to
3 registration denied
4 unknown (e.g. out of GERAN/UTRAN/E-UTRAN coverage)
5 registered, roaming
6 registered for "SMS only", home network (applicable only when indicates E-UTRAN)
7 registered for "SMS only", roaming (applicable only when indicates E-UTRAN)
8 attached for emergency bearer services only (see NOTE 2) (not applicable)
9 registered for "CSFB not preferred", home network (applicable only when indicates E-UTRAN)
10 registered for "CSFB not preferred", roaming (applicable only when indicates E-UTRAN)
  */
  
  Serial.println("Get registration status");
  runAT("AT+CREG");
  Serial.println("test complete");
}

void runAT(String command)
{
  Serial.println(">>" + command); //посылаем в монитор порта
  GSMport.println(command);  //посылаем в GSM модуль
  waitGSM();
  Serial.println(ReadGSM());  //показываем ответ от GSM модуля
}
/*
  void gprs_send(String data) {  //Процедура отправки данных на сервер
  //отправка данных на сайт
  int d = 400;
  Serial.println("Send start");
  Serial.println("setup url");
  GSMport.println("AT+HTTPPARA=\"URL\",\"http://mysite.ru/?a=" + data + "\"");
  delay(d * 2);
  Serial.println(ReadGSM());
  delay(d);
  Serial.println("GET url");
  GSMport.println("AT+HTTPACTION=0");
  delay(d * 2);
  Serial.println(ReadGSM());
  delay(d);
  Serial.println("Send done");
  }
*/
String ReadGSM() {  //функция чтения данных от GSM модуля
  int c;
  String v;
  while (GSMport.available()) {  //сохраняем входную строку в переменную v
    c = GSMport.read();
    if (char(c) != NULL && char(c) != 13)
      v += char(c);
    delay(10);
  }
  return v;
}

void waitGSM()
{
  delay(10);
  unsigned int t = 0;
  while (!GSMport.available())   //сохраняем входную строку в переменную v
  {
    t += 50;
    if (t > 5000)
    {
      Serial.println("Timeout before reply too big!");
      return;
    }
    delay(50);
  }
}

void sms(String text, String phone) {
  Serial.println("SMS send started");
  GSMport.println("AT+CMGS=\"" + phone + "\"");
  delay(1000);
  GSMport.print(text);
  delay(300);
  GSMport.print((char)26);
  delay(300);
  Serial.println("SMS send finish");
  delay(1000);
  Serial.println(ReadGSM());  //показываем ответ от GSM модуля
  delay(3000);
}
