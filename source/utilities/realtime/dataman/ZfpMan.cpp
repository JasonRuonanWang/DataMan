#include "utilities/realtime/dataman/ZfpMan.h"
#include <zfp.h>

namespace adios
{
namespace realtime
{

ZfpMan::ZfpMan()
    :CompressMan()
{
}

ZfpMan::~ZfpMan(){
}

int ZfpMan::init(json p_jmsg){
    return 0;
}

int ZfpMan::put(const void *p_data, json p_jmsg){
    put_begin(p_data, p_jmsg);

    void *compressed_data = NULL;
    if (check_json(p_jmsg, {"doid", "var", "dtype", "putshape"}, "ZfpMan")){
        if (p_jmsg["compression_rate"] == nullptr){
            p_jmsg["compression_rate"] = 4;
        }
        compressed_data = compress(const_cast<void*>(p_data), p_jmsg);
    }

    put_end(compressed_data, p_jmsg);
    if(compressed_data) free(compressed_data);
    return 0;
}

int ZfpMan::get(void *p_data, json &p_jmsg){
    return 0;
}

void ZfpMan::flush(){
    flush_next();
}

void* ZfpMan::compress(void* p_data, json &p_jmsg){

    string dtype = p_jmsg["dtype"];
    vector<size_t> shape = p_jmsg["putshape"].get<vector<size_t>>();
    int compression_rate = p_jmsg["compression_rate"].get<int>();

    int status = 0;    // return value: 0 = success
    uint dim = 1;
    zfp_type type = zfp_type_none;     // array scalar type
    zfp_field* field;  // array meta data
    zfp_stream* zfp;   // compressed stream
    size_t bufsize;    // byte size of compressed buffer
    bitstream* stream; // bit stream to write to or read from
    size_t zfpsize;    // byte size of compressed stream

    // allocate meta data for the 3D array a[nz][ny][nx]
    if(dtype == "int"){
        type = zfp_type_int32;
    }
    else if(dtype == "long"){
        type = zfp_type_int64;
    }
    else if(dtype == "float"){
        type = zfp_type_float;
    }
    else if(dtype == "double"){
        type = zfp_type_double;
    }

    switch (shape.size()){
        case 3:
            field = zfp_field_3d(p_data, type, shape[0], shape[1], shape[2]);
            dim = 3;
            break;
        case 2:
            field = zfp_field_2d(p_data, type, shape[0], shape[1]);
            dim = 2;
            break;
        case 1:
            field = zfp_field_1d(p_data, type, shape[0]);
            break;
        default:
            field = zfp_field_1d(p_data, type, product(shape));
    }

    // allocate meta data for a compressed stream
    zfp = zfp_stream_open(NULL);

    // set compression mode and parameters via one of three functions
    zfp_stream_set_rate(zfp, compression_rate, type, dim, 0);
    // zfp_stream_set_precision(zfp, m_precision, type);
    // zfp_stream_set_accuracy(zfp, m_accuracy, type);

    // allocate buffer for compressed data
    bufsize = zfp_stream_maximum_size(zfp, field);
    void *buffer = malloc(bufsize);

    // associate bit stream with allocated buffer
    stream = stream_open(buffer, bufsize);
    zfp_stream_set_bit_stream(zfp, stream);
    zfp_stream_rewind(zfp);

    // compress or decompress entire array

    zfpsize = zfp_compress(zfp, field);

    if (!zfpsize) {
        cout << "compression failed\n";
        status = 1;
    }

    p_jmsg["compressed_size"] = bufsize;
    p_jmsg["compression_method"] = "zfp";

    // clean up
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    return buffer;
}

void* ZfpMan::decompress(void* p_data, json p_jmsg){

    string dtype = p_jmsg["dtype"];
    vector<size_t> shape = p_jmsg["putshape"].get<vector<size_t>>();
    int compression_rate = p_jmsg["compression_rate"].get<int>();

    int status = 0;    // return value: 0 = success
    uint dim = 1;
    zfp_type type = zfp_type_none;     // array scalar type
    zfp_field* field;  // array meta data
    zfp_stream* zfp;   // compressed stream
    size_t bufsize = p_jmsg["compressed_size"].get<size_t>();    // byte size of compressed buffer
    bitstream* stream; // bit stream to write to or read from
    size_t zfpsize;    // byte size of compressed stream

    // allocate meta data for the 3D array a[nz][ny][nx]
    if(dtype == "int"){
        type = zfp_type_int32;
    }
    else if(dtype == "long"){
        type = zfp_type_int64;
    }
    else if(dtype == "float"){
        type = zfp_type_float;
    }
    else if(dtype == "double"){
        type = zfp_type_double;
    }

    void *data;
    data=malloc(product(shape,dsize(dtype)));

    switch (shape.size()){
        case 3:
            field = zfp_field_3d(data, type, shape[0], shape[1], shape[2]);
            dim = 3;
            break;
        case 2:
            field = zfp_field_2d(data, type, shape[0], shape[1]);
            dim = 2;
            break;
        case 1:
            field = zfp_field_1d(data, type, shape[0]);
            break;
        default:
            field = zfp_field_1d(data, type, product(shape));
    }

    zfp = zfp_stream_open(NULL);
    zfp_stream_set_rate(zfp, compression_rate, type, dim, 0);
    stream = stream_open(p_data, bufsize);
    zfp_stream_set_bit_stream(zfp, stream);
    zfp_stream_rewind(zfp);
    if (!zfp_decompress(zfp, field)) {
      fprintf(stderr, "decompression failed\n");
      status = 1;
    }
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    return data;

}

void ZfpMan::transform(const void* p_in, void* p_out, json &p_jmsg){

    string dtype = p_jmsg["dtype"];
    vector<size_t> shape = p_jmsg["putshape"].get<vector<size_t>>();
    int compression_rate = p_jmsg["compression_rate"].get<int>();

    int status = 0;    // return value: 0 = success
    uint dim = 1;
    zfp_type type = zfp_type_none;     // array scalar type
    zfp_field* field;  // array meta data
    zfp_stream* zfp;   // compressed stream
    size_t bufsize = p_jmsg["compressed_size"].get<size_t>();    // byte size of compressed buffer
    bitstream* stream; // bit stream to write to or read from
    size_t zfpsize;    // byte size of compressed stream

    // allocate meta data for the 3D array a[nz][ny][nx]
    if(dtype == "int"){
        type = zfp_type_int32;
    }
    else if(dtype == "long"){
        type = zfp_type_int64;
    }
    else if(dtype == "float"){
        type = zfp_type_float;
    }
    else if(dtype == "double"){
        type = zfp_type_double;
    }

    switch (shape.size()){
        case 3:
            field = zfp_field_3d(p_out, type, shape[0], shape[1], shape[2]);
            dim = 3;
            break;
        case 2:
            field = zfp_field_2d(p_out, type, shape[0], shape[1]);
            dim = 2;
            break;
        case 1:
            field = zfp_field_1d(p_out, type, shape[0]);
            break;
        default:
            field = zfp_field_1d(p_out, type, product(shape));
    }

    zfp = zfp_stream_open(NULL);
    zfp_stream_set_rate(zfp, compression_rate, type, dim, 0);
    stream = stream_open(const_cast<void*>(p_in), bufsize);
    zfp_stream_set_bit_stream(zfp, stream);
    zfp_stream_rewind(zfp);
    if (!zfp_decompress(zfp, field)) {
      fprintf(stderr, "decompression failed\n");
      status = 1;
    }
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    p_jmsg.erase("compression_rate");
    p_jmsg.erase("compression_method");
    p_jmsg.erase("compressed_size");

}

} // end namespace realtime
} // end namespace adios


