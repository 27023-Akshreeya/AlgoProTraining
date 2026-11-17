#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

# define BIT(n) (1UL << (n))
# define IS_SET(reg,bit) (((reg) & BIT(bit))!=0)
# define MAX_SENSORS 5
# define BIT_ACTIVE 0
# define BIT_ERROR 1
# define BIT_WARN 2
# define ID_SHIFT 3
# define ID_MASK (0x0F << ID_SHIFT)
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

typedef enum{
    FROZEN,
    NORMAL,
    WARM,
    HOT,
    CRITICAL
}temperature;

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
        if(arr[i].id == id)
        {
            return &(arr[i]);
        }
    }
    return NULL;
}

char *classify_temp(float temp){
    temperature tempState;

    if( temp < 0.0f)
    {
        tempState = FROZEN;
    }
    else if( temp <= 35.0f)
    {
        tempState = NORMAL;
    }
    else if( temp <= 55.0f)
    {
        tempState = WARM;
    }
    else if(temp <= 80.0f)
    {
        tempState = HOT;
    }
    else
    {
        tempState = CRITICAL;
    }

    switch (tempState)
    {
    case FROZEN:
        return "Frozen";
    case NORMAL:
        return "Normal";
    case WARM:
        return "Warm";
    case HOT:
        return "Hot";
    case CRITICAL:
        return "Critical";
    default:
        return "Invalid";
    }
    
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

void foreach_sensor(Sensor *arr, int n, void(*cb)(const Sensor*)){
    for(int i =0 ;i<n;i++){
        cb(arr+i);
    }
}

void cb_print(const Sensor *s) {
    printf("ID: %u\nName: %-10s\nTemperature: %6.2f C (%s)\nRaw: 0x%08X\n",
           s->id, s->name, s->temp, classify_temp(s->temp), s->status.raw);
}

void cb_count_errors(const Sensor* s){
    static int errors = 0;
    if (IS_SET(s->status.raw, BIT_ERROR))
    {
        errors++;
    }
    printf("ID: %d Error:%d\n", s->id, errors);
}

void activate_sensor(Sensor *s, uint8_t id) {
    s->status.raw |= BIT(BIT_ACTIVE);         
    s->status.raw &= ~ID_MASK;                  
    s->status.raw |= ((id << ID_SHIFT) & ID_MASK); 
}

void set_error(Sensor *s) {
    s->status.raw |= BIT(BIT_ERROR);
}

void clear_error(Sensor *s) {
    s->status.raw &= ~BIT(BIT_ERROR);
}

int is_critical(const Sensor *s) {
    return (IS_SET(s->status.raw, BIT_ERROR)) && (IS_SET(s->status.raw, BIT_WARN));
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

    Sensor DummySensor;
        printf("Sizes -> id: %zu, name: %zu, temp: %zu, status: %zu, total: %zu\n\n",
           sizeof(DummySensor.id), sizeof(DummySensor.name), sizeof(DummySensor.temp),
           sizeof(DummySensor.status), sizeof(Sensor));

    printf("searching sensor\n");
    Sensor *foundSensor = find_sensor(sensorPtr, MAX_SENSORS, 3);
    if(foundSensor != NULL){
        printf("Sensor found: ID: %u, Name: %s, Temp: %.2f, Status: 0x%08X\n",
               foundSensor->id, foundSensor->name, foundSensor->temp, foundSensor->status.raw);
    } else {
        printf("Sensor not found\n");
    }
    Sensor backup;
    my_memcpy(&backup, sensorPtr, sizeof(Sensor));
    printf("\nBackup Sensor -> ID: %u, Name: %s, Temp: %.2f, Status: 0x%08X\n",
           backup.id, backup.name, backup.temp, backup.status.raw);

    printf("\nIterating over sensors:\n");
    foreach_sensor(sensorPtr, MAX_SENSORS, cb_print);

    printf("\nCounting errors:\n");
    foreach_sensor(sensorPtr, MAX_SENSORS, cb_count_errors);

    printf("Testing activate_sensor, set_error, clear_error, and is_critical functions:\n");
    Sensor *testSensor = sensorPtr + 2;
    printf("Initial ID: %u, Status: 0x%08X\n", testSensor->id, testSensor->status.raw);
    activate_sensor(testSensor, 10);
    printf("After activating sensor with ID: %u, Status: 0x%08X\n", testSensor->id, testSensor->status.raw);
    set_error(testSensor);
    printf("After setting error: ID: %u, Status: 0x%08X\n", testSensor->id, testSensor->status.raw);
    clear_error(testSensor);
    printf("After clearing error: ID: %u, Status: 0x%08X\n", testSensor->id, testSensor->status.raw);
    if (is_critical(testSensor)) {
        printf("Sensor ID: %u is in a critical state.\n", testSensor->id);
    } else {
        printf("Sensor ID: %u is NOT in a critical state.\n", testSensor->id);
    }

    free(sensorPtr);
    return 0;
}