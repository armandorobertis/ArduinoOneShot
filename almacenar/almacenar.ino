#include <WiegandNG.h>

//#include <Time.h>
#include <TimeLib.h>

int led = 13;
int contador;
int var;
int var2;
int entrar;
int esta;
unsigned long matriz[10][2]; // declaracion de variables globales 
int tiempo;
int i;
int j;
int tiempo_borrado;
unsigned long miArray[10];// = {};  declaracion de variables globales 
unsigned long currentKey;
unsigned long numeros_de_bit;

WiegandNG wg;

void setup() {
  contador = 0;

  Serial.begin(9600);  
 
//  wg.begin(18, 19);
//  wg.begin(20, 21);

 // unsigned int pinD0 = 2;
  //unsigned int pinD1 = 3;
  
  unsigned int wiegandbits = 48;
  unsigned int packetGap = 15;  
  
  wg.begin(18, 19, wiegandbits, packetGap);
  wg.begin(20, 21, wiegandbits, packetGap);
/*
  if(!wg.begin(pinD0, pinD1, wiegandbits, packetGap)) {
    Serial.println("Out of memory!");
  }
  */
  setTime(0,0,0,0,1,2019); // seteo de hora y fecha

  pinMode(18, INPUT); //Dato 0 D0 Lector 1
  digitalWrite(18, HIGH);  // Habilitar resistencia pull-up
  pinMode(19, INPUT);// Dato 1 D1 Lector 1
  digitalWrite(19, HIGH);  // Habilitar resistencia pull-up

  pinMode(20, INPUT); //Dato 0 D0 Lector 1
  digitalWrite(20, HIGH);  // Habilitar resistencia pull-up
  pinMode(21, INPUT);// Dato 1 D1 Lector 1
  digitalWrite(21, HIGH);  // Habilitar resistencia pull-up
 


  Serial.print("tamaño de matriz ");
  Serial.println(sizeof(matriz)/4);
  //(sizeof(miArray)/4)
}


void PrintBinary(WiegandNG &tempwg) {
  volatile unsigned char *buffer=tempwg.getRawData();
  unsigned int bufferSize = tempwg.getBufferSize();
  unsigned int countedBits = tempwg.getBitCounted();

  unsigned int countedBytes = (countedBits/8);
  if ((countedBits % 8)>0) countedBytes++;
  // unsigned int bitsUsed = countedBytes * 8;
  
  for (unsigned int i=bufferSize-countedBytes; i< bufferSize;i++) {
    unsigned char bufByte=buffer[i];
    for(int x=0; x<8;x++) {
      if ( (((bufferSize-i) *8)-x) <= countedBits) {
        if((bufByte & 0x80)) {
          Serial.print("1");
        }
        else {
          Serial.print("0");
        }
      }
      bufByte<<=1;
    }
  }
  Serial.println();
}

void loop() {

  if(wg.available()) {
    wg.pause();     // pause Wiegand pin interrupts
    Serial.print("Bits=");
    Serial.println(wg.getBitCounted()); // display the number of bits counted
    Serial.print("RAW Binary=");
    PrintBinary(wg); // display raw data in binary form, raw data inclusive of PARITY
    wg.clear(); // compulsory to call clear() to enable interrupts for subsequent data
  }

  return;


  
  time_t t = now();//Declaramos la variable time_t t
 // Serial.println("prueba");
  //Serial.print(minute(t));

  if(wg.available())
  {


    //Serial.print("tamaño de matriz ");
    //Serial.println(sizeof(matriz)/4);

      unsigned long currentKey = wg.getBitCounted();
  //  unsigned long currentKey = wg.getCode();
   // unsigned long numeros_de_bit = wg.nBit();
    //currentKey = wg.getCode();
    Serial.println(currentKey); 
  Serial.print("numeros de bit contados: ");
    Serial.println(numeros_de_bit);
    tiempo = minute(t);                 // (minutos)minute , (hora)    si se quiere borrar en mas de 59 minutos usar en horas

                    
    Serial.println(currentKey);   

    agregarKey(currentKey,tiempo);

    borrado_por_tiempo(tiempo);         //  llamado a funcion de borrado condicionado por tiempo, se debe independizar en el loop

//    Serial.println(currentKey);
    if(entrar==1){Serial.println("ENTRAR");       
    }else{
      Serial.println("NO ENTRAR");
    }
     /*  
    Serial.print("Wiegand HEX = ");
    Serial.print(wg.getCode(),HEX);
    Serial.print(", DECIMAL = ");
    Serial.print(wg.getCode());
    Serial.print(", Type W");
    Serial.println(wg.getWiegandType());   
*/
    Serial.print("contador: -->");
    Serial.println(contador);
    Serial.println("entrada");
    Serial.println("comienzo");

    for(i=0;i<((sizeof(matriz))/8); i++){
      
      Serial.print(matriz[i][0]);
      Serial.print(" ");
      Serial.print(matriz[i][1]);
      Serial.print(" ");
      Serial.println(minute(t));

        if (matriz[i][1]>0 and (minute(t))+2 > ((matriz[i][1])+1))
        {
          Serial.println(" Hay que resetear ");
        }
     
     Serial.println(" Siguiente ");
     }
    Serial.println("FIN");
  }
}



void agregarKey(unsigned long currentKey,int tiempo) 
  {
    Serial.println(currentKey);   
  esta = 0;
  for (var = 0; var < (sizeof(miArray)/4); var++){
    if ((matriz[var][0])==(currentKey)){
      //Serial.print(matriz[var][1]);
      esta = 1;
      Serial.println(var);
    }
  }
  if (esta == 1)
  {
    Serial.println("si hay un registro ");
    entrar=0;  // la persona no debe pasar porque hay un registro
  }else{
    Serial.println("No hay un registro y se registro 11111 ");

    for (var = 0; var < (sizeof(miArray)/4); var++)
        {
        if ((matriz[var][0])== 0)
            {
            Serial.println(currentKey);   
            matriz[var][0] =  currentKey;
            matriz[var][1] = tiempo;
            Serial.println(matriz[var][0]);   
            Serial.println("No hay un registro y se registro");
            break;
            }
        }
    
    // matriz[contador][0] = currentKey;
    // matriz[contador][1] = tiempo;
    // contador ++;
    // Serial.println("No hay un registro y se registro");
     entrar=1;
  }
  Serial.print("contador: >>>>>");
  Serial.println(contador);
}

void borrado_por_tiempo(int tiempo)
  {
    for (var = 0; var < (sizeof(miArray)/4); var++)
    {
      if (((matriz[var][1])+2)<(tiempo))
        {
          matriz[var][0] = 0;
          matriz[var][1] = 0;
        //Serial.println(var);
        }
    }

  }
