struct mem {
    size_t size;
    size_t cap;
    void *buf;
};

int meminit(struct mem *mem, size_t datasize, size_t cap);
int memgrow(struct mem *mem, size_t datasize);
