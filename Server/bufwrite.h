#ifndef include_LEAPrint_bufwrite_h
#define include_LEAPrint_bufwrite_h

inline cairo_status_t bufwrite(void* bp, unsigned char const* data, unsigned int len) {
    std::vector<uint8_t>& buffer = *((std::vector<uint8_t>*) bp);
    std::copy(data, data + len, std::back_inserter(buffer));
    return CAIRO_STATUS_SUCCESS;
}

#endif