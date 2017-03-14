#include "ZfpMan.h"
#include <zfp.h>

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
        if (p_jmsg["compressrate"] == nullptr){
            p_jmsg["compressrate"] = 4;
        }
        string doid = p_jmsg["doid"];
        string var = p_jmsg["var"];
        string dtype = p_jmsg["dtype"];
        vector<size_t> putshape = p_jmsg["putshape"].get<vector<size_t>>();

        compressed_data = compress(const_cast<void*>(p_data), p_jmsg);

        /*
        cout << "original data";  cout << "---------------------\n";
        p_jmsg["dumplength"] = 30;
        dump(p_data, p_jmsg);
        void* decompressed_data = decompress(compressed_data, p_jmsg);
        cout << "decompressed data\n"; cout << "---------------------\n";
        dump(decompressed_data, p_jmsg);
        */
    }

    put_next(p_data, p_jmsg);
    free(compressed_data);
    put_end(p_data, p_jmsg);
    return 0;
}

int ZfpMan::get(void *p_data, json &p_jmsg){
    return 0;
}

void ZfpMan::flush(){

}

void* ZfpMan::compress(void* p_data, json &p_jmsg){

    string dtype = p_jmsg["dtype"];
    vector<size_t> shape = p_jmsg["putshape"].get<vector<size_t>>();
    int compressrate = p_jmsg["compressrate"].get<int>();

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
        cout << "type=int" << endl;
    }
    else if(dtype == "long"){
        type = zfp_type_int64;
        cout << "type=long" << endl;
    }
    else if(dtype == "float"){
        type = zfp_type_float;
        cout << "type=float" << endl;
    }
    else if(dtype == "double"){
        type = zfp_type_double;
        cout << "type=double" << endl;
    }

    switch (shape.size()){
        case 3:
            field = zfp_field_3d(p_data, type, shape[0], shape[1], shape[2]);
            dim = 3;
            cout << "dim=3" << ", size=" << product(shape) << endl;
            break;
        case 2:
            field = zfp_field_2d(p_data, type, shape[0], shape[1]);
            dim = 2;
            cout << "dim=2" << ", size=" << product(shape) << endl;
            break;
        case 1:
            field = zfp_field_1d(p_data, type, shape[0]);
            cout << "dim=1" << ", size=" << product(shape) << endl;
            break;
        default:
            field = zfp_field_1d(p_data, type, product(shape));
            cout << "dim=x" << ", size=" << product(shape) << endl;
    }

    // allocate meta data for a compressed stream
    zfp = zfp_stream_open(NULL);

    // set compression mode and parameters via one of three functions
    zfp_stream_set_rate(zfp, compressrate, type, dim, 0);
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

    p_jmsg["compressedsize"] = bufsize;

    // clean up
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    return buffer;
}

void* ZfpMan::decompress(void* p_data, json p_jmsg){

    string dtype = p_jmsg["dtype"];
    vector<size_t> shape = p_jmsg["putshape"].get<vector<size_t>>();
    int compressrate = p_jmsg["compressrate"].get<int>();

    int status = 0;    // return value: 0 = success
    uint dim = 1;
    zfp_type type = zfp_type_none;     // array scalar type
    zfp_field* field;  // array meta data
    zfp_stream* zfp;   // compressed stream
    size_t bufsize = p_jmsg["compressedsize"].get<size_t>();    // byte size of compressed buffer
    bitstream* stream; // bit stream to write to or read from
    size_t zfpsize;    // byte size of compressed stream

    // allocate meta data for the 3D array a[nz][ny][nx]
    if(dtype == "int"){
        type = zfp_type_int32;
        cout << "type=int" << endl;
    }
    else if(dtype == "long"){
        type = zfp_type_int64;
        cout << "type=long" << endl;
    }
    else if(dtype == "float"){
        type = zfp_type_float;
        cout << "type=float" << endl;
    }
    else if(dtype == "double"){
        type = zfp_type_double;
        cout << "type=double" << endl;
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
    zfp_stream_set_rate(zfp, compressrate, type, dim, 0);
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
