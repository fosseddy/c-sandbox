struct mem {
    unsigned long size;
    unsigned long cap;
    unsigned long data_size;
    void *buf;
};

int meminit(struct mem *m, unsigned long data_size, unsigned long cap);
int memgrow(struct mem *m);
void memfree(struct mem *m);
