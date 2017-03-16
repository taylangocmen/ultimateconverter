
#ifndef UC_UTILS_H
#define UC_UTILS_H


#define BINARY(x)    (print_binary(sizeof(x), &x))


//enum ultimate_converter_format {

enum UC_FORMAT {
    bmp,
    jpg,
    png,
};

void abort_(const char* s, ...);

unsigned to_power(unsigned base, unsigned power);

void print_binary(size_t const size, void const* const ptr);

unsigned get_bytes(unsigned i, unsigned j, unsigned char* buffer);


#endif /* UC_UTILS_H */
