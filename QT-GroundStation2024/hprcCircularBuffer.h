#ifndef HPRCCIRCULARBUFFER_H
#define HPRCCIRCULARBUFFER_H

#include <iostream>

#define CIRCULAR_BUFFER_LEN 35

typedef struct {
    float value;
    float time;
} graphPoint;

typedef struct {
    unsigned int length;
    unsigned int dataSize;
    graphPoint *dataPtr;
    graphPoint *data;

    graphPoint *maxValue;
    graphPoint *minValue;
} GraphPointCircularBuffer;


inline graphPoint *graphPointCircularBufferGetValueAtIndex(GraphPointCircularBuffer *buffer, unsigned int index)
{
    unsigned int currentIndex = (buffer->dataPtr - buffer->data);
    return &(buffer->data[(currentIndex + index) % buffer->length]);
}

inline GraphPointCircularBuffer *graphPointCirularBufferCreate(unsigned int length)
{
    unsigned int size = sizeof(graphPoint);
    GraphPointCircularBuffer* buffer = (GraphPointCircularBuffer*)malloc(sizeof(GraphPointCircularBuffer) + size * length);

    buffer->length = length;
    buffer->dataSize = size;

    buffer->data = (graphPoint *)calloc(size, length);

    buffer->dataPtr = &buffer->data[0];
    buffer->maxValue = buffer->dataPtr;
    buffer->minValue = buffer->dataPtr;

    return buffer;
}

inline graphPoint *graphPointCircularBufferGetMaxValue(GraphPointCircularBuffer *buffer)
{
    graphPoint *maxValue = graphPointCircularBufferGetValueAtIndex(buffer, 0);
    for (int i = 0; i < buffer->length; i++)
    {
        graphPoint *graphPoint = graphPointCircularBufferGetValueAtIndex(buffer, i);
        if(maxValue->value < graphPoint->value)
        {
            maxValue = graphPoint;
        }
    }

    return maxValue;
}

inline graphPoint *graphPointCircularBufferGetMinValue(GraphPointCircularBuffer *buffer)
{
    graphPoint *minValue = graphPointCircularBufferGetValueAtIndex(buffer, 0);
    for (int i = 0; i < buffer->length; i++)
    {
        graphPoint *graphPoint = graphPointCircularBufferGetValueAtIndex(buffer, i);
        if(minValue->value > graphPoint->value)
        {
            minValue = graphPoint;
        }
    }

    return minValue;
}

inline void graphPointCirularBufferAdd(GraphPointCircularBuffer *buffer, graphPoint data)
{

    bool needNewMaxValue = false;
    bool needNewMinValue = false;


    if (buffer->dataPtr == buffer->maxValue)
    {
        needNewMaxValue = true;
    }
    else if (buffer->dataPtr == buffer->minValue)
    {
        needNewMinValue = true;
    }
    else if (data.value >= buffer->maxValue->value)
    {
        buffer->maxValue = buffer->dataPtr;
    }
    else if (data.value <= buffer->minValue->value)
    {
        buffer->minValue = buffer->dataPtr;
    }

    *buffer->dataPtr = data;

    if(needNewMaxValue)
    {
        buffer->maxValue = graphPointCircularBufferGetMaxValue(buffer);
    }
    else if (needNewMinValue)
    {
        buffer->minValue = graphPointCircularBufferGetMinValue(buffer);
    }


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
