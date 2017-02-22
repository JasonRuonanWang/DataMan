#include "ZfpMan.h"
#include <zfp.h>

ZfpMan::ZfpMan()
    :CompressMan()
{
}

ZfpMan::~ZfpMan(){
}


int ZfpMan::put(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_putshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t p_timestep,
        int p_tolerance,
        int p_priority){

    int status = 0;    /* return value: 0 = success */
    zfp_type type;     /* array scalar type */
    zfp_field* field;  /* array meta data */
    zfp_stream* zfp;   /* compressed stream */
    void* buffer;      /* storage for compressed stream */
    size_t bufsize;    /* byte size of compressed buffer */
    bitstream* stream; /* bit stream to write to or read from */
    size_t zfpsize;    /* byte size of compressed stream */

    /* allocate meta data for the 3D array a[nz][ny][nx] */
    type = zfp_type_double;

    field = zfp_field_3d(p_data, type, p_putshape[0], p_putshape[1], p_putshape[2]);

    /* allocate meta data for a compressed stream */
    zfp = zfp_stream_open(NULL);

    /* set compression mode and parameters via one of three functions */
    /*  zfp_stream_set_rate(zfp, rate, type, 3, 0); */
    /*  zfp_stream_set_precision(zfp, precision, type); */
    zfp_stream_set_accuracy(zfp, p_tolerance, type);

    /* allocate buffer for compressed data */
    bufsize = zfp_stream_maximum_size(zfp, field);
    buffer = malloc(bufsize);

    /* associate bit stream with allocated buffer */
    stream = stream_open(buffer, bufsize);
    zfp_stream_set_bit_stream(zfp, stream);
    zfp_stream_rewind(zfp);

    /* compress or decompress entire array */
    /*
    if (decompress) {
        zfpsize = fread(buffer, 1, bufsize, stdin);
        if (!zfp_decompress(zfp, field)) {
            fprintf(stderr, "decompression failed\n");
            status = 1;
        }
    }
    else {
    }
    */
        zfpsize = zfp_compress(zfp, field);
        if (!zfpsize) {
            fprintf(stderr, "compression failed\n");
            status = 1;
        }
        else
            fwrite(buffer, 1, zfpsize, stdout);

    /* clean up */
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);
    free(buffer);


    return 0;
}

int ZfpMan::get(void *p_data,
        string p_doid,
        string p_var,
        string p_dtype,
        vector<uint64_t> p_getshape,
        vector<uint64_t> p_varshape,
        vector<uint64_t> p_offset,
        uint64_t p_timestep
        ){

    return 0;
}

int ZfpMan::get(void *p_data,
        string p_doid,
        string p_var,
        string &p_dtype,
        vector<uint64_t> &p_varshape,
        uint64_t &p_timestep
        ){

    return 0;
}

void ZfpMan::flush(){

}

