

const int kBufSize= 10;
int *int_buffer = new int[kBufSize];

//..........

if (NULL != int_buffer)
{
    delete []int_buffer;
    int_buffer = NULL;
}
