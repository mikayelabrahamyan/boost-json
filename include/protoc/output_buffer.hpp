#ifndef PROTOC_OUTPUT_BUFFER_HPP
#define PROTOC_OUTPUT_BUFFER_HPP

namespace protoc
{

template<typename Encoder>
class output_buffer
{
public:
    output_buffer()
        : output(buffer.data(), buffer.data() + buffer.size())
    {
    }

    ~output_buffer()
    {
    }

    void put() // Null
    {
        std::size_t size = encoder.capacity();
        // FIXME: Extend buffer if needed
        encoder.put();
    }

private:
    std::string buffer;
    Encoder output;
};

}

#endif /* PROTOC_OUTPUT_BUFFER_HPP */
