
class BumpAllocator {
    uint8_t *_buffer;
    public:
        BumpAllocator( int bytes ) {
            _buffer = new uint8_t[bytes];
        }

        ~BumpAllocator() {
            delete [] _buffer;
            _buffer = nullptr;
        }

};