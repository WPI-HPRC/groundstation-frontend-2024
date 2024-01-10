#ifndef HPRCCIRCULARBUFFER_H
#define HPRCCIRCULARBUFFER_H

#include <iostream>

#define CIRCULAR_BUFFER_LEN 35

typedef struct {
    float value;
    float time;
} GraphPoint;

typedef struct {
    unsigned int length;
    unsigned int dataSize;
    GraphPoint *dataPtr;
    GraphPoint *data;

    GraphPoint *maxValue;
    GraphPoint *minValue;
} GraphPointCircularBuffer;


inline GraphPoint *graphPointCircularBufferGetValueAtIndex(GraphPointCircularBuffer *buffer, unsigned int index)
{
    // (buffer->dataPtr - buffer->data) is the "current 0 index", add the desired index and mod it by the buffer length to make it wrap around
    return &(buffer->data[((buffer->dataPtr - buffer->data) + index) % buffer->length]);
}

inline GraphPointCircularBuffer *graphPointCirularBufferCreate(unsigned int length)
{
    unsigned int size = sizeof(GraphPoint);
    GraphPointCircularBuffer* buffer = (GraphPointCircularBuffer*)malloc(sizeof(GraphPointCircularBuffer) + size * length);

    buffer->length = length;
    buffer->dataSize = size;

    buffer->data = (GraphPoint *)calloc(size, length);

    buffer->dataPtr = &buffer->data[0];
    buffer->maxValue = buffer->dataPtr;
    buffer->minValue = buffer->dataPtr;

    return buffer;
}

inline float graphPointCircularBufferGetMaxValue(GraphPointCircularBuffer *buffer)
{
    float maxValue = graphPointCircularBufferGetValueAtIndex(buffer, 0)->value;
    for (int i = 1; i < buffer->length; i++)
    {
        maxValue = fmax(maxValue, graphPointCircularBufferGetValueAtIndex(buffer, i)->value);
    }

    return maxValue;
}

inline float graphPointCircularBufferGetMinValue(GraphPointCircularBuffer *buffer)
{
    float minValue = graphPointCircularBufferGetValueAtIndex(buffer, 0)->value;
    for (int i = 1; i < buffer->length; i++)
    {
        minValue = fmin(minValue, graphPointCircularBufferGetValueAtIndex(buffer, i)->value);
    }

    return minValue;
}

inline void graphPointCirularBufferAdd(GraphPointCircularBuffer *buffer, GraphPoint data)
{
    *buffer->dataPtr = data;

    if (buffer->dataPtr == &buffer->data[buffer->length - 1])
    {
        buffer->dataPtr = &buffer->data[0];
    }
    else
    {
        buffer->dataPtr++;
    }
}

#endif // HPRCCIRCULARBUFFER_H
