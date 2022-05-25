#include <LCD5110_Basic.h>
#include "DHT.h"
DHT dht(5, DHT11); //dht adında sensor nesnesi tanımladık.

LCD5110 lcd(7,8,9,11,10); 
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
unsigned long simdikiZaman=0,eskiZaman=0;
int esikDeger=600;
int kontrol=1;
int sayac = 0;
int tNem;
int b=0;
int buton1=0;
float nem,sicaklik;
float sonSicaklik,sonNem;
int isikMiktari;


void setup() {
  pinMode(A0,INPUT);//A0 toprak nem sensörünü analog giriş olarak ayarlar
  pinMode(6,OUTPUT);//6.pine bağlı motoru çıkış olarak ayarlar
  pinMode(A1,INPUT);//A1 pinine bağlı ldr ışık sensörünü analog giriş olarak ayarlar
  pinMode(4,INPUT);//4.pine bağlı butonu giriş olarak ayarlar 
  digitalWrite(6,HIGH);//6.pine bağlı motoru başlangıçta çalışmamasını sağlar
  dht.begin();
  Serial.begin(9600);
  lcd.InitLCD(); //lcd yi başlat
}

void loop() {
  
  lcd.setFont(SmallFont); //lcd fontunu ayarla
  lcd.print("Ortam Nem:",0,0);
  lcd.print("O.Sicaklik:",0,15);  
  lcd.print("T.Nem:%",0,20); 
  lcd.print("Isik Mik:%",0,30); 
 
buton1=digitalRead(4);
simdikiZaman=millis();

if(simdikiZaman-eskiZaman>=1000 || kontrol==1) 
{
  Serial.println( esikDeger);
  
  sayac++;
  kontrol = 0;
    
  Ldr();
  ToprakNem(); //Toprak nem sensöründen gelen değerleri okuyup LCD ye yazdırır
  OdaSicaklikNem(); //DHT11 sıcaklık nem sensöründen gelen verileri alır ve LCD ye yazdırır
  MotorCalistir(); //Toprak sensöründen alınan veriye göre motoru çalıştırır ve durdurur
  eskiZaman=millis();
  }  
}


void Ldr(){
isikMiktari=analogRead(A1);

lcd.printNumI(isikMiktari/(10.23),62, 30);
  
}


void ToprakNem(){
  tNem=analogRead(A0);
  
  lcd.printNumI(100-(tNem/(10.23)),42, 20);
  
}



void OdaSicaklikNem() //odanın sıcaklık ve nem değerlerini alıp lcd ye yazdırır
{ 
nem = dht.readHumidity(); // Nem miktarını oku
  sicaklik = dht.readTemperature();  //Sıcaklık bilgisini oku

  if(!isnan(sicaklik) && !isnan(nem)){ //Sıcaklık ve Nem nan değilse
  sonSicaklik = sicaklik;
  sonNem = nem;
  
  lcd.printNumI(nem, 60, 0);

  
  lcd.printNumI(sicaklik, 67, 15);
  }
else{
  lcd.printNumI(sonNem, 24, 0);

  lcd.printNumI(sonSicaklik, 55, 15);
  }   
}



void MotorCalistir(){
  if(tNem>esikDeger)
{ 
  if(b%2==0)
  {
  digitalWrite(6,LOW);//6. pine bağlı motoru çalıştırır (röleye bağlı olduğu için LOW dediğimizde başlatır)
  }   
}

if(sayac%5==0){
  b++;
    
  digitalWrite(6,HIGH);//6. pine bağlı motoru durdurur (röleye bağlı olduğu için HIGH dediğimizde durdurur)
  sayac=0;  
  } 
  }
