/* ###################################################################
**     Filename    : ProcessorExpert.c
**     Project     : ProcessorExpert
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-12-19, 15:43, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file ProcessorExpert.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
**  @{
*/         
/* MODULE ProcessorExpert */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "PC.h"
#include "ASerialLdd1.h"
#include "ESP.h"
#include "ASerialLdd2.h"
#include "MCUC1.h"
#include "LEDR.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "LEDG.h"
#include "LEDpin2.h"
#include "BitIoLdd2.h"
#include "LEDB.h"
#include "LEDpin3.h"
#include "BitIoLdd3.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include  <string.h>

/* User includes (#include below this line is not maintained by Processor Expert) */
volatile char rxPC = 0;
volatile uint8_t buf[100] = {0};
volatile uint8_t pos = 0;
volatile uint8_t rcv[100] = {0};
volatile char msg = 0;
uint8_t cmd[100] = {0};

uint8_t scan1,scan2;
uint8_t top[50] = {0};
uint8_t par[50] = {0};
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */

void send_cmd(uint8_t *s)
{// Envia mensagem para a placa ESP, recebe uma string s como par�metro
	while(*s){
			while(ESP_SendChar(*s));
			s++;
		}
	
}

void sendPC(uint8_t *s)
{// Envia mensagem contida na string s para PC
	while(*s){
			while(PC_SendChar(*s));
			s++;
		}
}

////////////////////////////////////////////////////////
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  
  
  // CONFIGURANDO ESP01
  ////////////////////////////////////////////////////////
  strcpy(cmd,"CONNWIFI \"nomedarede\",\"senhadarede\"\r\n");
  send_cmd(cmd); //< inserir nome da rede e senha
  while(!msg); // < Espera ESP enviar mensagem
  msg = 0; // Resetando flag
  if(strcmp(rcv,"CONNECT WIFI")){
  	// retorna diferente de 0 se nao for igual
  	sendPC("Problemas ao enviar mensagem \r\n");
  }
  else{
  	// sucesso ao conectar
	strcpy(cmd,"CONNMQTT \"xxx.xxx.xxx.xxx\",porta,\"clientID\"\r\n");
  	send_cmd(cmd); // < xxxx � IP e porta � do broker do broker | client ID � mac adress
  	while(!msg); // < Espera ESP enviar mensagem
  	msg = 0;
  	if(strcmp(rcv,"CONNECT MQTT")){
  		// se for diferente de 0 deu problema
  		sendPC("Problemas ao conectar no ESP \r\n");  
  	}
  	else{
  		// CONECTOU
  		send_cmd("SUBSCRIBE \"topico\"\r\n"); 
  		while(!msg); // < Espera ESP enviar mensagem
  		msg = 0;
  		
  		send_cmd("SUBSCRIBE \"topico\"\r\n"); 
		while(!msg); // < Espera ESP enviar mensagem
		msg = 0;
		
		send_cmd("SUBSCRIBE \"topico\"\r\n");
		while(!msg); // < Espera ESP enviar mensagem
		msg = 0;
		
  		// -> ENVIAR MENSAGEM PRA UART0 QUE EST� TUDO OK NA CONFIGURACAO
		sendPC("Configuracao dos LEDS|TOPICOS esta OK \r\n"); 
  		
  	}
  }
////////////////////////////////////////////////
  
  /* Write your code here */
  /* For example: for(;;) { } */
  for(;;){
    	  // -> VERIFICAR SE H� MENSAGEM NO ESP, SEPARAR STRINGS DE TOPICO E MENSAGEM. SE ERRO -> ENVIAR UART0
	  	  // SE A MENSAGEM RECEBIDA FOR UMA DOS TOPICOS ASSINADOS NA CONFIG ACIMA, ATUAR NOS LEDS
	  if(msg){
		  msg = 0;
		  if (strFind(rcv,"MESSAGE") == -1){ // ERRO na mensagem recebida (nao encontramos message
			  sendPC(rcv);
		  }
		  else{// scan da mensagem 
			  scan1 = 0;
			  while (rcv[scan1] != '['){ // percorre ate o come�o da string do topico
				  scan1++;
			  }
			  scan1++;
			  scan2 = 0;
			  while(rcv[scan1] != ']'){ // insere string do em topico
				  top[scan2] = rcv[scan1];
				  scan2++;
				  scan1++;
			  }
			  top[scan2] = 0;
			  
			  while(rcv[scan1] != '['){ // incrementa ate chegar em mensagem
				  scan1++;
			  }
			  scan1++;
			  scan2 = 0;
			  
			  while(rcv[scan1] != ']'){
				  par[scan2] = rcv[scan1];
				  scan1++;
				  scan2++;
			  }
			  par[scan2] = 0;
			  
			  //TOPICO EM TOP E MENSAGEM EM PAR
			  if (strcmp(top,"topico") == 0){
				  if (strcmp(par,"ON") == 0){
					  // ligar o LED VERDE 
					  LEDG_On();
					  strcpy(cmd,"PUBLISH \"topico\",\"LEDG ON\"\r\n");
					  send_cmd(cmd);
					  while(!msg);
					  msg = 0;
				  	  }
				  else{
					  LEDG_Off();
					  strcpy(cmd,"PUBLISH \"topico\",\"LEDG OFF\"\r\n");
					  send_cmd(cmd);
					  while(!msg);
					  msg = 0;
				  	  }
			  	  }
			  else if (strcmp(top,"topico") == 0){
				  if (strcmp(par,"ON") == 0){
					  // ligar o LED AZUL
					  LEDB_On();
					  strcpy(cmd,"PUBLISH \"topico\",\"LEDB ON\"\r\n");
					  send_cmd(cmd);
					  while(!msg);
					  msg = 0;
				  	  }
				  else{
					  LEDB_Off();
					  strcpy(cmd,"PUBLISH \"topico\",\"LEDB OFF\"\r\n");
					  send_cmd(cmd);
					  while(!msg);
					  msg = 0;
				  	  }
			  	  }
			  else if (strcmp(top,"topico") == 0){
			  				  if (strcmp(par,"ON") == 0){
			  					  // ligar o LED AZUL
			  					  LEDB_On();
			  					  strcpy(cmd,"PUBLISH \"topico\",\"LEDR ON\"\r\n");
			  					  send_cmd(cmd);
			  					  while(!msg);
			  					  msg = 0;
			  				  	  }
			  				  else{
			  					  LEDB_Off();
			  					  strcpy(cmd,"PUBLISH \"topico\",\"LEDR OFF\"\r\n");
			  					  send_cmd(cmd);
			  					  while(!msg);
			  					  msg = 0;
			  				  	  }
			  			  	  }
			  
			  
		  	  }
	  	  }
	  
      }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.2 [05.07]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
