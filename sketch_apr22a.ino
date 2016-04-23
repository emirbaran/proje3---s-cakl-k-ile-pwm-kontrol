#include <LiquidCrystal.h>
#define say 2
#define mod 1
#define out 9
#define lm35 0

#define cikis_ver 1
#define cikis_konum 2
#define setsicaklik_ayar 3

double sicaklik;
float analogokunan;
float pwm;
float set_sicaklik=25;
char cikis = 'A';
int durum=1;

double eski_lm35C=0;
double eski_setC=0;
char eski_cikis = 'A';

long simdikizaman;
long eskizaman;
long ekran_eskizaman;
long cursor_eskizaman;
long say_eskizaman;
long bounce_zaman = 300;
int bekleme = 250;
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
void setup() {
  pinMode(say,INPUT);
  pinMode(mod,INPUT);
  pinMode(out,OUTPUT);
  lcd.begin(16, 2);
  lcd.noCursor();
  simdikizaman = millis();
  ekran_eskizaman= millis();
  cursor_eskizaman = millis();
  eskizaman = millis();
  say_eskizaman = millis();
}

void loop() {
  simdikizaman=millis();
  switch(durum){
    case cikis_ver:
      hesapver();
      ekran_yaz(sicaklik,set_sicaklik,cikis);
    break;
    case cikis_konum:
      analogWrite(out,0);
      cursorblink(14,0);
      ekran_yaz(sicaklik,set_sicaklik,cikis);
    break;
    case setsicaklik_ayar:
      analogWrite(out,0);
      cursorblink(5,1);
      ekran_yaz(sicaklik,set_sicaklik,cikis);
    break;
  }

  say_buton();
  mod_buton();
}
void mod_buton(){
  if(digitalRead(mod) == HIGH){
    if(simdikizaman - eskizaman > bounce_zaman){
      durum++;
      if(durum==4){
        durum=1;
      }
      eskizaman = simdikizaman;
    }
  }
}
void say_buton(){
  if(digitalRead(say)==HIGH){
    if(millis() - say_eskizaman > bounce_zaman){
      switch(durum){
      case cikis_konum:
        if(cikis == 'A'){
          cikis = 'K';
        }else{
          cikis = 'A';
        }
      break;
      case setsicaklik_ayar:
        set_sicaklik++;
        if(set_sicaklik == 151){
          set_sicaklik=0;
        }
      break;
      }
      say_eskizaman = millis();
    } 
  }
}
void hesapver(){
  analogokunan = analogRead(lm35);
  analogokunan = (analogokunan/1023)*5000;
  sicaklik = analogokunan / 10,0;
  if(set_sicaklik - sicaklik >0){
    pwm = ((set_sicaklik - sicaklik)*254)/150;
    if(cikis == 'A'){
      analogWrite(out,pwm);
    }
  }else{
    pwm=0;
  }
  
}
void cursorblink(int sutun,int satir){
  lcd.setCursor(sutun,satir);
  if(simdikizaman-cursor_eskizaman >0 & simdikizaman-cursor_eskizaman <1000){
    lcd.cursor();
  }else if(simdikizaman-cursor_eskizaman >=1000 & simdikizaman-cursor_eskizaman <2000){
    lcd.noCursor();
  }
  else{
    cursor_eskizaman = simdikizaman;
  }
}

void ekran_yaz(double lm35_C,double set_C,char c_durum){
  if(eski_lm35C != lm35_C | eski_setC != set_C | eski_cikis != c_durum){
    if(simdikizaman-ekran_eskizaman > bekleme){
      lcd.noCursor();
      lcd.clear();
      lcd.print("LM35:");
      lcd.print(lm35_C,2);
      lcd.print("C");
      lcd.setCursor(13,0);
      lcd.print("(");
      lcd.print(c_durum);
      lcd.print(")");
      lcd.setCursor(0,1);
      lcd.print("SetC:");
      lcd.print(set_C,2);
      lcd.print("C");
      ekran_eskizaman = simdikizaman;
      eski_lm35C = lm35_C;
      eski_setC = set_C;
      eski_cikis = c_durum;
    }
  }
  
}

