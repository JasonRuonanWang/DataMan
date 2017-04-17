#include <iostream>
#include <vector>

#include "ADIOS_CPP.h"

int main(int argc, char *argv[])
{
    const bool adiosDebug = true;
    adios::ADIOS adios(adios::Verbose::WARN, adiosDebug);

    std::vector<float> myFloats = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> myDoubles = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const std::size_t Nx = myDoubles.size();

    std::vector<std::complex<float>> myCFloats;
    myCFloats.reserve(3);
    myCFloats.emplace_back(1, 3);
    myCFloats.emplace_back(2, 2);
    myCFloats.emplace_back(3, 1);

    try
    {
        auto ioMyFloats = adios.DefineVariable<float>("myfloats", adios::Dims{Nx});
        auto ioMyFloat = adios.DefineVariable<float>("myfloat", adios::Dims{1});

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

        datamanWriter->Write<float>(ioMyFloats, myFloats.data());
        const float num = 1.12;
        datamanWriter->Write<float>(ioMyFloat, &num);
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

    return 0;
}
