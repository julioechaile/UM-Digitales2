#include "test.h"
#include "FreeRTOSConfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>



#include <stdio.h>


enum estado
{
    estado_avanzar,
    estado_derecha,
    estado_izquierda,
    estado_reversa,
};

struct entradas
{
    int izq;
    int der;
    int ret;
};

struct salidas
{
    bool mot1;
    bool mot2;
    bool mota;
    bool motb;
};
struct direcciones
{   int dir_izq;
    int dir_der;
    int dir_ret;

};
struct robot
{
    enum estado status;
    struct entradas sensor;
    struct salidas motor;
    struct direcciones add;
};



void init(struct robot *robot_init) // aca la maquina se llama robot_init
{
    printf("Iniciando robot\n\r");
    robot_init->status = estado_avanzar;
    robot_init->motor.mot1 = 1;
    robot_init->motor.mot2 = 0;
    robot_init->motor.mota = 1;
    robot_init->motor.motb = 0;
    robot_init->add.dir_izq =0;
    robot_init->add.dir_der =1;
    robot_init->add.dir_ret =2;
}

void robot_update(struct robot *robot_update)  // aca la maquina se llama robot_update
{
    printf("Actualizando robot\n\r");
    robot_update->sensor.izq= hal_read_pin(robot_update->add.dir_izq);
    robot_update->sensor.der= hal_read_pin(robot_update->add.dir_der);
    robot_update->sensor.ret= hal_read_pin(robot_update->add.dir_ret);
    switch (robot_update->status) {  //revisa los estados posibles del robot
    case estado_avanzar: 
        if (robot_update->sensor.izq ==1)
        {  
            robot_update->status = estado_izquierda;
            
        }else if (robot_update->sensor.der==1)
        {
            robot_update->status = estado_derecha;
            
        }else if (robot_update->sensor.ret==1)
        {
            robot_update->status = estado_reversa;
            
        }

        robot_update->motor.mot1 = 1;
        robot_update->motor.mot2 = 0;
        robot_update->motor.mota = 1;
        robot_update->motor.motb = 0;
        printf("Avanzando\n\r");
        break;
    case estado_derecha:
        robot_update->motor.mot1 = 1;
        robot_update->motor.mot2 = 0;
        robot_update->motor.mota = 0;
        robot_update->motor.motb = 1;
        printf("Giro derecha\n\r");
  

        if (robot_update->sensor.der==1)
        { 
        break;          
        }else
        {
            robot_update->status = estado_avanzar;
        }
        break;  
    case estado_izquierda:
        
        robot_update->motor.mot1 = 0;
        robot_update->motor.mot2 = 1;
        robot_update->motor.mota = 1;
        robot_update->motor.motb = 0;
        printf("Giro izquierda\n\r");

        if (robot_update->sensor.izq==1)
        {
        break;
        }else
        {   
            robot_update->status = estado_avanzar;
        }
         break;
    case estado_reversa:
        robot_update->motor.mot1 = 0;
        robot_update->motor.mot2 = 1;
        robot_update->motor.mota = 0;
        robot_update->motor.motb = 1;
        printf("Retrocediendo\n\r");
        if (robot_update->sensor.ret==1)
        {
        break;    
        }else
        {
            robot_update->status = estado_avanzar;
        }
        break;
        default:
        robot_update->status = estado_avanzar;
        break;
    }
}




void TestTask(void *notUsed)
{
  int contador = 0;
  struct robot maquinas;
  while (1)
    {
        robot_update(&maquinas);
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("tick %d\n\r", contador);
        contador++;
    }
      
  vTaskDelete(NULL);
  return 0;

}

void TestCreate(void)
{
  struct robot maquina;
  init(&maquina);
  LPRINTF("[TEST] Creating task\n\r");  
  BaseType_t res = xTaskCreate(TestTask,(const char *)"test",configMINIMAL_STACK_SIZE,NULL,2,NULL);
}
