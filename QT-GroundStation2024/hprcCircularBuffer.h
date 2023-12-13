#ifndef HPRCCIRCULARBUFFER_H
#define HPRCCIRCULARBUFFER_H

#include <mainwindow.h>
#include <iostream>

typedef struct {
    unsigned int length;
    unsigned int dataSize;
    MainWindow::graphPoint *dataPtr;
    MainWindow::graphPoint *data;
} GraphPointCircularBuffer;


GraphPointCircularBuffer *createGraphPointCirularBuffer(unsigned int length)
{
    unsigned int size = sizeof(MainWindow::graphPoint);
    GraphPointCircularBuffer* buffer = (GraphPointCircularBuffer*)malloc(sizeof(GraphPointCircularBuffer) + size * length);

    buffer->length = length;
    buffer->dataSize = size;

    buffer->data = (MainWindow::graphPoint *)calloc(size, length);

    buffer->dataPtr = &buffer->data[0];

    return buffer;
}

void graphPointCirularBufferAdd(GraphPointCircularBuffer *buffer, MainWindow::graphPoint data)
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

MainWindow::graphPoint graphPointCircularBufferGetValueAtIndex(GraphPointCircularBuffer *buffer, unsigned int index)
{
    unsigned int currentIndex = (buffer->dataPtr - buffer->data);
    return (buffer->data[(currentIndex + index) % buffer->length]);
}

#endif // HPRCCIRCULARBUFFER_H
