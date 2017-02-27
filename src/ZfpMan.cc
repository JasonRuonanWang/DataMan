#include "ZfpMan.h"
#include <zfp.h>

ZfpMan::ZfpMan()
    :CompressMan()
{
}

ZfpMan::~ZfpMan(){
}

int ZfpMan::put(const void *p_data, json p_jmsg){

    string p_doid = p_jmsg["doid"];
    string p_var = p_jmsg["var"];
    string p_dtype = p_jmsg["dtype"];
    vector<size_t> p_putshape = p_jmsg["putshape"].get<vector<size_t>>();

    m_rate = 4;
    size_t compressed_size;

    cout << "original data\n";
    cout << "---------------------\n";
    dump(
            p_data,
            p_doid,
            p_var,
            p_dtype,
            p_putshape,
            40
        );

    void* compressed_data = compress(
            const_cast<void*>(p_data),
            p_putshape,
            p_dtype,
            compressed_size
            );

    void* decompressed_data = decompress(
            compressed_data,
            p_putshape,
            p_dtype,
            compressed_size
            );

    cout << "decompressed data\n";
    cout << "---------------------\n";
    dump(
            decompressed_data,
            p_doid,
            p_var,
            p_dtype,
            p_putshape,
            40
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

void* ZfpMan::compress(
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
    if(p_dtype == "int"){
        type = zfp_type_int32;
        cout << "type=int" << endl;
    }
    else if(p_dtype == "long"){
        type = zfp_type_int64;
        cout << "type=long" << endl;
    }
    else if(p_dtype == "float"){
        type = zfp_type_float;
        cout << "type=float" << endl;
    }
    else if(p_dtype == "double"){
        type = zfp_type_double;
        cout << "type=double" << endl;
    }


    switch (p_shape.size()){
        case 3:
            field = zfp_field_3d(p_data, type, p_shape[0], p_shape[1], p_shape[2]);
            dim = 3;
            cout << "dim=3" << ", size=" << product(p_shape) << endl;
            break;
        case 2:
            field = zfp_field_2d(p_data, type, p_shape[0], p_shape[1]);
            dim = 2;
            cout << "dim=2" << ", size=" << product(p_shape) << endl;
            break;
        case 1:
            field = zfp_field_1d(p_data, type, p_shape[0]);
            cout << "dim=1" << ", size=" << product(p_shape) << endl;
            break;
        default:
            field = zfp_field_1d(p_data, type, product(p_shape));
            cout << "dim=x" << ", size=" << product(p_shape) << endl;
    }

    // allocate meta data for a compressed stream
    zfp = zfp_stream_open(NULL);

    // set compression mode and parameters via one of three functions
    zfp_stream_set_rate(zfp, m_rate, type, dim, 0);
    // zfp_stream_set_precision(zfp, m_precision, type);
    // zfp_stream_set_accuracy(zfp, m_accuracy, type);

    // allocate buffer for compressed data
    bufsize = zfp_stream_maximum_size(zfp, field);
    cout << "bufsize=" <<bufsize << endl;
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

    p_compressed_size = bufsize;

    // clean up
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    return buffer;
}

void* ZfpMan::decompress(
        void* buffer,
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
    size_t zfpsize;    // byte size of compressed stream

    // allocate meta data for the 3D array a[nz][ny][nx]
    if(p_dtype == "int"){
        type = zfp_type_int32;
        cout << "type=int" << endl;
    }
    else if(p_dtype == "long"){
        type = zfp_type_int64;
        cout << "type=long" << endl;
    }
    else if(p_dtype == "float"){
        type = zfp_type_float;
        cout << "type=float" << endl;
    }
    else if(p_dtype == "double"){
        type = zfp_type_double;
        cout << "type=double" << endl;
    }

    void *data;
    data=malloc(product(p_shape,dsize(p_dtype)));

    switch (p_shape.size()){
        case 3:
            field = zfp_field_3d(data, type, p_shape[0], p_shape[1], p_shape[2]);
            dim = 3;
            break;
        case 2:
            field = zfp_field_2d(data, type, p_shape[0], p_shape[1]);
            dim = 2;
            break;
        case 1:
            field = zfp_field_1d(data, type, p_shape[0]);
            break;
        default:
            field = zfp_field_1d(data, type, product(p_shape));
    }

    zfp = zfp_stream_open(NULL);
    zfp_stream_set_rate(zfp, 4, type, dim, 0);
    bufsize = p_compressed_size;
    stream = stream_open(buffer, bufsize);
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
