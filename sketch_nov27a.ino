
//Programa: Trabalho para automatização de ração
//Autor: Gabriel Barreto
 
//Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
#include "Stepper_28BYJ_48.h"
#include <string.h>
#include "TimerOne.h"

//Define os pinos que serão utilizados para ligação ao display

Stepper_28BYJ_48 myStepper(11,10,9,8); 
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
#define btn1 A0
#define btn2 A1

#define btnMais A2
#define btnMenos A3

int estado = 0; //0 - Proxima racao, 1 - Set racao   
char *str;
char *str2;
int horaConfigurada = 0;
int minutosConfigurados = 0;
int hora = 0;
int minutos = 0;
int segundos = 60;
char buffer[20];
int flagEdit = 0; // 0 - Reset, 1 - Hora, 2 - Minutos
int disparaMotor = 0;


void abreMotor(){
  for (int i = 0; i<=3; i++)
   {
   myStepper.step(-512); 
   delay(1000);
  }
}
void fechaMotor(){
  for (int i = 0; i<=3; i++)
   {
   myStepper.step(512); 
   delay(1000);
  }
}

void decrementaTempo(){
  if(horaConfigurada != 0 || minutosConfigurados !=0){
    if(segundos == 0){
      if(minutosConfigurados != 0){
        minutosConfigurados--;
      }else{
        horaConfigurada--;
        minutosConfigurados = 60;
      }
      segundos = 60;
    }else{
      segundos--;
    }
  }
}
 
void setup()
{
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(decrementaTempo);      
}
 
void loop()
{
  

  
  if(analogRead(btn1)>0){
    estado = 0;
    while(analogRead(btn1)>0);
  }

  if(analogRead(btnMais)>0){
    if(flagEdit == 1){
      if(hora < 24){
        hora++;
      }
    }else if(flagEdit == 2){
      if(minutos < 60){
        minutos+=1;
      }
    }
    while(analogRead(btnMais)>0);
  }

  
  if(analogRead(btnMenos)>0){
    if(flagEdit == 1){
      if(hora > 0){
        hora--;
      }
    }else if(flagEdit == 2){
      if(minutos > 0){
        minutos-=1;
      }
    }
    while(analogRead(btnMenos)>0);
  }


  if(analogRead(btn2)>0){
    estado = 1;
    //while(analogRead(btn2)>0);
  }
  
  if(estado == 0){
    //Limpa a tela
    lcd.clear();
    //Posiciona o cursor na coluna 3, linha 0;
    lcd.setCursor(0, 0);
    //Envia o texto entre aspas para o LCD
    lcd.print("Proxima Racao");
    lcd.setCursor(0, 1);
    lcd.setCursor(0, 1);
    if(horaConfigurada < 10){
      lcd.print("0");
      lcd.setCursor(1, 1);
      lcd.print(itoa(horaConfigurada,buffer,10));
    }else{
      lcd.print(itoa(horaConfigurada,buffer,10));
    }
    lcd.setCursor(2, 1);
    lcd.print(":");
    lcd.setCursor(3, 1);
    if(minutosConfigurados < 10){
      lcd.print("0");
      lcd.setCursor(4, 1);
      lcd.print(itoa(minutosConfigurados,buffer,10));
    }else{
      lcd.print(itoa(minutosConfigurados,buffer,10));
    }
    delay(100);
  }else{
    //Limpa a tela
    lcd.clear();
    //Posiciona o cursor na coluna 3, linha 0;
    lcd.setCursor(0, 0);
    //Envia o texto entre aspas para o LCD
    lcd.print("Config. Horario");
    //Mostra o horario
    lcd.setCursor(0, 1);
    if(hora < 10){
      lcd.print("0");
      lcd.setCursor(1, 1);
      lcd.print(itoa(hora,buffer,10));
    }else{
      lcd.print(itoa(hora,buffer,10));
    }
    lcd.setCursor(2, 1);
    lcd.print(":");
    lcd.setCursor(3, 1);
    if(minutos < 10){
      lcd.print("0");
      lcd.setCursor(4, 1);
      lcd.print(itoa(minutos,buffer,10));
    }else{
      lcd.print(itoa(minutos,buffer,10));
    }
    if(flagEdit == 1){
      lcd.setCursor(7,1);
      lcd.print("Horas");
    }else{
      lcd.setCursor(7,1);
      lcd.print("Minutos");
    }
    //Edita o horario
    if(analogRead(btn2)>0){
      flagEdit++;
      while(analogRead(btn2)>0);
    }

    if(flagEdit == 3){
      disparaMotor = 1;
      horaConfigurada = hora;
      minutosConfigurados = minutos;
      flagEdit = 0;
      estado = 0;
      segundos = 60;
    }
    
    delay(100);
  }    

  if(horaConfigurada == 0 && minutosConfigurados == 0 && disparaMotor == 1){
    abreMotor();
    fechaMotor();
    disparaMotor = 0;
  }
}
