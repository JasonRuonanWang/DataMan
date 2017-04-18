#include <iostream>
#include <vector>

#include "ADIOS_CPP.h"

int main(int argc, char *argv[])
{
    adios::ADIOS adios(adios::Verbose::WARN, true);

    std::vector<float> myFloats(1024);
    const std::size_t Nx = myFloats.size();
    for(size_t i=0; i<Nx; i++){
        myFloats[i]=i;
    }

    auto ioMyFloats = adios.DefineVariable<float>("myfloats", adios::Dims{Nx});
    adios::Method &datamanSettings = adios.DeclareMethod("WAN");
    if (!datamanSettings.IsUserDefined())
    {
        datamanSettings.SetEngine("DataManWriter");
        datamanSettings.SetParameters(
                "real_time=yes",
                "method_type=stream",
                "method=zmq",
                "monitoring=yes",
                "local_ip=127.0.0.1",
                "remote_ip=127.0.0.1",
                "local_port=12306",
                "remote_port=12307");
    }

    auto datamanWriter = adios.Open("stream", "w", datamanSettings);
    if (datamanWriter == nullptr)
        throw std::ios_base::failure("ERROR: failed to create DataMan I/O engine at Open\n");

    for(int i=0; i<100; i++){
        try
        {
            datamanWriter->Write<float>(ioMyFloats, myFloats.data());
            datamanWriter->Close();
        }
        catch (std::invalid_argument &e)
        {
            std::cout << "Invalid argument exception, STOPPING PROGRAM\n";
            std::cout << e.what() << "\n";
        }
        catch (std::ios_base::failure &e)
        {
            std::cout << "System exception, STOPPING PROGRAM\n";
            std::cout << e.what() << "\n";
        }
        catch (std::exception &e)
        {
            std::cout << "Exception, STOPPING PROGRAM\n";
            std::cout << e.what() << "\n";
        }

    }
    return 0;
}


