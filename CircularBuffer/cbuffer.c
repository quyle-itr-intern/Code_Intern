#include "cbuffer.h"

void cb_init(cbuffer_t *cb, void *buf, uint32_t size)
{
    cb->data = buf;
    cb->reader = 0;
    cb->writer = 0;
    cb->size = size;
    cb->overflow = 0;
}

void cb_clear(cbuffer_t *cb)
{
    cb->data = NULL;
    cb->reader = 0;
    cb->writer = 0;
    cb->size = 0;
    cb->overflow = 0;
}

uint32_t cb_read(cbuffer_t *cb, void *buf, uint32_t nbytes)
{
    uint32_t u32count = nbytes;
    if( cb->writer == cb->reader )
        return 0;
    
    while ( nbytes )
    {
        if( cb->reader == cb->writer )
            break;
        else
        {
            cb->reader++;
            cb->reader %= cb->size;
            *(uint8_t *) buf++ = *( cb->data + cb->reader);
            *( cb->data + cb->reader) = 0;
            nbytes--;
        }
    }
    cb->overflow = 0;
    return u32count - nbytes;
}

uint32_t cb_write(cbuffer_t *cb, void *buf, uint32_t nbytes)
{
    uint32_t u32count = nbytes;
    if( nbytes < 0)
        return 0;

    while ( !cb->overflow && nbytes )
    {
        if( ( (cb->writer + 1) % cb->size ) == cb->reader )
            cb->overflow = 1;
        else
        {
            cb->writer++;
            cb->writer %= cb->size;
            *( cb->data + cb->writer) = *(uint8_t *) buf++; 
            nbytes--;
        }
    }
    return u32count - nbytes;
}

uint32_t cb_data_count(cbuffer_t *cb)
{
    if( cb->reader == cb->writer )
        return 0;
    if( cb->writer > cb->reader )   
        return cb->writer - cb->reader;
    else
        return cb->size - cb->reader + cb->writer;
}

uint32_t cb_space_count(cbuffer_t *cb)
{
    return cb->size - 1 - cb_data_count(cb);
}
