struct mem {
    int size;
    int cap;
    int data_size;
    void *buf;
};

int meminit(struct mem *m, int data_size, int cap);
int memgrow(struct mem *m);
void *memnext(struct mem *m);
void memfree(struct mem *m);
