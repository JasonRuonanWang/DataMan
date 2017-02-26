#include "ZfpMan.h"
#include <zfp.h>

ZfpMan::ZfpMan()
    :CompressMan()
{
}

ZfpMan::~ZfpMan(){
}

int ZfpMan::put(const void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<size_t> p_putshape,
        vector<size_t> p_varshape,
        vector<size_t> p_offset,
        size_t p_timestep,
        int p_tolerance,
        int p_priority){

    m_rate = p_tolerance;
    size_t compressed_size;
    compress(
            const_cast<void*>(p_data),
            p_putshape,
            p_dtype,
            compressed_size
            );

    return 0;
}

int ZfpMan::get(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<size_t> p_getshape,
        vector<size_t> p_varshape,
        vector<size_t> p_offset,
        size_t p_timestep
        ){

    return 0;
}

int ZfpMan::get(void *p_data,
        string p_doid,
        string p_var,
        string &p_dtype,
        vector<size_t> &p_varshape,
        size_t &p_timestep
        ){

    return 0;
}

void ZfpMan::flush(){

}

shared_ptr<char> ZfpMan::compress(
        void* p_data,
        vector<size_t> p_shape,
        string p_dtype,
        size_t &p_compressed_size
        ){

    int status = 0;    // return value: 0 = success
    uint dim = 1;
    zfp_type type = zfp_type_none;     // array scalar type
    zfp_field* field;  // array meta data
    zfp_stream* zfp;   // compressed stream
    size_t bufsize;    // byte size of compressed buffer
    bitstream* stream; // bit stream to write to or read from
    size_t zfpsize;    // byte size of compressed stream

    // allocate meta data for the 3D array a[nz][ny][nx]
    if(p_dtype == "int")
        type = zfp_type_int32;
    else if(p_dtype == "long")
        type = zfp_type_int64;
    else if(p_dtype == "float")
        type = zfp_type_float;
    else if(p_dtype == "double")
        type = zfp_type_double;


    switch (p_shape.size()){
        case 3:
            field = zfp_field_3d(p_data, type, p_shape[0], p_shape[1], p_shape[2]);
            dim = 3;
            break;
        case 2:
            field = zfp_field_2d(p_data, type, p_shape[0], p_shape[1]);
            dim = 2;
            break;
        case 1:
            field = zfp_field_1d(p_data, type, p_shape[0]);
            break;
        default:
            field = zfp_field_1d(p_data, type, product(p_shape));
    }

    // allocate meta data for a compressed stream
    zfp = zfp_stream_open(NULL);

    // set compression mode and parameters via one of three functions
    zfp_stream_set_rate(zfp, m_rate, type, dim, 0);
    // zfp_stream_set_precision(zfp, m_precision, type);
    // zfp_stream_set_accuracy(zfp, m_accuracy, type);

    // allocate buffer for compressed data
    bufsize = zfp_stream_maximum_size(zfp, field);
    shared_ptr<char> buffer(new char[bufsize]);

    // associate bit stream with allocated buffer
    stream = stream_open(buffer.get(), bufsize);
    zfp_stream_set_bit_stream(zfp, stream);
    zfp_stream_rewind(zfp);

    // compress or decompress entire array

    zfpsize = zfp_compress(zfp, field);

    if (!zfpsize) {
        fprintf(stderr, "compression failed\n");
        status = 1;
    }
    else{
        fwrite(buffer.get(), 1, zfpsize, stdout);
    }

    // clean up
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    return buffer;
}

shared_ptr<char> ZfpMan::decompress(
        void* p_data,
        vector<size_t> p_shape,
        string p_dtype,
        size_t p_compressed_size
        ){

    int status = 0;    // return value: 0 = success
    uint dim = 1;
    zfp_type type = zfp_type_none;     // array scalar type
    zfp_field* field;  // array meta data
    zfp_stream* zfp;   // compressed stream
    size_t bufsize;    // byte size of compressed buffer
    bitstream* stream; // bit stream to write to or read from
    size_t zfpsize=p_compressed_size;    // byte size of compressed stream

    // allocate meta data for the 3D array a[nz][ny][nx]
    if(p_dtype == "int")
        type = zfp_type_int32;
    else if(p_dtype == "long")
        type = zfp_type_int64;
    else if(p_dtype == "float")
        type = zfp_type_float;
    else if(p_dtype == "double")
        type = zfp_type_double;

    switch (p_shape.size()){
        case 3:
            field = zfp_field_3d(p_data, type, p_shape[0], p_shape[1], p_shape[2]);
            dim = 3;
            break;
        case 2:
            field = zfp_field_2d(p_data, type, p_shape[0], p_shape[1]);
            dim = 2;
            break;
        case 1:
            field = zfp_field_1d(p_data, type, p_shape[0]);
            break;
        default:
            field = zfp_field_1d(p_data, type, product(p_shape));
    }

    // allocate meta data for a compressed stream
    zfp = zfp_stream_open(NULL);

    // set compression mode and parameters via one of three functions
    zfp_stream_set_rate(zfp, m_rate, type, dim, 0);
    // zfp_stream_set_precision(zfp, m_precision, type);
    // zfp_stream_set_accuracy(zfp, m_accuracy, type);

    // allocate buffer for compressed data
    bufsize = zfp_stream_maximum_size(zfp, field);
    shared_ptr<char> buffer(new char[bufsize]);

    // associate bit stream with allocated buffer
    stream = stream_open(buffer.get(), bufsize);
    zfp_stream_set_bit_stream(zfp, stream);
    zfp_stream_rewind(zfp);

    // compress or decompress entire array

    if (!zfp_decompress(zfp, field)) {
      fprintf(stderr, "decompression failed\n");
      status = 1;
    }

    // clean up
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    return buffer;
}
