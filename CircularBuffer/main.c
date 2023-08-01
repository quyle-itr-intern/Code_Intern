#include "stdio.h"
#include "cbuffer.c"

void ShowInforCb(cbuffer_t cb)
{
    printf("\n-------------------------------------------\n");
    printf("Data: ");
    for(uint8_t i = 0; i < cb.size; i++)
        printf("%d ", cb.data[i]);
    printf("\n");
    printf("Space count: %d \nData count: %d \n", cb_space_count(&cb), cb_data_count(&cb) );
    printf("Reader: %d \nWriter: %d\n", cb.reader, cb.writer);
    printf("---------------------------------------------\n");
}

int main()
{
    // Declare a buffer memory to be used as the Circular buffer
    uint8_t buffer[10] = { 0 };
    // Write data into the Circular buffer
    uint8_t read_buffer[5];
    uint8_t read_b[10];
    uint8_t data_to_write[] = {1, 2, 3, 4, 5, 6};

    memset(buffer, 0, 10);

    // Initialize the Circular buffer with a size of 10 bytes
    cbuffer_t cb;
    cb_init(&cb, buffer, 10);

    printf("Write %d: ", cb_write(&cb, data_to_write, sizeof(data_to_write)));
    for(uint8_t i = 0; i < sizeof(data_to_write); i++)
        printf("%d ", data_to_write[i]);

    ShowInforCb(cb);

    printf("Read %d: ", cb_read(&cb, read_buffer, sizeof(read_buffer)));
    for(uint8_t i = 0; i < sizeof(read_buffer); i++)
        printf("%d ", read_buffer[i]);

    ShowInforCb(cb);

    printf("Read %d: ", cb_read(&cb, read_buffer, sizeof(read_buffer)) );
    for(uint8_t i = 0; i < sizeof(read_buffer); i++)
        printf("%d ", read_buffer[i]);

    ShowInforCb(cb);

    printf("Write %d: ", cb_write(&cb, data_to_write, sizeof(data_to_write)));
    for(uint8_t i = 0; i < sizeof(data_to_write); i++)
        printf("%d ", data_to_write[i]);

    ShowInforCb(cb);

    printf("Read %d: ", cb_read(&cb, read_buffer, sizeof(read_buffer)) );
    for(uint8_t i = 0; i < sizeof(read_buffer); i++)
        printf("%d ", read_buffer[i]);

    ShowInforCb(cb);

    printf("Write %d: ", cb_write(&cb, data_to_write, sizeof(data_to_write)));
    for(uint8_t i = 0; i < sizeof(data_to_write); i++)
        printf("%d ", data_to_write[i]);

    ShowInforCb(cb);

    printf("Read %d: ", cb_read(&cb, read_b, sizeof(read_b)) );
    for(uint8_t i = 0; i < sizeof(read_b); i++)
        printf("%d ", read_b[i]);

    ShowInforCb(cb);
    return 1;
}