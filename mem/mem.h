struct mem {
    size_t size;
    size_t cap;
    void *buf;
};

int meminit(void *mem, size_t datasize, size_t cap);
int memgrow(void *mem, size_t datasize);
void memfree(void *mem);
