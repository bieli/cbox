#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbox.h"
#include "cbox_registry.h"
#include "cbox_struct.h"
#include "cbox_array.h"
#include "cbox_debug.h"

// Custom struct: SensorReading
typedef struct {
    char sensor_id[32];
    float value;
    int timestamp;
} SensorReading;

void* sensor_clone(const void* data) {
    SensorReading* copy = malloc(sizeof(SensorReading));
    if (copy) memcpy(copy, data, sizeof(SensorReading));
    return copy;
}

void sensor_destroy(void* data) {
    free(data);
}

void sensor_print(const void* data) {
    const SensorReading* s = (const SensorReading*)data;
    printf("SensorReading{id: %s, value: %.2f, time: %d}\n", s->sensor_id, s->value, s->timestamp);
}

void sensor_serialize_json(const void* data, char* out, size_t maxlen) {
    const SensorReading* s = (const SensorReading*)data;
    snprintf(out, maxlen,
        "{\"type\":\"SensorReading\",\"sensor_id\":\"%s\",\"value\":%.2f,\"timestamp\":%d,\"address\":\"%p\"}",
        s->sensor_id, s->value, s->timestamp, data);
}

CBoxTrait SENSOR_TRAIT = {
    .clone = sensor_clone,
    .destroy = sensor_destroy,
    .print = sensor_print,
    .serialize_json = sensor_serialize_json
};

// Registry-based boxing
CBox* box_from_registry(CBoxRegistry* registry, void* data, size_t size, CBoxType type) {
    CBoxTrait* trait = cbox_registry_get(registry, type);
    if (!trait) {
        printf("[ERROR] No trait registered for type %d\n", type);
        return NULL;
    }
    return cbox_new_with_trait(data, size, type, trait);
}

int main() {
    CBoxRegistry registry;
    cbox_registry_init(&registry);

    // Register traits
    cbox_registry_register(&registry, CBOX_INT, &CBOX_INT_TRAIT);
    cbox_registry_register(&registry, CBOX_FLOAT, &CBOX_FLOAT_TRAIT);
    cbox_registry_register(&registry, CBOX_STRUCT, &SENSOR_TRAIT);

    // Simulated ingestion
    int id = 42;
    float temperature = 23.5f;
    SensorReading reading = { "sensor-A1", 78.9f, 169765 };

    CBox* box1 = box_from_registry(&registry, &id, sizeof(int), CBOX_INT);
    CBox* box2 = box_from_registry(&registry, &temperature, sizeof(float), CBOX_FLOAT);
    CBox* box3 = box_from_registry(&registry, &reading, sizeof(SensorReading), CBOX_STRUCT);

    printf("== Pipeline Output ==\n");
    CBOX_DEBUG_ALL(box1);
    CBOX_DEBUG_ALL(box2);
    CBOX_DEBUG_ALL(box3);

    cbox_free(box1);
    cbox_free(box2);
    cbox_free(box3);

    return 0;
}
