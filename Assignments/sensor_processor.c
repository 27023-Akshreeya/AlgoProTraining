#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

# define BIT(n) (1UL << (n))
# define IS_SET(reg,bit) 
# define MAX_SENSORS 5
typedef union 
{
    uint32_t raw;
    struct {
        unsigned int active : 1;
        unsigned int error : 1;
        unsigned int warn : 1;
        unsigned int sensor_id :4 ;
    };
}SensorStatus ;

typedef struct
{
    uint8_t id;
    char name[32];
    float temp;
    SensorStatus status;
}Sensor;

int main()
{
    Sensor *sensorPtr = malloc(MAX_SENSORS * sizeof(Sensor));
    if(sensorPtr == NULL){
        printf("Error : Null pointer!");
        return -1;
    }

    for(int i = 0;i <MAX_SENSORS; i++){
        Sensor *currSensor = sensorPtr + i;
        snprintf(currSensor->name, sizeof(currSensor->name),"Sensor%d",i+1);
    }
}