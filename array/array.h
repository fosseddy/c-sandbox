struct array {
    size_t size;
    size_t cap;
    size_t membsize;
    void *buf;
};

int array_init(struct array *a, size_t membsize, size_t cap);
void array_free(struct array *a);

int array_add(struct array *a, void *value);
void *array_at(struct array *a, size_t index);
