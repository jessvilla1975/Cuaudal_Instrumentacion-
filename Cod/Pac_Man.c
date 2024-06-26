
#include <18F4550.H>
#include <stdlib.h>
#use delay(clock=4Mhz)
#fuses XT, NOPROTECT, NOPUT, NOWDT, NOBROWNOUT, NOLVP, NOCPD
#include "Mi_LCD.c"
#define WATER_SENSOR_PIN PIN_C0
#define LED_PIN PIN_C1

int16 flow_count = 0;
float flow_rate = 0.0;

#int_ext
void ext_isr() {
   flow_count++;
   ext_int_edge(H_TO_L); // Configurar interrupción externa en flanco de bajada
}


     

void main() {
   setup_counters(RTCC_INTERNAL, RTCC_DIV_1);
   enable_interrupts(INT_EXT); // Habilitar interrupción externa
   enable_interrupts(GLOBAL); // Habilitar interrupciones globales
   ext_int_edge(H_TO_L); // Configurar interrupción externa en flanco de bajada
   
   set_tris_b(0b00000001); // RB0 como entrada
   lcd_init(); //inicializar pantalla
   
   while(1) {
      if(input(WATER_SENSOR_PIN)) {
        
         lcd_gotoxy(1, 2);
         printf(lcd_putc, "Stop: Tanque lleno"); 
         delay_ms(100);
         output_high(LED_PIN);
         delay_ms(100);
         flow_count = 0; 
      } else {
         output_HIGH(LED_PIN);
         delay_ms(100);
         
         flow_rate = (float)flow_count * 2.25; // Convertir a flotante y multiplicar por 2.25 pulsos por segundo
         flow_rate = flow_rate * 60.0; // Convertir a minutos
         flow_rate = flow_rate / 1000.0; // Convertir a litros
         
         printf(lcd_putc, "\f"); 
         lcd_gotoxy(1, 1);
         printf(lcd_putc, "Caudal: %.2f L/min", flow_rate); //imprimir en la lcd 
         flow_count = 0; // iniciar contador en cero
         delay_ms(1000); // Esperar 1 segundo
      }
   }
}
