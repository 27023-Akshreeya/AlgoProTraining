#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

# define BIT(n) (1UL << (n))
# define IS_SET(reg,bit) 
# define MAX_SENSORS 5

# define BIT_ACTIVE 0
# define BIT_ERROR 1
# define BIT_WARN 2

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

float randomTemperature(float minTemp, float maxTemp){
    float scale = rand() / (float) RAND_MAX;
    return minTemp + scale * (maxTemp - minTemp);
}

uint32_t randomStatus(uint8_t id) {
    uint32_t flags = rand() & 0x07;
    uint32_t shifted_id = (id & 0x0F) << 3;
    return flags | shifted_id;
}

Sensor* find_sensor(Sensor *arr, int count, uint8_t id)
{
    if(arr == NULL){
        return NULL;
    }
    for(int i = 0 ; i < count; i++)
    {
        if(arr[i].id == id){
            return &arr;
        }
    }
    return NULL;
}

void* my_memcpy(void *dst, const void *src, size_t n)
{
    unsigned char *d = (unsigned char*)dst;
    const unsigned char *s = (const unsigned char*) src;

    for(size_t i = 0; i < n ; i++)
    {
        *(d+i) = *(s+i);
    }
    return dst;
}

int main()
{
    Sensor *sensorPtr = malloc(MAX_SENSORS * sizeof(Sensor));
    if(sensorPtr == NULL){
        printf("Error : Null pointer!");
        return -1;
    }

        uint32_t sample_flags[MAX_SENSORS] = {
        BIT(BIT_ACTIVE),
        BIT(BIT_ACTIVE) | BIT(BIT_WARN),
        BIT(BIT_ERROR),
        BIT(BIT_ACTIVE) | BIT(BIT_ERROR) | BIT(BIT_WARN),
        BIT(BIT_ERROR) | BIT(BIT_WARN)
    };

    for(int i = 0;i <MAX_SENSORS; i++){
        Sensor *currSensor = sensorPtr + i;
        snprintf(currSensor->name, sizeof(currSensor->name),"Sensor%d",i+1);
        currSensor->temp = randomTemperature(0.0f,80.0f);
        currSensor->id = i +1;
        currSensor->status.raw = sample_flags[i] | ((currSensor->id << 3) & (0x0F << 3));
    }
}