extern "C"
{
#pragma function(memset)
    void* memset(void* dest, int c, size_t count)
    {
        char* bytes = (char*)dest;
        while (count--)
        {
            *bytes++ = (char)c;
        }
        return dest;
    }

#pragma function(memcpy)
    void* memcpy(void* dest, const void* src, size_t count)
    {
        char* dest8 = (char*)dest;
        const char* src8 = (const char*)src;
        while (count--)
        {
            *dest8++ = *src8++;
        }
        return dest;
    }

    int atoi(const char *str)
    {
        int value = 0;
        while (true)
        {
            char c = *str;
            if (c == 0) break;
            if (c >= '0' && c <= '9')
            {
                value *= 10;
                value += (c - '0');
            }
            str++;
        }
        return value;
    }
}
